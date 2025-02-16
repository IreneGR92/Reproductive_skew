#ifndef GROUP_AUGMENTATION_GROUP_H
#define GROUP_AUGMENTATION_GROUP_H

#include <memory>
#include "Individual.h"
#include "../util/Parameters.h"
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
    std::shared_ptr<Parameters> parameters;
    double cumHelp; ///< The cumulative help provided by the group.
    bool mainBreederAlive; ///< A flag indicating if the main breeder is alive.
    int groupSize; ///< The size of the group.
    double acceptanceRate; ///< The acceptance rate of immigrants to the group.
    int acceptedFloatersSize; ///< Number of immigrants accepted by the group.
    double reproductiveShareRate; ///< Rate of how many helpers are allowed to breeder in the group.
    int fecundityGroup; ///< The max number of offspring produced by the group.
    int offspringMainBreeder;
    int offspringSubordinateBreeders;
    int totalOffspringGroup;
    bool hasPotentialImmigrants;


    Individual mainBreeder; ///< The main breeder of the group.
    IndividualVector subordinateBreeders; ///< A vector of Individual objects that are breeders in the group.
    IndividualVector helpers; ///< A vector of Individual objects that are helpers in the group.



    Individual *selectBreeder(int &newBreederOutsider, int &newBreederInsider, int &inheritance);

    void mortalityGroupVector(int &deaths, IndividualVector &individuals);

    int countHelpersAgeOne();

    int calculateHelpersToReassign();

    void calcAcceptanceRate();

    void calcReproductiveShareRate();



public:
    int calcFecundity(double mk);
    explicit Group(const std::shared_ptr<Parameters>& parameters);

    void calculateGroupSize();

    std::vector<Individual> disperse();

    std::vector<Individual> noRelatedHelpersToReassign(int index);

    int getAcceptedFloatersSize(int numSampledFloaters);

    void transferBreedersToHelpers();

    void calculateCumulativeHelp();

    void survivalGroup();

    void mortalityGroup(int &deaths);

    void reassignBreeders(int &newBreederOutsider, int &newBreederInsider, int &inheritance);

    void increaseAge();

    void reproduce(int generation, double mk);


    // Getters and setters
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


    bool isGroupEmpty();
};


#endif //GROUP_AUGMENTATION_GROUP_H