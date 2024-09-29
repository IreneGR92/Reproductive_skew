#include <thread>
#include "SimulationRunner.h"
#include "util/FilePrinter.h"
#include "spdlog/spdlog.h"

void SimulationRunner::run(const std::string &parameterFilePath) {

    if (!parameterFilePath.empty()) {
        // Initialize parameters from the provided file
        parameters = new Parameters(parameterFilePath, 0);
    } else {
        // Initialize parameters with default values
        parameters = new Parameters(0);
    }
    spdlog::info("Starting: {}", parameters->getName());
    std::vector<ResultCache *> results(0);
#ifdef NDEBUG
    // Run the simulation in multi-threaded mode
    runMultithreaded(*parameters, results);
#else
    // Run the simulation in single-threaded mode
    runSinglethreaded(*parameters, results);
#endif

    // Print the results to files
    FilePrinter filePrinter(parameters);
    filePrinter.writeMainFile(results);
    filePrinter.writeLastGenerationFile(results);
    spdlog::info("Finished: {}", parameters->getName());
    delete parameters;
}

void SimulationRunner::runSimulation(Simulation *simulation, ResultCache **result) {
    // Run the simulation and store the result in the provided ResultCache pointer
    *result = simulation->run();
    // Clean up the simulation object
    delete simulation;
}

void SimulationRunner::runMultithreaded(Parameters &parameters, std::vector<ResultCache *> &results) {
    spdlog::info("Running multi-threaded mode");
    std::vector<std::thread> threads;
    // Create and start a thread for each replica
    for (int replica = 0; replica < parameters.getMaxNumReplicates(); replica++) {
        // Clone parameters for the current replica
        auto *newParams = parameters.cloneWithIncrementedReplica(replica);
        // Create a new simulation instance with the cloned parameters
        auto *simulation = new Simulation(newParams);
        // Start the simulation in a new thread
        threads.emplace_back(runSimulation, simulation, &results[replica]);
    }

    // Wait for all threads to finish
    for (auto &thread: threads) {
        thread.join();
    }

}

void SimulationRunner::runSinglethreaded(Parameters &parameters, std::vector<ResultCache *> &results) {

    spdlog::info("Running single-threaded mode");
    // Run each replica sequentially
    for (int replica = 0; replica < parameters.getMaxNumReplicates(); replica++) {
        // Clone parameters for the current replica
        auto *newParams = parameters.cloneWithIncrementedReplica(replica);
        // Create a new simulation instance with the cloned parameters
        auto *simulation = new Simulation(newParams);
        // Run the simulation and store the result
        results.emplace_back(simulation->run());
    }

}
