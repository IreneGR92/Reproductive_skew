#include <iostream>
#include <algorithm>
#include <yaml-cpp/yaml.h>
#include "Parameters.h"

using namespace std;


static Parameters *singletonInstance;

//default value if not provided
Parameters::Parameters() : Parameters("../parameters/default.yml") {}

Parameters::Parameters(const string &url) {
    singletonInstance = this;

    YAML::Node config = YAML::LoadFile(url);

    this->name = this->getName(url);
    this->REACTION_NORM_HELP = config["REACTION_NORM_HELP"].as<bool>();
    this->REACTION_NORM_DISPERSAL = config["REACTION_NORM_DISPERSAL"].as<bool>();
    this->EVOLUTION_HELP_AFTER_DISPERSAL = config["EVOLUTION_HELP_AFTER_DISPERSAL"].as<bool>();
    this->DIRECT_BROOD_CARE_ONLY = config["DIRECT_BROOD_CARE_ONLY"].as<bool>();
    this->NO_GROUP_AUGMENTATION = config["NO_GROUP_AUGMENTATION"].as<bool>();
    this->NO_RELATEDNESS = config["NO_RELATEDNESS"].as<bool>();
    this->NO_RELATEDNESS_RANDOM_GROUP = config["NO_RELATEDNESS_RANDOM_GROUP"].as<bool>();
    this->AGE_NO_INFLUENCE_INHERITANCE = config["AGE_NO_INFLUENCE_INHERITANCE"].as<bool>();
    this->REACTION_NORM_TASK = config["REACTION_NORM_TASK"].as<bool>();
    this->NEED_DIVISION_LABOUR = config["NEED_DIVISION_LABOUR"].as<bool>();
    this->MAX_COLONIES = config["MAX_COLONIES"].as<int>();
    this->NUM_GENERATIONS = config["NUM_GENERATIONS"].as<int>();
    this->MAX_NUM_REPLICATES = config["MAX_NUM_REPLICATES"].as<int>();
    this->SKIP = config["SKIP"].as<int>();
    this->INIT_NUM_HELPERS = config["INIT_NUM_HELPERS"].as<int>();
    this->BIAS_FLOAT_BREEDER = config["BIAS_FLOAT_BREEDER"].as<double>();
    this->MIN_AGE_BECOME_BREEDER = config["MIN_AGE_BECOME_BREEDER"].as<int>();
    this->FIXED_IND_QUALITY = config["FIXED_IND_QUALITY"].as<int>();
    this->FIXED_GROUP_SIZE = config["FIXED_GROUP_SIZE"].as<double>();
    this->m = config["m"].as<double>();
    this->n = config["n"].as<double>();
    this->X0 = config["X0"].as<double>();
    this->Xsh = config["Xsh"].as<double>();
    this->Xsn = config["Xsn"].as<double>();
    this->Yh = config["Yh"].as<double>();
    this->K0 = config["K0"].as<double>();
    this->Kh = config["Kh"].as<double>();
    this->Km = config["Km"].as<double>();
    this->INIT_ALPHA = config["INIT_ALPHA"].as<double>();
    this->INIT_ALPHA_AGE = config["INIT_ALPHA_AGE"].as<double>();
    this->MUTATION_ALPHA = config["MUTATION_ALPHA"].as<double>();
    this->MUTATION_ALPHA_AGE = config["MUTATION_ALPHA_AGE"].as<double>();
    this->STEP_ALPHA = config["STEP_ALPHA"].as<double>();
    this->INIT_BETA = config["INIT_BETA"].as<double>();
    this->INIT_BETA_AGE = config["INIT_BETA_AGE"].as<double>();
    this->MUTATION_BETA = config["MUTATION_BETA"].as<double>();
    this->MUTATION_BETA_AGE = config["MUTATION_BETA_AGE"].as<double>();
    this->STEP_BETA = config["STEP_BETA"].as<double>();
    this->INIT_GAMMA = config["INIT_GAMMA"].as<double>();
    this->INIT_GAMMA_AGE = config["INIT_GAMMA_AGE"].as<double>();
    this->MUTATION_GAMMA = config["MUTATION_GAMMA"].as<double>();
    this->MUTATION_GAMMA_AGE = config["MUTATION_GAMMA_AGE"].as<double>();
    this->STEP_GAMMA = config["STEP_GAMMA"].as<double>();
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

                 << "Reaction_norm_help?:" << "\t" << this->isReactionNormHelp() << endl
                 << "Reaction_norm_dispersal?:" << "\t" << this->isReactionNormDispersal() << endl
                 << "Evolution_help_after_dispersal?:" << "\t" << this->isEvolutionHelpAfterDispersal() << endl
                 << "Reaction_norm_task?:" << "\t" << this->isReactionNormTask() << endl
                 << "Need_division_labour?:" << "\t" << this->isNeedDivisionLabour() << endl
                 << "Direct_brood_care_only?:" << "\t" << this->isDirectBroodCareOnly() << endl
                 << "No_group_augmentation?:" << "\t" << this->isNoGroupAugmentation() << endl
                 << "No_effect_relatedness?:" << "\t" << this->isNoRelatedness() << endl
                 << "Non-related_helpers_random_group?:" << "\t" << this->isNoRelatednessRandomGroup() << endl
                 << "No_effect_age_inheritance?:" << "\t" << this->isAgeNoInfluenceInheritance() << endl
                 << "Initial_population:" << "\t" << this->getMaxColonies() * (this->getInitNumHelpers() + 1) << endl
                 << "Number_of_colonies:" << "\t" << this->getMaxColonies() << endl
                 << "Number_generations:" << "\t" << this->getNumGenerations() << endl
                 << "Number_replicates:" << "\t" << this->getMaxNumReplicates() << endl
                 << "Min_age_become_breeder:" << "\t" << this->getMinAgeBecomeBreeder() << endl
                 << "Fixed_ind_quality:" << "\t" << this->getFixedIndQuality() << endl
                 << "Fixed_group_size:" << "\t" << this->getFixedGroupSize() << endl
                 << "Bias_float_breeder:" << "\t" << this->getBiasFloatBreeder() << endl
                 << "m(Overall_mortality):" << "\t" << this->getM() << endl
                 << "n(Mortality_dispersal):" << "\t" << this->getN() << endl
                 << "X0(intercept):" << "\t" << this->getX0() << endl
                 << "Xh(Cost_help_survival):" << "\t" << this->getXsh() << endl
                 << "Xn(Benefit_group_size_survival):" << "\t" << this->getXsn() << endl
                 << "Yh(Cost_help_rank):" << "\t" << this->getYh() << endl
                 << "K0(Base_fecundity):" << "\t" << this->getK0() << endl
                 << "Kh(Benefit_help_fecundity):" << "\t" << this->getKh() << endl
                 << "Km(Reduced_need_DOL):" << "\t" << this->getKm() << endl
                 << "initAlpha:" << "\t" << this->getInitAlpha() << endl
                 << "initAlphaAge:" << "\t" << this->getInitAlphaAge() << endl
                 << "initBeta:" << "\t" << this->getInitBeta() << endl
                 << "initBetaAge:" << "\t" << this->getInitBetaAge() << endl
                 << "initGamma:" << "\t" << this->getInitGamma() << endl
                 << "initGammaAge:" << "\t" << this->getInitGammaAge() << endl
                 << "mutAlpha:" << "\t" << this->getMutationAlpha() << endl
                 << "mutAlphaAge:" << "\t" << this->getMutationAlphaAge() << endl
                 << "mutBeta:" << "\t" << this->getMutationBeta() << endl
                 << "mutBetaAge:" << "\t" << this->getMutationBetaAge() << endl
                 << "mutGamma:" << "\t" << this->getMutationGamma() << endl
                 << "mutGammaAge:" << "\t" << this->getMutationGammaAge() << endl
                 << "mutDrift:" << "\t" << this->getMutationDrift() << endl
                 << "stepAlpha:" << "\t" << this->getStepAlpha() << endl
                 << "stepBeta:" << "\t" << this->getStepBeta() << endl
                 << "stepGamma:" << "\t" << this->getStepGamma() << endl
                 << "stepDrift:" << "\t" << this->getStepDrift() << endl << endl;
}


const string &Parameters::getName() const {
    return name;
}

bool Parameters::isReactionNormHelp() const {
    return REACTION_NORM_HELP;
}

bool Parameters::isReactionNormDispersal() const {
    return REACTION_NORM_DISPERSAL;
}

bool Parameters::isEvolutionHelpAfterDispersal() const {
    return EVOLUTION_HELP_AFTER_DISPERSAL;
}

bool Parameters::isReactionNormTask() const {
    return REACTION_NORM_TASK;
}

bool Parameters::isNeedDivisionLabour() const {
    return NEED_DIVISION_LABOUR;
}

bool Parameters::isDirectBroodCareOnly() const {
    return DIRECT_BROOD_CARE_ONLY;
}

bool Parameters::isNoGroupAugmentation() const {
    return NO_GROUP_AUGMENTATION;
}

bool Parameters::isNoRelatedness() const {
    return NO_RELATEDNESS;
}

bool Parameters::isNoRelatednessRandomGroup() const {
    return NO_RELATEDNESS_RANDOM_GROUP;
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

double Parameters::getBiasFloatBreeder() const {
    return BIAS_FLOAT_BREEDER;
}

int Parameters::getMinAgeBecomeBreeder() const {
    return MIN_AGE_BECOME_BREEDER;
}

int Parameters::getFixedIndQuality() const {
    return FIXED_IND_QUALITY;
}

double Parameters::getFixedGroupSize() const {
    return FIXED_GROUP_SIZE;
}

double Parameters::getM() const {
    return m;
}

double Parameters::getN() const {
    return n;
}

double Parameters::getX0() const {
    return X0;
}

double Parameters::getXsh() const {
    return Xsh;
}

double Parameters::getXsn() const {
    return Xsn;
}

double Parameters::getYh() const {
    return Yh;
}

double Parameters::getK0() const {
    return K0;
}

double Parameters::getKh() const {
    return Kh;
}

double Parameters::getKm() const {
    return Km;
}

double Parameters::getInitAlpha() const {
    return INIT_ALPHA;
}

double Parameters::getInitAlphaAge() const {
    return INIT_ALPHA_AGE;
}

double Parameters::getMutationAlpha() const {
    return MUTATION_ALPHA;
}

double Parameters::getMutationAlphaAge() const {
    return MUTATION_ALPHA_AGE;
}

double Parameters::getStepAlpha() const {
    return STEP_ALPHA;
}

double Parameters::getInitBeta() const {
    return INIT_BETA;
}

double Parameters::getInitBetaAge() const {
    return INIT_BETA_AGE;
}

double Parameters::getMutationBeta() const {
    return MUTATION_BETA;
}

double Parameters::getMutationBetaAge() const {
    return MUTATION_BETA_AGE;
}

double Parameters::getStepBeta() const {
    return STEP_BETA;
}

double Parameters::getInitGamma() const {
    return INIT_GAMMA;
}

double Parameters::getInitGammaAge() const {
    return INIT_GAMMA_AGE;
}

double Parameters::getMutationGamma() const {
    return MUTATION_GAMMA;
}

double Parameters::getMutationGammaAge() const {
    return MUTATION_GAMMA_AGE;
}

double Parameters::getStepGamma() const {
    return STEP_GAMMA;
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

Parameters *Parameters::instance() {
    if (singletonInstance == nullptr) {
        singletonInstance = new Parameters;
    }
    return singletonInstance;
}








