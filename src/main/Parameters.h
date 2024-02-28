#ifndef CPP_PARAMETERS_H
#define CPP_PARAMETERS_H

#include <iomanip>
#include <string>
#include <fstream>
#include <random>

/**
 * @class Parameters
 * @brief A singleton class that holds the parameters for a population simulation model.
 *
 * This class maintains various parameters related to the simulation such as the number of generations, mutation rates, and other genetic parameters.
 * It provides methods to access these parameters.
 */
class Parameters {

public:
    /**
     * @brief Default constructor for the Parameters class.
     */
    Parameters();

    /**
     * @brief Destructor for the Parameters class.
     */
    virtual ~Parameters();

    /**
     * @brief Constructor for the Parameters class that takes a URL.
     * @param url The URL to load parameters from.
     */
    explicit Parameters(const std::string &url);

private:

    std::string name; ///< The name of the simulation.

    // Run parameters
    bool REACTION_NORM_HELP;    ///< Apply reaction norm to age for dispersal?
    bool EVOLUTION_HELP_AFTER_DISPERSAL; ///< Help evolves only after the evolution of dispersal?
    bool NO_GROUP_AUGMENTATION; ///< No group augmentation in the simulation?
    bool NO_RELATEDNESS;       ///< Apply implementation to remove the effect of relatedness?
    bool AGE_NO_INFLUENCE_INHERITANCE; ///< Age has no influence on who inherits territory, randomly sampled from candidate (helpers + sample floaters)

    int MAX_COLONIES;     ///< Maximum number of groups or colonies --> breeding spots.
    int NUM_GENERATIONS;  ///< Number of generations in the simulation.
    int MAX_NUM_REPLICATES; ///< Maximum number of replicates in the simulation.
    int SKIP;   ///< Interval between print-outs

    // Fixed values
    int INIT_NUM_HELPERS;     ///< Initial number of helpers per group.
    int BIAS_FLOAT_BREEDER; ///< Floaters sampled to try to join a group.
    int MIN_AGE_BECOME_BREEDER; ///< Minimum age for individuals to be able to become breeders.
    double FIXED_GROUP_SIZE;       ///< In the implementation of no group augmentation, virtual group size for survival for breeder and helpers.

    // Modifiers in survival.
    double m;      ///< Base mortality.
    double n;      ///< Mortality linked to dispersal.
    double X0;     ///< Base survival without the effect of help or group size.
    double Xh;    ///< Cost of help in survival.
    double Xn;    ///< Benefit of group size in survival.
    double Xe;    ///< Cost of expulsion in survival.
    double Xrs;   ///< Cost of reproductive suppression in survival.

    // Modifiers in offspring mortality
    double mOff;     ///< mortality of offspring
    double mRate;    ///< rate of change in mortality of offspring
    double mStep;    ///< step size in mortality of offspring

    // Modifiers in fecundity
    double K0;    ///< Minimum fecundity, fecundity when no help provided.
    double Kh;    ///< Benefit of cumulative help in the fecundity.
    double Knb;   ///<  cost of number of breeders in the fecundity

    // Genetic values

    // For help
    double INIT_ALPHA;            ///< Initial value for genetic parameter alpha.
    double INIT_ALPHA_AGE;        ///< Initial value for genetic parameter alphaAge.

    double MUTATION_ALPHA;        ///< Mutation rate in alpha for level of help.
    double MUTATION_ALPHA_AGE;    ///< Mutation rate in alphaAge for level of help.
    double STEP_ALPHA;            ///< Mutation step size in alpha for level of help.

    // For dispersal
    double INIT_BETA;             ///< Initial value for genetic parameter beta.

    double MUTATION_BETA;         ///< Mutation rate for the propensity to disperse.
    double STEP_BETA;             ///< Mutation step size for the propensity to disperse.

    // For expulsion of immigrants
    double INIT_GAMMA;            ///< Initial value for genetic parameter gamma.
    double MUTATION_GAMMA;        ///< Mutation rate for the propensity to expel immigrants.
    double STEP_GAMMA;            ///< Mutation step size for the propensity to expel immigrants.

    // For reproductive concessions
    double INIT_DELTA;            ///< Initial value for genetic parameter delta.
    double MUTATION_DELTA;        ///< Mutation rate for the propensity for reproductive concessions.
    double STEP_DELTA;            ///< Mutation step size for the propensity for reproductive concessions.

    // For relatedness
    double MUTATION_DRIFT;        ///< Mutation rate in the neutral selected value to track level of relatedness.
    double STEP_DRIFT;            ///< Mutation step size in the neutral genetic value to track level of relatedness.

    std::default_random_engine *generator; ///< A pointer to the random number generator.

    std::string getName(std::string url); ///< Helper function to get the name of the simulation from a URL.

    void print(std::ofstream &outputStream); ///< Helper function to print the parameters to an output stream.

    std::ofstream *mainWriter; ///< A pointer to the main output stream writer.
    std::ofstream *lastGenerationWriter; ///< A pointer to the last generation output stream writer.

    double idCounter = 0;
public:

    std::uniform_real_distribution<double> driftUniform; ///< A uniform real distribution for drift.
    std::uniform_real_distribution<double> uniform; ///< A uniform real distribution.

    /**
     * @brief Prints the parameters to the console.
     */
    void print();

    // Getters for the parameters
    const std::string &getName() const;

    bool isReactionNormHelp() const;

    bool isEvolutionHelpAfterDispersal() const;

    bool isNoGroupAugmentation() const;

    bool isNoRelatedness() const;

    bool isAgeNoInfluenceInheritance() const;

    int getMaxColonies() const;

    int getNumGenerations() const;

    int getMaxNumReplicates() const;

    int getSkip() const;

    int getInitNumHelpers() const;

    int getBiasFloatBreeder() const;

    double getFixedGroupSize() const;

    int getMinAgeBecomeBreeder() const;

    double getM() const;

    double getN() const;

    double getX0() const;

    double getXh() const;

    double getXn() const;

    double getXe() const;

    double getXrs() const;

    double getMOff() const;

    double getMRate() const;

    double getMStep() const;

    double getK0() const;

    double getKh() const;

    double getKnb() const;

    double getInitAlpha() const;

    double getInitAlphaAge() const;

    double getMutationAlpha() const;

    double getMutationAlphaAge() const;

    double getStepAlpha() const;

    double getInitBeta() const;

    double getMutationBeta() const;

    double getStepBeta() const;

    double getInitGamma() const;

    double getMutationGamma() const;

    double getStepGamma() const;

    double getInitDelta() const;

    double getMutationDelta() const;

    double getStepDelta() const;

    double getMutationDrift() const;

    double getStepDrift() const;

    static const int NO_VALUE = -1; ///< A constant representing no value.

    std::ofstream *getMainWriter() const;

    std::ofstream *getLastGenerationWriter() const;

    std::default_random_engine *getGenerator() const;

    double nextId() {
        return idCounter++;
    }

    /**
     * @brief Returns the singleton instance of the Parameters class.
     * @return The singleton instance of the Parameters class.
     */
    static Parameters *instance();

    static Parameters *unitTestInit();
};


#endif //CPP_PARAMETERS_H