#ifndef GROUP_AUGMENTATION_DATAMODEL_H
#define GROUP_AUGMENTATION_DATAMODEL_H

#include "container/IndividualVector.h"
#include "Individual.h"
#include "Group.h"

/**
 * @class Population
 * @brief A class that represents a population in a simulation model.
 *
 * This class maintains a list of Group objects and Individual objects (floaters).
 * It provides methods to manipulate and access these groups and individuals.
 */
class Population {

private:
    Parameters *parameters = Parameters::instance(); ///< A pointer to the singleton instance of Parameters class.

    std::vector<Group> groups; ///< A vector of Group objects.

    IndividualVector floaters; ///< A vector of Individual objects that are not part of any group.

    int deaths = 0; ///< The number of deaths in the population.

    int newBreederOutsider = 0, newBreederInsider = 0, inheritance = 0; ///< Statistics related to new breeders and inheritance.

    /**
     * @brief Returns a list of floaters that are accepted by a given group.
     * @param group The group to check for accepted floaters.
     * @return A vector of Individual objects that are accepted by the group.
     */
    std::vector<Individual> getAcceptedFloaters(Group &group);

public:
    /**
     * @brief Getter for the number of new breeders that were outsiders.
     * @return The number of new breeders that were outsiders.
     */
    int getNewBreederOutsider() const;

    /**
     * @brief Getter for the number of new breeders that were insiders.
     * @return The number of new breeders that were insiders.
     */
    int getNewBreederInsider() const;

    /**
     * @brief Getter for the number of inheritance events.
     * @return The number of inheritance events.
     */
    int getInheritance() const;

public:
    /**
     * @brief Getter for the groups in the population.
     * @return A vector of Group objects in the population.
     */
    const std::vector<Group> &getGroups() const;

    /**
     * @brief Getter for the floaters in the population.
     * @return A vector of Individual objects that are floaters in the population.
     */
    const IndividualVector &getFloaters() const;

    /**
     * @brief Default constructor for the Population class.
     */
    Population();

    /**
     * @brief Handles the dispersal of individuals within the population.
     * @param generation The current generation of the simulation.
     */
    void disperse(int generation);

    /**
     * @brief Calculates the cumulative help for each group.
     */
    void help();

    /**
     * @brief Handles the survival process for groups.
     */
    void survival();

    /**
     * @brief Handles the mortality process for groups.
     */
    void mortality();

    /**
     * @brief Handles the process of new breeders joining groups.
     */
    void newBreeder();

    /**
     * @brief Increases the age of individuals in groups.
     */
    void increaseAge();

    /**
     * @brief Increases the age of floaters.
     */
    void increaseAgeFloaters();

    /**
     * @brief Handles the reproduction process within groups.
     * @param generation The current generation of the simulation.
     */
    void reproduce(int generation);

    /**
     * @brief Handles the survival process for floaters.
     */
    void survivalFloaters();

    /**
     * @brief Getter for the number of deaths in the population.
     * @return The number of deaths in the population.
     */
    int getDeaths() const;

    /**
     * @brief Handles the mortality process for floaters.
     */
    void mortalityFloaters();

    /**
     * @brief Resets the statistics for the population.
     */
    void reset();

    /**
     * @brief Handles the immigration of individuals to different groups.
     */
    void immigrate();

public:
};


#endif //GROUP_AUGMENTATION_DATAMODEL_H