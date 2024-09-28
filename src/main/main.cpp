/***********************************************
 GROUP AUGMENTATION MODEL
 - Passive group augmentation: individuals help in order to increase group size which in turn increases survival
 - Active group augmentation or delayed reciprocity: if inherit the breeding position, individuals benefit of the help given by the recruits in the group.


 Stochastic individual based model.
 Limited breeding spots, unlimited population size; one breeder per group.
 Population overlap, turn over when breeder dies. 
 Older/more dominant individual higher probability of becoming new breeder.
 Evolution of level of help and dispersal. Inclusion of reaction norm to age.
 Dispersal produces dispersers/floaters that can reproduce in another group or join as helpers.
 Survival dependent on group size, level of help and predation/environment.
 Fecundity dependent on cumulative level of help within group.
 Relatedness as an emergent property.

***********************************************/

/*HEADER FILES*/

#include <thread>
#include "util/Parameters.h"
#include "stats/Statistics.h"
#include "util/FilePrinter.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

void runSimulation(Simulation *simulation, ResultCache **result) {
    // Run the simulation and store the result in the provided ResultCache pointer
    *result = simulation->run();
    // Clean up the simulation object
    delete simulation;
}

void runMultithreaded(Parameters &parameters, std::vector<ResultCache *> &results) {
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

void runSinglethreaded(Parameters &parameters, std::vector<ResultCache *> &results) {
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

/* MAIN PROGRAM */
int main(int count, char **argv) {
    // Define the log pattern

    // Set the log level to debug (shows all levels: trace, debug, info, warn, error, critical)
#ifdef NDEBUG
    std::string log_pattern_file = "[%Y-%m-%d %H:%M:%S.%e] [%l] %v";
    auto file_logger = spdlog::basic_logger_mt("file_logger", "reproductive_skew.log");
    spdlog::set_default_logger(file_logger);
    file_logger->set_pattern(log_pattern_file);
    spdlog::set_level(spdlog::level::info); // Release build
#else
    std::string log_pattern_console = "[%l] %v";
    auto console_logger = spdlog::stdout_color_mt("console_logger");
    spdlog::set_default_logger(console_logger);
    console_logger->set_pattern(log_pattern_console);
    spdlog::set_level(spdlog::level::debug); // Debug build
#endif

    Parameters *parameters;
    if (count > 1) {
        // Initialize parameters from the provided file
        parameters = new Parameters(argv[1], 0);
    } else {
        // Initialize parameters with default values
        parameters = new Parameters(0);
    }
    std::vector<ResultCache *> results(parameters->getMaxNumReplicates());
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

    delete parameters;

    return 0;
}


