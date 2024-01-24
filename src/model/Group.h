
#ifndef GROUP_AUGMENTATION_GROUP_H
#define GROUP_AUGMENTATION_GROUP_H


#include "Individual.h"
#include "../Parameters.h"
#include "container/IndividualVector.h"


class Group {

private:
    Parameters *parameters;
    double cumHelp;
    bool breederAlive;      // for the mainBreeder: 1 alive, 0 dead
    int groupSize;
    double fecundity;
    int realFecundity;

    Individual mainBreeder;

    IndividualVector helpers;
    IndividualVector breeders;

public:

    explicit Group();

    void calculateGroupSize();

    std::vector<Individual> disperse();

    double getAcceptanceRate();

    void calculateCumulativeHelp();

    const IndividualVector &getHelpers() const;

    void survivalGroup();

    void mortalityGroup(int &deaths);

    void newBreeder(std::vector<Individual> &floaters, int &newBreederOutsider, int &newBreederInsider, int &inheritance);

    double getReproductiveShare();

    void increaseAge();

    void reproduce(int generation);

    const Individual &getBreeder() const;

    int getGroupSize() const;

    bool isBreederAlive() const;

    double getCumHelp() const;

    bool hasHelpers() const;

    void addHelper(Individual &helper);

    void addHelpers(std::vector<Individual> &helpers);

    std::vector<double> get(Attribute attribute) const;

    std::vector<double> get(Attribute attribute, bool includeBreeder) const;

    std::vector<Individual> reassignNoRelatedness(int index);

    void transferBreedersToHelpers();

    Individual& chooseNewBreeder(int &newBreederOutsider, int &newBreederInsider, int &inheritance);
};


#endif //GROUP_AUGMENTATION_GROUP_H
