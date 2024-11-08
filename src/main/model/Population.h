#ifndef GROUP_AUGMENTATION_DATAMODEL_H
#define GROUP_AUGMENTATION_DATAMODEL_H

#include <memory>
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

    std::shared_ptr<Parameters> parameters;

    std::vector<Group> groups; ///< A vector of Group objects.

    IndividualVector floaters; ///< A vector of Individual objects that are not part of any group.

    int deaths, groupColonization; ///< The number of deaths in the population.

    int newBreederOutsider, newBreederInsider, inheritance; ///< Statistics related to new breeders and inheritance.

    int emigrants; ///< The number of floaters generated before immigration.

    double mk; ///< variable environmental mortality of offspring.

    // variables to change the predictability of the environment
    int conditionCheckCounter = 0;
    int changeCounter = 0;
    bool negativeModification = false;

    void reassignNoRelatedHelpers();

    void increaseAgeFloaters();


public:
    explicit Population(const std::shared_ptr<Parameters>& parameters);

    void reset();

    void disperse();

    void immigrate();

    void help();

    void survivalGroup();

    void survivalFloaters();

    void mortalityGroup();

    void mortalityFloaters();

    void reassignBreeder();

    void increaseAge();

    void reproduce(int generation);


    // Getters and setters

    const std::vector<Group> &getGroups() const;

    const IndividualVector &getFloaters() const;

    int getDeaths() const;

    int getEmigrants() const;

    int getNewBreederOutsider() const;

    int getNewBreederInsider() const;

    int getInheritance() const;

    double getOffspringSurvival();

    double getMk() const;

    int getGroupColonization() const;


};


#endif //GROUP_AUGMENTATION_DATAMODEL_H