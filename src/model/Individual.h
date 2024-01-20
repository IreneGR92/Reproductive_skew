
#ifndef GROUP_AUGMENTATION_INDIVIDUAL_H
#define GROUP_AUGMENTATION_INDIVIDUAL_H


#include <unordered_map>
#include "IndRole.h"
#include "../Parameters.h"
#include "Attribute.h"

class Individual {

    Parameters *parameters;

    double alpha;
    double alphaAge;
    double beta;
    double gamma;
    double gammaAge;
    double drift;

    double dispersal;
    double help;
    double survival;

    IndRole fishType;                                                // possible classes: breeder, helper, floater
    int age;
    int ageBecomeBreeder;
    bool inherit;                                                    //did the new breeder inherit the territory or did it disperse?

    void mutate(int generation);

    void initializeIndividual(IndRole type);

    int groupIndex;
public:
    int getGroupIndex() const;

public:
    Individual(Individual &individual, IndRole fishType, int &generation);

    explicit Individual(IndRole indRole);


    //Functions inside Individual
    void calcDispersal();

    void calcHelp();

    void calculateSurvival(const int &groupSize);

    //Getters and setters
    double getAlpha() const;

    double getAlphaAge() const;

    double getBeta() const;

    double getGamma() const;

    double getGammaAge() const;

    double getDrift() const;

    double getDispersal() const;

    double getHelp() const;

    void setHelp(double help_);

    double getSurvival() const;

    IndRole getIndRole() const;

    void setIndRole(IndRole type);

    int getAge() const;
    void setAgeBecomeBreeder(int ageBecomeBreeder_);

    void increaseAge(bool alive); //for the breeder
    void increaseAge(); // For helpers and floaters

    bool isInherit() const;

    void setInherit(bool inherit);

    //allows to iterate trough members
    double get(Attribute geneType) const;

    void setGroupIndex(int groupIndex);

    bool isViableBreeder();
};


#endif //GROUP_AUGMENTATION_INDIVIDUAL_H
