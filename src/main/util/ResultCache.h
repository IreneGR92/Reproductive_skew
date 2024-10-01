#ifndef REPRODUCTIVE_SKEW_RESULTCACHE_H
#define REPRODUCTIVE_SKEW_RESULTCACHE_H


#include <vector>
#include <string>
#include <queue>

#include "LastGenerationCacheElement.h"
#include "MainCacheElement.h"
#include "../model/Individual.h"
#include "../Simulation.h"

class Simulation; // Forward declaration of the Simulation class.

class ResultCache {
    Parameters *parameters; ///< The parameters of the simulation.

    ///< stores the text output before written to file.
    std::vector<LastGenerationCacheElement> lastGenerationCache;
    std::queue<MainCacheElement> mainCache;

public:
    explicit ResultCache(Parameters *parameters) : parameters(parameters) {
//        mainCache.reserve(parameters->getNumGenerations() / parameters->getSkip());
        // lastGenerationCache.reserve();
    };

    void clear();

    /**
 * @brief Prints the attributes of an individual to a file.
 * @param individual The individual to print.
 * @param generation The current generation of the simulation.
 * @param groupID The ID of the group the individual belongs to.
 * @param replica The current replica of the simulation.
 */
    void writeToCacheIndividual(Individual individual, int generation, int groupID);

    void writeToCacheMain(MainCacheElement element);

    const std::vector<LastGenerationCacheElement> &getLastGenerationCache() const;

    const std::queue<MainCacheElement> &getMainCache() const;

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
};


#endif //REPRODUCTIVE_SKEW_RESULTCACHE_H
