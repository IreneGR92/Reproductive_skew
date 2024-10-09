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
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "SimulationRunner.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "loadbalancing/ThreadPool.h"
#include "util/Config.h"


static std::vector<std::string> loadParameterFiles();

void runSimulations(const std::vector<std::string> &parameters, std::shared_ptr<ThreadPool> &threadPool);

void setupLogging();


/* MAIN PROGRAM */
int main() {
    // Set the log level to debug (shows all levels: trace, debug, info, warn, error, critical)
    setupLogging();

    //load config file
    Config::loadConfig();

    std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(
            Config::GET_MAX_THREADS());// Create a thread pool with 4 threads

    // Load parameter files
    auto parameters = loadParameterFiles();

    // Run the simulations
    runSimulations(parameters, pool);

    return 0;
}


// Function to load parameter files from a specified path
static std::vector<std::string> loadParameterFiles() {
#ifdef NDEBUG
    std::string filePath = Config::GET_PARAMETERS_FOLDER() + "/parameters_debug.yml";
#else
    std::string filePath = Config::GET_PARAMETERS_FOLDER() + "/parameters_debug.yml";
#endif
    std::vector<std::string> parameterFiles;
    std::ifstream file(filePath);
    std::string line;

    // Read each line from the file and add to the parameter files vector
    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.empty()) continue; //ignore empty lines
            if (line[0] == '#') continue; //ignore comments
            parameterFiles.push_back(Config::GET_PARAMETERS_FOLDER() + "/" + line);
        }
        file.close();
    } else {
        spdlog::error("Unable to open file: {} ", filePath);
    }
    return parameterFiles;
}

void setupLogging() {
#ifdef NDEBUG
    std::string log_pattern_file = "[%Y-%m-%d %H:%M] [%l] %v";
    auto file_logger = spdlog::basic_logger_mt("file_logger", "reproductive_skew.log");
    file_logger->flush_on(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);
    file_logger->set_pattern(log_pattern_file);
    spdlog::set_level(spdlog::level::info);
#else
    std::string log_pattern_console = "[%l][%t] %v";
    auto console_logger = spdlog::stdout_color_mt("console_logger");
    spdlog::set_default_logger(console_logger);
    console_logger->set_pattern(log_pattern_console);
    spdlog::set_level(spdlog::level::trace);
#endif
}

/**
 * Runs the simulations based on the provided parameters and thread pool.
 *
 * @param parameters A vector of strings containing the parameter file names.
 * @param threadPool A shared pointer to the ThreadPool used for managing threads.
 */
void runSimulations(const std::vector<std::string> &parameters, std::shared_ptr<ThreadPool> &threadPool) {

    std::vector<std::thread> threads;
    std::condition_variable completionCondition;
    std::mutex completionMutex;

    int simulationCount = 0;
    std::condition_variable finishedCondition;

    for (size_t i = 0; i < parameters.size(); ++i) {
        const std::string &parameterFilename = parameters[i];

        spdlog::trace("Thread pool length: {}", threadPool->queueLength());
        spdlog::info("start {}: {} of {}", parameterFilename, i + 1, parameters.size());
        threads.emplace_back([parameterFilename, threadPool, &simulationCount, &finishedCondition]() {
            auto simulationRunner = std::make_unique<SimulationRunner>();
            simulationRunner->run(parameterFilename, threadPool, simulationCount, finishedCondition);
            spdlog::info("finish {}", parameterFilename);
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        threadPool->subscribeToPoolEmpty([&completionCondition] {
            completionCondition.notify_one();
        });
        spdlog::trace("waiting.. Thread pool length: {}", threadPool->queueLength());

        std::unique_lock<std::mutex> lock(completionMutex);
        completionCondition.wait(lock, [threadPool] { return threadPool->queueLength() == 0; });
        spdlog::trace("continue Thread pool length: {}", threadPool->queueLength());

    }
    std::mutex finishedMutex;
    std::unique_lock<std::mutex> lock(finishedMutex);
    finishedCondition.wait(lock, [&simulationCount, &parameters] { return simulationCount == parameters.size(); });
    spdlog::info("All simulations completed");

    // Join all remaining threads
    for (auto &thread: threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}