#ifndef GROUP_AUGMENTATION_INDIVIDUAL_H
#define GROUP_AUGMENTATION_INDIVIDUAL_H

#include <unordered_map>
#include "FishType.h"
#include "../Parameters.h"
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
    Parameters *parameters; ///< A pointer to the singleton instance of Parameters class.

    double id; ///< The unique identifier of the individual.
    double alpha; ///< Genetic parameter alpha.
    double alphaAge; ///< Genetic parameter alphaAge.
    double beta; ///< Genetic parameter beta.
    double gamma; ///< Genetic parameter gamma.
    double delta; ///< Genetic parameter delta.
    double drift; ///< Genetic parameter drift.

    double dispersal; ///< The dispersal rate of the individual.
    double help; ///< The help provided by the individual.
    double survival; ///< The survival rate of the individual.

    FishType fishType; ///< The type of the individual (breeder, helper, floater).
    int age; ///< The age of the individual.
    int ageBecomeBreeder; ///< The age at which the individual became a breeder.
    bool inherit; ///< Flag indicating if the individual inherited the territory or dispersed.
    double fecundity;
    int groupIndex; ///< The index of the group the individual belongs to.


    /**
     * @brief Mutates the individual.
     * @param generation The current generation of the simulation.
     */
    void mutate(int generation);

    /**
     * @brief Initializes the individual.
     * @param type The type of the individual.
     */
    void initializeIndividual(FishType type);

public:
    /**
     * @brief Getter for the group index of the individual.
     * @return The group index of the individual.
     */
    int getGroupIndex() const;

    /**
     * @brief Constructor for the Individual class.
     * @param individual The individual to copy.
     * @param fishType The type of the individual.
     * @param generation The current generation of the simulation.
     */
    Individual(Individual &individual, FishType fishType, int &generation);

    /**
     * compare function for the individual class
     * @param other
     * @return
     */
    bool operator==(const Individual &other) const;

    /**
     * @brief Constructor for the Individual class.
     * @param fishType The type of the individual.
     */
    explicit Individual(FishType fishType);

    double returnFecundity(int breedersSize, double cumHelp);

    /**
     * @brief Calculates the dispersal rate of the individual.
     */
    void calcDispersal();

    /**
     * @brief Calculates the help provided by the individual.
     */
    void calcHelp();

    /**
     * @brief Calculates the survival rate of the individual.
     * @param groupSize The size of the group the individual belongs to.
     */
    void calcSurvival(const int &groupSize, double delta);

    // Getters and setters
    double getAlpha() const;

    double getAlphaAge() const;

    double getBeta() const;

    double getGamma() const;

    double getDelta() const;

    double getDrift() const;

    double getDispersal() const;

    double getHelp() const;

    void setHelp(double help_);

    double getSurvival() const;

    FishType getFishType() const;

    void setFishType(FishType type);

    int getAge() const;

    void setAgeBecomeBreeder();

    void increaseAge(bool alive); //for the breeder
    void increaseAge(); // For helpers and floaters
    bool isInherit() const;

    void setInherit(bool inherit);

    double get(Attribute geneType) const;

    void setGroupIndex(int groupIndex);

    bool isViableBreeder();

    double getFecundity() const;
};


#endif //GROUP_AUGMENTATION_INDIVIDUAL_H