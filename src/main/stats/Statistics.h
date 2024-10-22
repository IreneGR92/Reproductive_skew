#ifndef GROUP_AUGMENTATION_STATISTICS_H
#define GROUP_AUGMENTATION_STATISTICS_H

#include "../model/Group.h"
#include "../Simulation.h"
#include "StatisticalFormulas.h"
#include "../util/MainCacheElement.h"

/**
 * @class Statistics
 * @brief A class that calculates and maintains various statistics related to a population simulation model.
 *
 * This class maintains various statistics related to the population such as the number of individuals, relatedness, age, genetic parameters, and others.
 * It provides methods to calculate these statistics and print them to the console or a file.
 */
class Statistics {


private:
    std::shared_ptr<Parameters> parameters; ///< The parameters of the simulation.


    // Population parameters and Statistics
    int population{}, totalFloaters{}, totalHelpers{}, totalMainBreeders{}, totalSubordinateBreeders{}; // Counters

    double relatednessHelpers{}, relatednessBreeders{}; ///< The relatedness in the population.
    double mk{}; // variable environmental mortality of offspring.

    // StatisticalFormulas objects for various statistics
    StatisticalFormulas groupSize, numOfSubBreeders;
    StatisticalFormulas age, ageDomBreeders, ageSubBreeders, ageFloaters, ageHelpers, ageBecomeBreeder; //age
    StatisticalFormulas alpha, beta, gamma, delta; //genetic parameters
    StatisticalFormulas help, cumulativeHelp;
    StatisticalFormulas dispersal, acceptanceRate;
    StatisticalFormulas survival, survivalDomBreeders, survivalSubBreeders, survivalFloaters, survivalHelpers;
    StatisticalFormulas fecundityGroup, reproductiveShareRate, offspringMainBreeder, offspringOfSubordinateBreeders, totalOffspringGroup;
    StatisticalFormulas driftB, driftH, driftBB, driftHB; // Relatedness


public:
    /**
     * @brief Default constructor for the Statistics class.
     */
    explicit Statistics(std::shared_ptr<Parameters> parameters) : parameters(parameters) {};

    /**
     * @brief Calculates various statistics for the population.
     * @param populationObj The population to calculate statistics for.
     */
    void calculateStatistics(const Population &populationObj);

    /**
     * @brief Calculates the relatedness in the population.
     * @param groups The groups in the population.
     * @return The relatedness in the population.
     */
    double calculateRelatednessHelpers(const std::vector<Group> &groups);

    double calculateRelatednessBreeders(const std::vector<Group> &groups);


    /**
     * @brief Prints the headers for the statistics to the console.
     */
    static void printHeadersToConsole();


    /**
     * @brief Prints the statistics to the console.
     * @param generation The current generation of the simulation.
     * @param deaths The number of deaths in the population.
     */
    void printToConsole(int generation, int deaths, int emigrants);

    MainCacheElement generateMainCacheElement(int generation, int deaths, int newBreederOutsider,
                                              int newBreederInsider);
};


#endif //GROUP_AUGMENTATION_STATISTICS_H