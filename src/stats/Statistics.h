#ifndef GROUP_AUGMENTATION_STATISTICS_H
#define GROUP_AUGMENTATION_STATISTICS_H

#include "../model/Group.h"
#include "../model/Individual.h"
#include "../Simulation.h"
#include "StatisticalFormulas.h"
#include "../model/container/IndividualVector.h"

/**
 * @class Statistics
 * @brief A class that calculates and maintains various statistics related to a population simulation model.
 *
 * This class maintains various statistics related to the population such as the number of individuals, relatedness, age, genetic parameters, and others.
 * It provides methods to calculate these statistics and print them to the console or a file.
 */
class Statistics {

private:
    Parameters *parameters; ///< A pointer to the singleton instance of Parameters class.

    // Population parameters and Statistics
    int population, totalFloaters, totalHelpers, totalMainBreeders, totalSubordinateBreeders; // Counters

    double relatedness; ///< The relatedness in the population.

    // StatisticalFormulas objects for various statistics
    StatisticalFormulas groupSize, numOfSubBreeders;
    StatisticalFormulas age, ageBreeders, ageFloaters, ageHelpers, ageBecomeBreeder; //age
    StatisticalFormulas alpha, alphaAge, beta, gamma, delta; //genetic parameters
    StatisticalFormulas help, cumulativeHelp;
    StatisticalFormulas dispersal, acceptanceRate;
    StatisticalFormulas survival, survivalBreeders, survivalFloaters, survivalHelpers;
    StatisticalFormulas fecundity, reproductiveShareRate, offspringMainBreeder, offspringOfSubordinateBreeders, totalOffspringGroup;
    StatisticalFormulas driftB, driftH, driftBB, driftHB; // Relatedness

    /**
     * @brief Prints the attributes of an individual to a file.
     * @param individual The individual to print.
     * @param generation The current generation of the simulation.
     * @param groupID The ID of the group the individual belongs to.
     * @param replica The current replica of the simulation.
     */
    void printIndividual(Individual individual, int generation, int groupID, int replica);

public:
    /**
     * @brief Default constructor for the Statistics class.
     */
    Statistics();

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
    double calculateRelatedness(const std::vector<Group> &groups);

    /**
     * @brief Prints the headers for the statistics to the console.
     */
    void printHeadersToConsole();

    /**
     * @brief Prints the headers for the statistics to a file.
     */
    void printHeadersToFile();

    /**
     * @brief Prints the statistics to the console.
     * @param generation The current generation of the simulation.
     * @param deaths The number of deaths in the population.
     */
    void printToConsole(int generation, int deaths);

    /**
     * @brief Prints the statistics to a file.
     * @param replica The current replica of the simulation.
     * @param generation The current generation of the simulation.
     * @param deaths The number of deaths in the population.
     * @param newBreederFloater The number of new breeders that were floaters.
     * @param newBreederHelper The number of new breeders that were helpers.
     * @param inheritance The number of inheritance events.
     */
    void printToFile(int replica, int generation, int deaths, int newBreederFloater,
                     int newBreederHelper, int inheritance);

    /**
     * @brief Prints the statistics of the last generation to a file.
     * @param simulation The simulation to print statistics for.
     * @param populationObj The population to print statistics for.
     */
    void printToFileLastGeneration(Simulation *simulation, const Population &populationObj);

};


#endif //GROUP_AUGMENTATION_STATISTICS_H