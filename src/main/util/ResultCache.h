#ifndef REPRODUCTIVE_SKEW_RESULTCACHE_H
#define REPRODUCTIVE_SKEW_RESULTCACHE_H


#include <string>
#include <queue>

#include "LastGenerationCacheElement.h"
#include "MainCacheElement.h"
#include "../model/Individual.h"
#include "../Simulation.h"

class Simulation; // Forward declaration of the Simulation class.

class ResultCache {
    const int replica;    ///< The replica number for this simulation.

    std::shared_ptr<Parameters> parameters; ///< The parameters of the simulation.

    ///< stores the text output before written to file.
    std::queue<LastGenerationCacheElement> lastGenerationCache;
    std::queue<MainCacheElement> mainCache;

public:
    explicit ResultCache(const std::shared_ptr<Parameters>& parameters, int replica) : parameters(parameters),
                                                                                       replica(replica) {
    }


    /**
 * @brief Prints the attributes of an individual to a file.
 * @param individual The individual to print.
 * @param generation The current generation of the simulation.
 * @param groupID The ID of the group the individual belongs to.
 * @param replica The current replica of the simulation.
 */
    void writeToCacheIndividual(Individual individual, int generation, int groupID);

    void writeToCacheMain(MainCacheElement element);

    [[nodiscard]] const std::queue<LastGenerationCacheElement> &getLastGenerationCache() const;

    [[nodiscard]] const std::queue<MainCacheElement> &getMainCache() const;

    /**
 * @brief Prints the statistics to a file.
 * @param replica The current replica of the simulation.
 * @param generation The current generation of the simulation.
 * @param deaths The number of deaths in the population.
 * @param newBreederOutsider The number of new breeders that were floaters.
 * @param newBreederInsider The number of new breeders that were helpers.
 * @param inheritance The number of inheritance events.
 */
    void writeToCacheMain(int generation, const Population &population, const Statistics &statistics);

    /**
     * @brief Prints the statistics of the last generation to a file.
     * @param simulation The simulation to print statistics for.
     * @param populationObj The population to print statistics for.
     */
    void writeToCacheLastGeneration(Simulation *simulation, const Population &populationObj);

    const int getReplica() const;
};


#endif //REPRODUCTIVE_SKEW_RESULTCACHE_H
