#include "Simulation.h"
#include "stats/Statistics.h"


std::unique_ptr<ResultCache> Simulation::run() {
    // Output file
    auto *statistics = new Statistics(parameters);
    auto results = std::make_unique<ResultCache>(parameters, parameters->getReplica());
    statistics->calculateStatistics(population);
    statistics->printHeadersToConsole();
    statistics->printToConsole(generation, population.getDeaths(), population.getEmigrants());
    //    statistics->writeToCacheMain(replica, generation, population.getDeaths(), newBreederOutsider, newBreederInsider, inheritance);

    delete statistics;

    for (generation = 1; generation <= parameters->getNumGenerations(); generation++) {
        statistics = new Statistics(this->parameters);
        population.reset();

        population.disperse();
        population.survivalFloaters();
        population.mortalityFloaters();
        population.immigrate();
        population.reassignBreeder();
        population.help();
        population.survivalGroup();


        //Calculate stats
        if (generation % parameters->getSkip() == 0) {
            statistics->calculateStatistics(population);

            //Print last generation
            if (generation == 10000 ||
                generation == 25000 ||
                generation == parameters->getNumGenerations() / 2 ||
                generation == parameters->getNumGenerations()) {
                results->writeToCacheLastGeneration(this, population);
            }
        }

        //        if (replica == 0 && generation == 20000){
        //            std::cout << "HERE" << std::endl; //TODO: Stopper for debugging
        //        }

        population.mortalityGroup();

        // Print main file (separately since we need values of deaths, newBreederFloater, newBreederHelper and inheritance to be calculated)
        if (generation % parameters->getSkip() == 0) {
            statistics->printToConsole(generation, population.getDeaths(), population.getEmigrants());
            results->writeToCacheMain(
                statistics->generateMainCacheElement(generation, population.getDeaths(),
                                                     population.getNewBreederOutsider(),
                                                     population.getNewBreederInsider()));
        }


        population.increaseAge();
        population.reproduce(generation);
    }

    return results;
}

int Simulation::getGeneration() const {
    return generation;
}
