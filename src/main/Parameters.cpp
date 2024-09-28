#include <iostream>
#include <algorithm>
#include "yaml-cpp/yaml.h"
#include "Parameters.h"

using namespace std;


static Parameters *singletonInstance;

//default value if not provided
Parameters::Parameters() : Parameters("../parameters/default.yml") {}

Parameters::Parameters(const string &url) {
    singletonInstance = this;

    YAML::Node config = YAML::LoadFile(url);

    this->name = this->getName(url);
    this->BET_HEDGING_HELP = config["BET_HEDGING_HELP"].as<bool>();
    this->HELP_OBLIGATORY = config["HELP_OBLIGATORY"].as<bool>();
    this->PREDICTABLE_ENVIRONMENT = config["PREDICTABLE_ENVIRONMENT"].as<bool>();
    this->EVOLUTION_HELP_AFTER_DISPERSAL = config["EVOLUTION_HELP_AFTER_DISPERSAL"].as<bool>();
    this->NO_GROUP_AUGMENTATION = config["NO_GROUP_AUGMENTATION"].as<bool>();
    this->NO_RELATEDNESS = config["NO_RELATEDNESS"].as<bool>();
    this->AGE_NO_INFLUENCE_INHERITANCE = config["AGE_NO_INFLUENCE_INHERITANCE"].as<bool>();
    this->MAX_COLONIES = config["MAX_COLONIES"].as<int>();
    this->NUM_GENERATIONS = config["NUM_GENERATIONS"].as<int>();
    this->MAX_NUM_REPLICATES = config["MAX_NUM_REPLICATES"].as<int>();
    this->SKIP = config["SKIP"].as<int>();
    this->INIT_NUM_HELPERS = config["INIT_NUM_HELPERS"].as<int>();
    this->FLOATERS_SAMPLED_IMMIGRATION = config["FLOATERS_SAMPLED_IMMIGRATION"].as<int>();
    this->MIN_AGE_BECOME_BREEDER = config["MIN_AGE_BECOME_BREEDER"].as<int>();
    this->FIXED_GROUP_SIZE = config["FIXED_GROUP_SIZE"].as<double>();
    this->REDUCED_RELATEDNESS = config["REDUCED_RELATEDNESS"].as<int>();
    this->m = config["m"].as<double>();
    this->X0 = config["X0"].as<double>();
    this->Xh = config["Xh"].as<double>();
    this->Xn = config["Xn"].as<double>();
    this->Xe = config["Xe"].as<double>();
    this->Xrs = config["Xrs"].as<double>();
    this->mOff = config["mOff"].as<double>();
    this->mRate = config["mRate"].as<double>();
    this->mStep = config["mStep"].as<double>();
    this->K0 = config["K0"].as<double>();
    this->Kh = config["Kh"].as<double>();
    this->Knb = config["Knb"].as<double>();
    this->y = config["y"].as<double>();
    this->INIT_ALPHA = config["INIT_ALPHA"].as<double>();
    this->MUTATION_ALPHA = config["MUTATION_ALPHA"].as<double>();
    this->STEP_ALPHA = config["STEP_ALPHA"].as<double>();
    this->INIT_BETA = config["INIT_BETA"].as<double>();
    this->MUTATION_BETA = config["MUTATION_BETA"].as<double>();
    this->STEP_BETA = config["STEP_BETA"].as<double>();
    this->INIT_GAMMA = config["INIT_GAMMA"].as<double>();
    this->INIT_DELTA = config["INIT_DELTA"].as<double>();
    this->MUTATION_GAMMA = config["MUTATION_GAMMA"].as<double>();
    this->MUTATION_DELTA = config["MUTATION_DELTA"].as<double>();
    this->STEP_GAMMA = config["STEP_GAMMA"].as<double>();
    this->STEP_DELTA = config["STEP_DELTA"].as<double>();
    this->MUTATION_DRIFT = config["MUTATION_DRIFT"].as<double>();
    this->STEP_DRIFT = config["STEP_DRIFT"].as<double>();

    this->driftUniform = uniform_real_distribution<double>(100, 200);
    this->uniform = uniform_real_distribution<double>(0, 1);

    this->mainWriter = new std::ofstream("main_" + this->name + ".txt");
    this->lastGenerationWriter = new std::ofstream("last_generation_" + this->name + ".txt");

    const int seed = 0;
    this->generator = new std::default_random_engine(seed);
}

Parameters::~Parameters() {
    delete mainWriter;
    delete lastGenerationWriter;
    delete generator;
}


void Parameters::print() {
    this->print(*mainWriter);
    this->print(*lastGenerationWriter);
}

void Parameters::print(std::ofstream &outputStream) {
    outputStream << "PARAMETER VALUES" << endl

                 << "Bet-hedging_help?:" << "\t" << this->isBetHedgingHelp() << endl
                 << "Help_obligatory?:" << "\t" << this->isHelpObligatory() << endl
                 << "Predictable_environment?:" << "\t" << this->isPredictableEnvironment() << endl
                 << "Evolution_help_after_dispersal?:" << "\t" << this->isEvolutionHelpAfterDispersal() << endl
                 << "No_group_augmentation?:" << "\t" << this->isNoGroupAugmentation() << endl
                 << "No_effect_relatedness?:" << "\t" << this->isNoRelatedness() << endl
                 << "No_effect_age_inheritance?:" << "\t" << this->isAgeNoInfluenceInheritance() << endl
                 << "Initial_population:" << "\t" << this->getMaxColonies() * (this->getInitNumHelpers() + 1) << endl
                 << "Number_of_colonies:" << "\t" << this->getMaxColonies() << endl
                 << "Number_generations:" << "\t" << this->getNumGenerations() << endl
                 << "Number_replicates:" << "\t" << this->getMaxNumReplicates() << endl
                 << "Min_age_become_breeder:" << "\t" << this->getMinAgeBecomeBreeder() << endl
                 << "Fixed_group_size:" << "\t" << this->getFixedGroupSize() << endl
                 << "Reduced_relatedness:" << "\t" << this->getReducedRelatedness() << endl
                 << "ImmigrantsMax:" << "\t" << this->getFloatersSampledImmigration() << endl
                 << "m(Overall_mortality):" << "\t" << this->getM() << endl
                 << "X0(intercept):" << "\t" << this->getX0() << endl
                 << "Xh(Cost_help_survival):" << "\t" << this->getXh() << endl
                 << "Xn(Benefit_group_size_survival):" << "\t" << this->getXn() << endl
                 << "Xe(Cost_expulsion_survival):" << "\t" << this->getXe() << endl
                 << "Xrs(Reproductive_suppression_survival):" << "\t" << this->getXrs() << endl
                 << "mOff(Average_mortality_offspring):" << "\t" << this->getMOff() << endl
                 << "mRate(Change_mortality_offspring):" << "\t" << this->getMRate() << endl
                 << "mStep(Rate_change_mortality_offspring):" << "\t" << this->getMStep() << endl
                 << "K0(Base_fecundity):" << "\t" << this->getK0() << endl
                 << "Kh(Benefit_help_fecundity):" << "\t" << this->getKh() << endl
                 << "Knb(Benefit_number_breeders_fecundity):" << "\t" << this->getKnb() << endl
                 << "y(Min_skew):" << "\t" << this->getY() << endl
                 << "initAlpha:" << "\t" << this->getInitAlpha() << endl
                 << "initBeta:" << "\t" << this->getInitBeta() << endl
                 << "initGamma:" << "\t" << this->getInitGamma() << endl
                 << "initDelta:" << "\t" << this->getInitDelta() << endl
                 << "mutAlpha:" << "\t" << this->getMutationAlpha() << endl
                 << "mutBeta:" << "\t" << this->getMutationBeta() << endl
                 << "mutGamma:" << "\t" << this->getMutationGamma() << endl
                 << "mutDelta:" << "\t" << this->getMutationDelta() << endl
                 << "mutDrift:" << "\t" << this->getMutationDrift() << endl
                 << "stepAlpha:" << "\t" << this->getStepAlpha() << endl
                 << "stepBeta:" << "\t" << this->getStepBeta() << endl
                 << "stepGamma:" << "\t" << this->getStepGamma() << endl
                 << "stepDelta:" << "\t" << this->getStepDelta() << endl
                 << "stepDrift:" << "\t" << this->getStepDrift() << endl << endl;
}


const string &Parameters::getName() const {
    return name;
}

bool Parameters::isBetHedgingHelp() const {
    return BET_HEDGING_HELP;
}

bool Parameters::isHelpObligatory() const {
    return HELP_OBLIGATORY;
}

bool Parameters::isPredictableEnvironment() const {
    return PREDICTABLE_ENVIRONMENT;
}

bool Parameters::isEvolutionHelpAfterDispersal() const {
    return EVOLUTION_HELP_AFTER_DISPERSAL;
}

bool Parameters::isNoGroupAugmentation() const {
    return NO_GROUP_AUGMENTATION;
}

bool Parameters::isNoRelatedness() const {
    return NO_RELATEDNESS;
}

bool Parameters::isAgeNoInfluenceInheritance() const {
    return AGE_NO_INFLUENCE_INHERITANCE;
}

int Parameters::getMaxColonies() const {
    return MAX_COLONIES;
}

int Parameters::getNumGenerations() const {
    return NUM_GENERATIONS;
}

int Parameters::getMaxNumReplicates() const {
    return MAX_NUM_REPLICATES;
}

int Parameters::getSkip() const {
    return SKIP;
}

int Parameters::getInitNumHelpers() const {
    return INIT_NUM_HELPERS;
}

int Parameters::getFloatersSampledImmigration() const {
    return FLOATERS_SAMPLED_IMMIGRATION;
}

int Parameters::getMinAgeBecomeBreeder() const {
    return MIN_AGE_BECOME_BREEDER;
}

double Parameters::getFixedGroupSize() const {
    return FIXED_GROUP_SIZE;
}

int Parameters::getReducedRelatedness() const {
    return REDUCED_RELATEDNESS;
}

double Parameters::getM() const {
    return m;
}

double Parameters::getX0() const {
    return X0;
}

double Parameters::getXh() const {
    return Xh;
}

double Parameters::getXn() const {
    return Xn;
}

double Parameters::getXe() const {
    return Xe;
}

double Parameters::getXrs() const {
    return Xrs;
}

double Parameters::getMOff() const {
    return mOff;
}

double Parameters::getMRate() const {
    return mRate;
}

double Parameters::getMStep() const {
    return mStep;
}

double Parameters::getK0() const {
    return K0;
}

double Parameters::getKh() const {
    return Kh;
}

double Parameters::getKnb() const {
    return Knb;
}

double Parameters::getY() const {
    return y;
}

double Parameters::getInitAlpha() const {
    return INIT_ALPHA;
}

double Parameters::getMutationAlpha() const {
    return MUTATION_ALPHA;
}

double Parameters::getStepAlpha() const {
    return STEP_ALPHA;
}

double Parameters::getInitBeta() const {
    return INIT_BETA;
}

double Parameters::getMutationBeta() const {
    return MUTATION_BETA;
}

double Parameters::getStepBeta() const {
    return STEP_BETA;
}

double Parameters::getInitGamma() const {
    return INIT_GAMMA;
}

double Parameters::getMutationGamma() const {
    return MUTATION_GAMMA;
}

double Parameters::getStepGamma() const {
    return STEP_GAMMA;
}

double Parameters::getInitDelta() const {
    return INIT_DELTA;
}

double Parameters::getMutationDelta() const {
    return MUTATION_DELTA;
}

double Parameters::getStepDelta() const {
    return STEP_DELTA;
}

double Parameters::getMutationDrift() const {
    return MUTATION_DRIFT;
}

double Parameters::getStepDrift() const {
    return STEP_DRIFT;
}


std::string Parameters::getName(std::string url) {


    unsigned first = url.find("parameters/");
    unsigned last = url.find(".yml");
    string name = url.substr(first, last - first);
    replace(name.begin(), name.end(), '/', '_');

    return name;
}

ofstream *Parameters::getMainWriter() const {
    return mainWriter;
}

ofstream *Parameters::getLastGenerationWriter() const {
    return lastGenerationWriter;
}

default_random_engine *Parameters::getGenerator() const {
    return generator;
}

/**
 * @brief Returns the singleton instance of the Parameters class.
 * @return
 */
Parameters *Parameters::instance() {
    if (singletonInstance == nullptr) {
        singletonInstance = new Parameters;
    }
    return singletonInstance;
}

/**
 * @brief Initializes the singleton instance of the Parameters class for unit testing.
 * @return
 */
Parameters *Parameters::unitTestInit() {
    singletonInstance = new Parameters("../parameters/unit_tests.yml");
    std::cout << "Unit test parameters initialized" << std::endl;
    return singletonInstance;
}








