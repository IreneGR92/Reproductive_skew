#include <thread>
#include "SimulationRunner.h"
#include "util/FilePrinter.h"
#include "spdlog/spdlog.h"

namespace YAML {
    class BadFile;
}

void SimulationRunner::run(const std::string &parameterFilePath, const std::shared_ptr<ThreadPool> &threadPool,
                           std::condition_variable &completionCondition) {
    if (!parameterFilePath.empty()) {
        // Initialize parameters from the provided file
        try {
            parameters = std::make_shared<Parameters>(parameterFilePath, 0);
        } catch (YAML::BadFile &e) {
            spdlog::error("unable to run simulation: {} -> skipping", parameterFilePath);
            return;
        }
    } else {
        // Initialize parameters with default values
        parameters = std::make_shared<Parameters>(0);
    }
    spdlog::debug("Starting: {}", parameters->getName());
    std::vector<std::unique_ptr<ResultCache> > results(parameters->getMaxNumReplicates());


    spawnWorkerThreads(results, threadPool);


    // Print the results to files
    FilePrinter filePrinter(parameters);
    filePrinter.writeMainFile(results);
    filePrinter.writeLastGenerationFile(results);
    completionCondition.notify_one();
}

void SimulationRunner::runSimulation(std::shared_ptr<Simulation> simulation, std::unique_ptr<ResultCache> &result) {
    result = simulation->run();
}

void SimulationRunner::spawnWorkerThreads(std::vector<std::unique_ptr<ResultCache> > &results,
                                          const std::shared_ptr<ThreadPool> &threadPool) {
    std::atomic<int> tasksRemaining(parameters->getMaxNumReplicates());
    std::mutex completionMutex;
    std::condition_variable completionCondition;


    for (int replica = 0; replica < parameters->getMaxNumReplicates(); replica++) {
        auto newParams = parameters->cloneWithIncrementedReplica(replica);
        auto simulation = std::make_shared<Simulation>(newParams);

        threadPool->enqueue(
            [simulation, &results, replica, &tasksRemaining, &completionMutex, &completionCondition]() mutable {
                spdlog::trace("replica {} started: task remaining: {}", replica, tasksRemaining);
                runSimulation(simulation, results[replica]);
                if (--tasksRemaining == 0) {
                    std::lock_guard<std::mutex> lock(completionMutex);
                    completionCondition.notify_one();
                }
                spdlog::trace("replica {} completed: task remaining: {}", replica, tasksRemaining);
            });
    }
    spdlog::debug("Waiting for completion");
    std::unique_lock<std::mutex> lock(completionMutex);
    completionCondition.wait(lock, [&tasksRemaining] { return tasksRemaining == 0; });
    spdlog::debug("All tasks completed");
}
