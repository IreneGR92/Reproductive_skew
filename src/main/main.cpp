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
    *result = simulation->run();
    delete simulation;
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
        parameters = new Parameters(argv[1], 0);
    } else {
        parameters = new Parameters(0);
    }

    std::vector<std::thread> threads;
    std::vector<ResultCache *> results(parameters->getMaxNumReplicates());

    for (int replica = 0; replica < parameters->getMaxNumReplicates(); replica++) {

        auto *newParams = parameters->cloneWithIncrementedReplica(replica);
        auto *simulation = new Simulation(newParams);

        threads.emplace_back(runSimulation, simulation, &results[replica]);
    }

    for (auto &thread: threads) {
        thread.join();
    }

    FilePrinter filePrinter(parameters);
    filePrinter.writeMainFile(results);
    filePrinter.writeLastGenerationFile(results);

    return 0;
}


