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
    bool mainBreederAlive; ///< A flag indicating if the main breeder is alive.
    int groupSize; ///< The size of the group.
    double acceptanceRate; ///< The acceptance rate of immigrants to the group.
    int acceptedFloatersSize; ///< Number of immigrants accepted by the group.
    double reproductiveShareRate; ///< Rate of how many helpers are allowed to breeder in the group.
    double fecundityGroup; ///< The max number of offspring produced by the group.
    int offspringMainBreeder;
    int offspringSubordinateBreeders;
    int totalOffspringGroup;


    Individual mainBreeder; ///< The main breeder of the group.

    IndividualVector helpers; ///< A vector of Individual objects that are helpers in the group.
    IndividualVector breeders; ///< A vector of Individual objects that are breeders in the group.

    Individual *selectBreeder(int &newBreederOutsider, int &newBreederInsider, int &inheritance);

    void mortalityGroupVector(int &deaths, IndividualVector &individuals);

public:

    explicit Group();

    void calculateGroupSize();

    std::vector<Individual> disperse();

    std::vector<Individual> reassignNoRelatedness(int index);

    void calcAcceptanceRate();

    std::vector<Individual> getAcceptedFloaters(IndividualVector &floaters);

    void calculateCumulativeHelp();

    void survivalGroup();

    void mortalityGroup(int &deaths);

    void reassignBreeders(int &newBreederOutsider, int &newBreederInsider, int &inheritance);

    void transferBreedersToHelpers();

    void calcReproductiveShareRate();

    void increaseAge();

    double returnFecundity();

    void reproduce(int generation);


    const Individual &getMainBreeder() const;

    bool isBreederAlive() const;

    int getGroupSize() const;

    int getBreedersSize() const;

    double getCumHelp() const;

    double getAcceptanceRate() const;

    double getReproductiveShareRate() const;

    double getFecundityGroup() const;

    int getOffspringMainBreeder() const;

    int getOffspringSubordinateBreeders() const;

    int getTotalOffspringGroup() const;

    bool hasHelpers() const;

    bool hasSubordinateBreeders() const;

    void addHelper(Individual &helper);

    void addHelpers(std::vector<Individual> &helpers);

    const IndividualVector &getHelpers() const;

    const IndividualVector &getSubordinateBreeders() const;

    std::vector<double> get(Attribute attribute) const;

    std::vector<double> get(Attribute attribute, bool includeBreeder) const;




};


#endif //GROUP_AUGMENTATION_GROUP_H