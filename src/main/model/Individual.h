#ifndef GROUP_AUGMENTATION_INDIVIDUAL_H
#define GROUP_AUGMENTATION_INDIVIDUAL_H

#include <unordered_map>
#include <memory>
#include "RoleType.h"
#include "../util/Parameters.h"
#include "Attribute.h"

/**
 * @class Individual
 * @brief A class that represents an individual in a population simulation model.
 *
 * This class maintains various attributes of an individual such as its type (breeder, helper, floater), age, and other genetic parameters.
 * It provides methods to manipulate and access these attributes.
 */
class Individual {

private:
    std::shared_ptr<Parameters> parameters;
    double id; ///< The unique identifier of the individual.
    double alpha; ///< Genetic parameter alpha.
    double beta; ///< Genetic parameter beta.
    double gamma; ///< Genetic parameter gamma.
    double delta; ///< Genetic parameter delta.
    double drift; ///< Genetic parameter drift.

    double dispersal; ///< The dispersal rate of the individual.
    double help; ///< The help provided by the individual.
    double survival; ///< The survival rate of the individual.

    RoleType roleType; ///< The type of the individual (breeder, helper, floater).
    int age; ///< The age of the individual.
    int ageBecomeBreeder; ///< The age at which the individual became a breeder.
    bool inherit; ///< Flag indicating if the individual inherited the territory or dispersed.
    int groupIndex; ///< The index of the group the individual belongs to.



    void mutate(int generation);

    void initializeIndividual(RoleType type);

public:

    Individual(RoleType roleType, const std::shared_ptr<Parameters> &parameters);

    Individual(Individual &individual, RoleType roleType, int &generation);

    bool operator==(const Individual &other) const;

    int getGroupIndex() const;

    void calcDispersal();

    void calcHelp();

    void calcSurvival(const int &groupSize, double delta, const bool &hasPotentialImmigrants);

    // Getters and setters
    double getAlpha() const;

    double getBeta() const;

    double getGamma() const;

    double getDelta() const;

    double getDrift() const;

    double getDispersal() const;

    double getHelp() const;

    double getSurvival() const;

    RoleType getRoleType() const;

    void setRoleType(RoleType type);

    int getAge() const;

    void setAgeBecomeBreeder();

    void increaseAge(bool alive); //for the breeder
    void increaseAge(); // For helpers and floaters
    bool isInherit() const;

    void setInherit(bool inherit);

    double get(Attribute geneType) const;

    void setGroupIndex(int groupIndex);

    bool isViableBreeder();

};


#endif //GROUP_AUGMENTATION_INDIVIDUAL_H