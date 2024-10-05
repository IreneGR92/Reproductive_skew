#include <thread>
#include "SimulationRunner.h"
#include "util/FilePrinter.h"
#include "spdlog/spdlog.h"

void SimulationRunner::run(const std::string &parameterFilePath) {
    if (!parameterFilePath.empty()) {
        // Initialize parameters from the provided file
        parameters = std::make_shared<Parameters>(parameterFilePath, 0);
    } else {
        // Initialize parameters with default values
        parameters = std::make_shared<Parameters>(0);
    }
    spdlog::debug("Starting: {}", parameters->getName());
    std::vector<std::unique_ptr<ResultCache>> results(parameters->getMaxNumReplicates());
#ifdef NDEBUG
    // Run the simulation in multi-threaded mode
    runMultithreaded(results);
#else
    // Run the simulation in single-threaded mode
    runSinglethreaded(results);
#endif

    // Print the results to files
    FilePrinter filePrinter(parameters);
    filePrinter.writeMainFile(results);
    filePrinter.writeLastGenerationFile(results);
}

void SimulationRunner::runSimulation(std::unique_ptr<Simulation> simulation, std::unique_ptr<ResultCache> &result) {
    result = simulation->run();
}

void SimulationRunner::runMultithreaded(std::vector<std::unique_ptr<ResultCache>> &results) {
    spdlog::debug("Running multi-threaded mode");
    std::vector<std::thread> threads;
    for (int replica = 0; replica < parameters->getMaxNumReplicates(); replica++) {
        auto newParams = parameters->cloneWithIncrementedReplica(replica);
        auto simulation = std::make_unique<Simulation>(newParams);
        threads.emplace_back([this, simulation = std::move(simulation), &results, replica]() mutable {
            runSimulation(std::move(simulation), results[replica]);
        });
    }
    for (auto &thread: threads) {
        thread.join();
    }
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
