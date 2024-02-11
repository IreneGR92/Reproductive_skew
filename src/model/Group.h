#ifndef GROUP_AUGMENTATION_GROUP_H
#define GROUP_AUGMENTATION_GROUP_H

#include "Individual.h"
#include "../Parameters.h"
#include "container/IndividualVector.h"

/**
 * @class Group
 * @brief A class that represents a group in a population simulation model.
 *
 * This class maintains a list of Individual objects that are part of the group.
 * It provides methods to manipulate and access these individuals.
 */
class Group {

private:
    Parameters *parameters; ///< A pointer to the singleton instance of Parameters class.
    double cumHelp; ///< The cumulative help provided by the group.
    bool breederAlive; ///< A flag indicating if the main breeder is alive.
    int groupSize; ///< The size of the group.
    double fecundity; ///< The fecundity of the group.
    int realFecundity; ///< The real fecundity of the group.

    Individual mainBreeder; ///< The main breeder of the group.

    IndividualVector helpers; ///< A vector of Individual objects that are helpers in the group.
    IndividualVector breeders; ///< A vector of Individual objects that are breeders in the group.

public:
    /**
     * @brief Default constructor for the Group class.
     */
    explicit Group();

    /**
     * @brief Calculates the size of the group.
     */
    void calculateGroupSize();

    /**
     * @brief Handles the dispersal of individuals within the group.
     * @return A vector of Individual objects that are dispersed.
     */
    std::vector<Individual> disperse();

    /**
     * @brief Calculates the acceptance rate of the group.
     * @return The acceptance rate of the group.
     */
    double getAcceptanceRate();

    /**
     * @brief Calculates the cumulative help provided by the group.
     */
    void calculateCumulativeHelp();

    /**
     * @brief Getter for the helpers in the group.
     * @return A vector of Individual objects that are helpers in the group.
     */
    const IndividualVector &getHelpers() const;

    /**
     * @brief Handles the survival process for the group.
     */
    void survivalGroup();

    /**
     * @brief Handles the mortality process for the group.
     * @param deaths A reference to the number of deaths in the population.
     */
    void mortalityGroup(int &deaths);

    /**
     * @brief Handles the process of new breeders joining the group.
     * @param newBreederOutsider A reference to the number of new breeders that were outsiders.
     * @param newBreederInsider A reference to the number of new breeders that were insiders.
     * @param inheritance A reference to the number of inheritance events.
     */
    void newBreeder(int &newBreederOutsider, int &newBreederInsider, int &inheritance);

    /**
     * @brief Calculates the reproductive share of the group.
     * @return The reproductive share of the group.
     */
    double getReproductiveShare();

    /**
     * @brief Increases the age of individuals in the group.
     */
    void increaseAge();

    /**
     * @brief Handles the reproduction process within the group.
     * @param generation The current generation of the simulation.
     */
    void reproduce(int generation);

    /**
     * @brief Getter for the main breeder in the group.
     * @return The main breeder of the group.
     */
    const Individual &getBreeder() const;

    /**
     * @brief Getter for the size of the group.
     * @return The size of the group.
     */
    int getGroupSize() const;

    /**
     * @brief Checks if the main breeder is alive.
     * @return True if the main breeder is alive, false otherwise.
     */
    bool isBreederAlive() const;

    /**
     * @brief Getter for the cumulative help provided by the group.
     * @return The cumulative help provided by the group.
     */
    double getCumHelp() const;

    /**
     * @brief Checks if the group has helpers.
     * @return True if the group has helpers, false otherwise.
     */
    bool hasHelpers() const;

    /**
     * @brief Adds a helper to the group.
     * @param helper The helper to be added.
     */
    void addHelper(Individual &helper);

    /**
     * @brief Adds multiple helpers to the group.
     * @param helpers A vector of helpers to be added.
     */
    void addHelpers(std::vector<Individual> &helpers);

    /**
     * @brief Gets the attribute values of the individuals in the group.
     * @param attribute The attribute to get the values of.
     * @return A vector of attribute values.
     */
    std::vector<double> get(Attribute attribute) const;

    /**
     * @brief Gets the attribute values of the individuals in the group, including or excluding the main breeder.
     * @param attribute The attribute to get the values of.
     * @param includeBreeder A flag indicating whether to include the main breeder.
     * @return A vector of attribute values.
     */
    std::vector<double> get(Attribute attribute, bool includeBreeder) const;

    /**
     * @brief Reassigns the individuals in the group with no relatedness.
     * @param index The index of the individual to start the reassignment from.
     * @return A vector of reassigned individuals.
     */
    std::vector<Individual> reassignNoRelatedness(int index);

    /**
     * @brief Transfers the breeders to the helpers.
     */
    void transferBreedersToHelpers();

    /**
     * @brief Chooses a new breeder for the group.
     * @param newBreederOutsider A reference to the number of new breeders that were outsiders.
     * @param newBreederInsider A reference to the number of new breeders that were insiders.
     * @param inheritance A reference to the number of inheritance events.
     * @return The new breeder of the group.
     */
    Individual &chooseNewBreeder(int &newBreederOutsider, int &newBreederInsider, int &inheritance);
};


#endif //GROUP_AUGMENTATION_GROUP_H