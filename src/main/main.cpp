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
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "SimulationRunner.h"
#include "spdlog/sinks/basic_file_sink.h"

void setupLogging() {
#ifdef NDEBUG
    std::string log_pattern_file = "[%Y-%m-%d %H:%M] [%l] %v";
    auto file_logger = spdlog::basic_logger_mt("file_logger", "reproductive_skew.log");
    file_logger->flush_on(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);
    file_logger->set_pattern(log_pattern_file);
    spdlog::set_level(spdlog::level::info);
#else
    std::string log_pattern_console = "[%l] %v";
    auto console_logger = spdlog::stdout_color_mt("console_logger");
    spdlog::set_default_logger(console_logger);
    console_logger->set_pattern(log_pattern_console);
    spdlog::set_level(spdlog::level::debug);
#endif
}


void runSimulations(const std::vector<std::string> &parameters) {
    const int MAX_NUMBER_OF_CONCURRENT_EXECUTIONS = 1;
    int executionCount = 0;
    std::vector<std::thread> threads;

    for (size_t i = 0; i < parameters.size(); ++i) {
        const std::string &parameterFilename = parameters[i];
        while (executionCount >= MAX_NUMBER_OF_CONCURRENT_EXECUTIONS) {
            std::this_thread::sleep_for(std::chrono::minutes(1));
        }
        spdlog::info("start {}: {} of {}", parameterFilename, i + 1, parameters.size());
        threads.emplace_back([parameterFilename, &executionCount]() {
            auto simulationRunner = std::make_unique<SimulationRunner>();
            simulationRunner->run(parameterFilename);
            executionCount--;
            spdlog::info("finish {}", parameterFilename);
        });
        executionCount++;
    }

    for (auto &thread: threads) {
        thread.join();
    }
}

static std::vector<std::string> loadParameterFiles();


/* MAIN PROGRAM */
int main() {
    // Set the log level to debug (shows all levels: trace, debug, info, warn, error, critical)
    setupLogging();

    // Load parameter files
    auto parameters = loadParameterFiles();

    // Run the simulations
    runSimulations(parameters);
    return 0;
}


// Function to load parameter files from a specified path
static std::vector<std::string> loadParameterFiles() {
#ifdef NDEBUG
    std::string filePath = "../parameters/parameters.yml";
#else
    std::string filePath = "../parameters/parameters_debug.yml";
#endif
    std::vector<std::string> parameterFiles;
    std::ifstream file(filePath);
    std::string line;

    // Read each line from the file and add to the parameter files vector
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.empty()) continue; //ignore empty lines
            if (line[0] == '#') continue; //ignore comments
            parameterFiles.push_back("../parameters/" + line);
        }
        file.close();
    } else {
        spdlog::error("Unable to open file: {} ", filePath);
    }
    return parameterFiles;
}

static std::vector<std::string> loadDefaultParameters() {
    std::vector<std::string> parameterFiles;
    parameterFiles.emplace_back("../parameters/default_parameters.txt");
    return parameterFiles;
}
