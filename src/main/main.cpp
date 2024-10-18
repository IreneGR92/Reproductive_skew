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
#include "spdlog/spdlog.h"
#include "SimulationRunner.h"
#include "loadbalancing/ThreadPool.h"
#include "util/Config.h"
#include "util/Util.h"

void runSimulations(const std::vector<std::string> &parameters, std::shared_ptr<ThreadPool> &threadPool);


/* MAIN PROGRAM */
int main() {
    //load config file
    Config::loadConfig();

    Util::setupLogging();

    std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(
        Config::GET_MAX_THREADS()); // Create a thread pool with 4 threads

    // Load parameter files
    auto parameters = Util::loadParameterFiles();
    spdlog::info("Loaded {} parameter files", parameters.size());
    // Run the simulations
    runSimulations(parameters, pool);
    spdlog::drop_all();
    return 0;
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
        std::this_thread::sleep_for(std::chrono::seconds(1));

        threadPool->subscribeToPoolEmpty([&completionCondition] {
            completionCondition.notify_one();
        });
        spdlog::trace("waiting.. Thread pool length: {}", threadPool->queueLength());

        std::unique_lock lock(completionMutex);
        completionCondition.wait(lock, [threadPool] { return threadPool->queueLength() == 0; });
        spdlog::trace("continue Thread pool length: {}", threadPool->queueLength());
    }
    std::mutex finishedMutex;
    std::unique_lock lock(finishedMutex);
    finishedCondition.wait(lock, [&simulationCount, &parameters] { return simulationCount == parameters.size(); });
    spdlog::info("All simulations completed");

    // Join all remaining threads
    for (auto &thread: threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
