#include <thread>
#include "SimulationRunner.h"
#include "util/FilePrinter.h"
#include "spdlog/spdlog.h"
#include "util/Config.h"

void SimulationRunner::run(const std::string &parameterFilePath, const std::shared_ptr<ThreadPool> &threadPool) {
    if (!parameterFilePath.empty()) {
        // Initialize parameters from the provided file
        parameters = std::make_shared<Parameters>(parameterFilePath, 0);
    } else {
        // Initialize parameters with default values
        parameters = std::make_shared<Parameters>(0);
    }
    spdlog::debug("Starting: {}", parameters->getName());
    std::vector<std::unique_ptr<ResultCache> > results(parameters->getMaxNumReplicates());


    if (Config::IS_MULTITHREADED())
        // Run the simulation in multithreaded mode
        runMultithreaded(results, threadPool);
    else
        // Run the simulation in single-threaded mode
        runSinglethreaded(results);


    // Print the results to files
    FilePrinter filePrinter(parameters);
    filePrinter.writeMainFile(results);
    filePrinter.writeLastGenerationFile(results);
}

void SimulationRunner::runSimulation(std::shared_ptr<Simulation> simulation, std::unique_ptr<ResultCache> &result) {
    result = simulation->run();
}

void SimulationRunner::runMultithreaded(std::vector<std::unique_ptr<ResultCache>> &results,
                                        const std::shared_ptr<ThreadPool> &threadPool) {
    spdlog::debug("Running multi-threaded mode");

    std::atomic<int> tasksRemaining(parameters->getMaxNumReplicates());
    std::mutex completionMutex;
    std::condition_variable completionCondition;


    for (int replica = 0; replica < parameters->getMaxNumReplicates(); replica++) {
        auto newParams = parameters->cloneWithIncrementedReplica(replica);
        auto simulation = std::make_shared<Simulation>(newParams);

        threadPool->enqueue(
                [simulation, &results, replica, &tasksRemaining, &completionMutex, &completionCondition]() mutable {
                    runSimulation(simulation, results[replica]);
                    if (--tasksRemaining == 0) {
                        std::lock_guard<std::mutex> lock(completionMutex);
                        completionCondition.notify_one();

                    }
                    spdlog::trace("replica {} completed: task remaining: ", replica, tasksRemaining);
                });
    }
    std::unique_lock<std::mutex> lock(completionMutex);
    completionCondition.wait(lock, [&tasksRemaining] { return tasksRemaining == 0; });
}

void SimulationRunner::runSinglethreaded(std::vector<std::unique_ptr<ResultCache>> &results) {
    spdlog::debug("Running single-threaded mode");
    results.clear();
    // Run each replica sequentially
    for (int replica = 0; replica < parameters->getMaxNumReplicates(); replica++) {
        // Clone parameters for the current replica
        auto newParams = parameters->cloneWithIncrementedReplica(replica);
        // Create a new simulation instance with the cloned parameters
        auto simulation = std::make_unique<Simulation>(newParams);
        // Run the simulation and store the result
        results.emplace_back(simulation->run());
    }
}
