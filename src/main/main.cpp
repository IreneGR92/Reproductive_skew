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
#include <csignal>
#include <thread>
#include "spdlog/spdlog.h"
#include "SimulationRunner.h"
#include "loadbalancing/ThreadPool.h"
#include "util/Config.h"
#include "util/Util.h"

std::atomic stopFlag(false);


void signalHandler(int signum) {
    stopFlag.store(true);
    spdlog::info("Turn off Signal triggered {}", signum);
}


/* MAIN PROGRAM */
int main() {
    // Register signal handler
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    //load config file
    Config::loadConfig();

    Util::setupLogging();


    std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>(
        Config::GET_MAX_THREADS()); // Create a thread pool with 4 threads

    // Load parameter files
    auto parameters = Util::loadParameterFiles();
    spdlog::info("Loaded {} parameter files", parameters.size());
    // Run the simulations
    SimulationRunner::runSimulations(parameters, pool, stopFlag);
    spdlog::drop_all();
    return 0;
}
