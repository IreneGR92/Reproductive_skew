
#include "Simulation.h"
#include "stats/Statistics.h"
#include <vector>


void Simulation::run() {

    // Output file
    auto *statistics = new Statistics(this->parameters);

    statistics->calculateStatistics(population);
    statistics->printHeadersToConsole();
    statistics->printToConsole(generation, population.getDeaths(), population.getEmigrants());
//    statistics->printToFile(replica, generation, population.getDeaths(), newBreederOutsider, newBreederInsider, inheritance);

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

                statistics->printToFileLastGeneration(this, population);
            }
        }

//        if (replica == 0 && generation == 20000){
//            std::cout << "HERE" << std::endl; //TODO: Stopper for debugging
//        }

        population.mortalityGroup();

        // Print main file (separately since we need values of deaths, newBreederFloater, newBreederHelper and inheritance to be calculated)
        if (generation % parameters->getSkip() == 0) {
            statistics->printToConsole(generation, population.getDeaths(), population.getEmigrants());
            statistics->printToFile(parameters->getReplica(), generation, population.getDeaths(),
                                    population.getNewBreederOutsider(),
                                    population.getNewBreederInsider(), population.getInheritance());
        }

        delete statistics;

        population.increaseAge();
        population.reproduce(generation);
    }
}

int Simulation::getGeneration() const {
    return generation;
}










