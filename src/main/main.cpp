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

#include <iostream>
#include <future>
#include "util/Parameters.h"
#include "stats/Statistics.h"
#include "util/FilePrinter.h"


/* MAIN PROGRAM */
int main(int count, char **argv) {

    //TODO will be done at the end
    Parameters *parameters;
    if (count > 1) {
        std::cout << "reading file " << argv[1] << "\n";
        parameters = new Parameters(argv[1], 0);
    } else {
        parameters = new Parameters(0);
    }

    std::vector<std::future<ResultCache *>> futures;

    for (int replica = 0; replica < parameters->getMaxNumReplicates(); replica++) {
        std::cout << "REPLICA = " << replica << std::endl;

        futures.emplace_back(std::async(std::launch::async, [parameters]() {
            auto *simulation = new Simulation(parameters->cloneWithIncrementedReplica());
            ResultCache *result = simulation->run();
            delete simulation;
            return result;
        }));
    }

    std::vector<ResultCache *> results;
    for (auto &future: futures) {
        results.emplace_back(future.get());
    }

    FilePrinter filePrinter(parameters);

    filePrinter.writeMainFile(results);
    filePrinter.writeLastGenerationFile(results);

    return 0;
}

