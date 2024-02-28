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

    int emigrants = 0; ///< The number of floaters generated before immigration.

    double mk = 0; ///< variable environmental mortality of offspring.

public:

    const std::vector<Group> &getGroups() const;

    const IndividualVector &getFloaters() const;

    int getDeaths() const;

    int getEmigrants() const;

    void reset();

    Population();

    void disperse(int generation);

    void immigrate();

    void help();

    void survivalGroup();

    void survivalFloaters();

    void mortalityGroup();

    void mortalityFloaters();

    void reassingBreeder();

    void increaseAge();

    void increaseAgeFloaters();

    void reproduce(int generation);

    int getNewBreederOutsider() const;

    int getNewBreederInsider() const;

    int getInheritance() const;

    double getOffspringSurvival() const;

    double getMk() const;
};


#endif //GROUP_AUGMENTATION_DATAMODEL_H