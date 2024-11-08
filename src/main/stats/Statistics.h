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
    int population{}, totalFloaters{}, totalHelpers{}, totalMainBreeders{}, totalSubordinateBreeders{}, emptyGroupsCount{}; // Counters
    double relatednessHelpers{}, relatednessBreeders{};
    double mk{}; // variable environmental survival of offspring.
    double groupExtinction{}, groupColonizationRate{};

    // StatisticalFormulas objects for various statistics
    StatisticalFormulas groupSize, numOfSubBreeders;
    StatisticalFormulas age, ageDomBreeders, ageSubBreeders, ageFloaters, ageHelpers, ageBecomeBreeder; //age
    StatisticalFormulas alpha, beta, gamma, delta; //genetic parameters
    StatisticalFormulas help, cumulativeHelp;
    StatisticalFormulas dispersal, acceptanceRate;
    StatisticalFormulas survival, survivalDomBreeders, survivalSubBreeders, survivalFloaters, survivalHelpers;
    StatisticalFormulas fecundityGroup, reproductiveShareRate, offspringMainBreeder, offspringOfSubordinateBreeders, totalOffspringGroup;
    StatisticalFormulas driftB, driftH, driftBB, driftHB, relatedness; // Relatedness


public:

    explicit Statistics(std::shared_ptr<Parameters> parameters) : parameters(parameters) {};

    void calculateStatistics(const Population &populationObj);

    static void printHeadersToConsole();

    void printToConsole(int generation, int deaths, int emigrants);

    MainCacheElement generateMainCacheElement(int generation, int deaths, int newBreederOutsider,
                                              int newBreederInsider);
};


#endif //GROUP_AUGMENTATION_STATISTICS_H