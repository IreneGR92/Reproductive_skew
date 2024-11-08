#include <thread>
#include "SimulationRunner.h"
#include "util/FilePrinter.h"
#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"


void SimulationRunner::run(const std::string &parameterFilePath, const std::shared_ptr<ThreadPool> &threadPool,
                           std::condition_variable &theadPoolEmptyCondition) {
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


    spawnWorkerThreads(results, threadPool, theadPoolEmptyCondition);


    // Print the results to files
    FilePrinter filePrinter(parameters);
    filePrinter.writeMainFile(results);
    filePrinter.writeLastGenerationFile(results);
}

void SimulationRunner::runSimulations(const std::vector<std::string> &parameters,
                                      std::shared_ptr<ThreadPool> &threadPool, std::atomic<bool> &stopFlag) {
    std::vector<std::thread> threads;
    std::mutex mtx;
    std::condition_variable theadPoolEmptyCondition;

    for (size_t i = 0; i < parameters.size(); i++) {
        const std::string &parameterFilename = parameters[i];

        spdlog::info("start {}: {} of {}", parameterFilename, i + 1, parameters.size());
        if (stopFlag == false) {
            threads.emplace_back([parameterFilename, threadPool, &theadPoolEmptyCondition]() {
                auto simulationRunner = std::make_unique<SimulationRunner>();
                simulationRunner->run(parameterFilename, threadPool, theadPoolEmptyCondition);
                spdlog::info("finish {}", parameterFilename);
            });
        }

        spdlog::trace("waiting.. Thread pool length: {}", threadPool->queueLength());
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lock(mtx);
        theadPoolEmptyCondition.wait(lock, [threadPool] { return threadPool->empty(); });
        spdlog::trace("continue Thread pool length: {}", threadPool->queueLength());


        // Check if a stop signal was received
        if (stopFlag) {
            spdlog::info("Gracefully stopping runs.. finishing current simulation queue empty: {}",
                         threadPool->queueLength());
            spdlog::info("NEXT simulation (not done yet) is: {}", parameters[i + 1]);
            break;
        }
    }

    // Join all remaining threads
    for (auto &thread: threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // Log the completion status
    if (stopFlag) { spdlog::info("Not all simulations completed");} else { spdlog::info("All simulations completed");}
}

void SimulationRunner::runSimulation(std::shared_ptr<Simulation> simulation, std::unique_ptr<ResultCache> &result,
                                     std::condition_variable &theadPoolEmptyCondition) {
    result = simulation->run();
}

void SimulationRunner::spawnWorkerThreads(std::vector<std::unique_ptr<ResultCache> > &results,
                                          const std::shared_ptr<ThreadPool> &threadPool,
                                          std::condition_variable &theadPoolEmptyCondition) const {
    std::atomic<int> tasksRemaining(parameters->getMaxNumReplicates());
    std::mutex completionMutex;
    std::condition_variable completionCondition;


    for (int replica = 0; replica < parameters->getMaxNumReplicates(); replica++) {
        auto newParams = parameters->cloneWithIncrementedReplica(replica);
        auto simulation = std::make_shared<Simulation>(newParams);

        threadPool->enqueue(
            [simulation, &results, replica, &tasksRemaining, &completionMutex, &completionCondition, &
                theadPoolEmptyCondition]() mutable {
                spdlog::trace("replica {} started: task remaining: {}", replica, tasksRemaining);
                runSimulation(simulation, results[replica], theadPoolEmptyCondition);
                if (--tasksRemaining == 0) {
                    std::lock_guard<std::mutex> lock(completionMutex);
                    completionCondition.notify_one();
                }
                theadPoolEmptyCondition.notify_one();
                spdlog::trace("replica {} completed: task remaining: {}", replica, tasksRemaining);
            });
    }
    spdlog::debug("Waiting for completion");
    std::unique_lock<std::mutex> lock(completionMutex);
    completionCondition.wait(lock, [&tasksRemaining] { return tasksRemaining == 0; });
    spdlog::debug("All tasks completed --> Simulation completed for: {}", parameters->getName());
}
