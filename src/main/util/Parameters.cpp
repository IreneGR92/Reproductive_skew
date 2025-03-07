#include <iostream>
#include <algorithm>
#include "yaml-cpp/yaml.h"
#include "Parameters.h"

#include "Config.h"
#include "spdlog/spdlog.h"


using namespace std;


//default value if not provided
Parameters::Parameters(const int replica) : Parameters("../parameters/default.yml", replica) {
}

Parameters::Parameters(const string &filename, const int replica) : replica(replica) {
    std::string path = Config::GET_PARAMETERS_FOLDER() + "/" + filename;

    spdlog::debug("Loading parameters from file: {}", path);
    YAML::Node config;
    try {
        config = YAML::LoadFile(path);
    } catch (YAML::BadFile &e) {
        spdlog::error("Error loading parameters from file: {}", filename);
        throw e;
    }
    this->name = this->removeExtension(filename);
    this->BET_HEDGING_HELP = config["BET_HEDGING_HELP"].as<bool>();
    this->HELP_OBLIGATORY = config["HELP_OBLIGATORY"].as<bool>();
    this->PREDICTABLE_ENVIRONMENT = config["PREDICTABLE_ENVIRONMENT"].as<bool>();
    this->DOMINANT_BREEDER_REPLACEMENT = config["DOMINANT_BREEDER_REPLACEMENT"].as<bool>();
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
    this->Xf = config["Xf"].as<double>();
    this->mOff = config["mOff"].as<double>();
    this->mFreq = config["mFreq"].as<double>();
    this->mMagnit = config["mMagnit"].as<double>();
    this->K0 = config["K0"].as<double>();
    this->Kh = config["Kh"].as<double>();
    this->Knb = config["Knb"].as<double>();
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


    this->generator = new std::default_random_engine(SEED + replica);
}

Parameters::~Parameters() {
    delete generator;
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

bool Parameters::isDominantBreederReplacement() const {
    return DOMINANT_BREEDER_REPLACEMENT;
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

double Parameters::getXf() const {
    return Xf;
}

double Parameters::getMOff() const {
    return mOff;
}

double Parameters::getMFreq() const {
    return mFreq;
}

double Parameters::getMMagnit() const {
    return mMagnit;
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


std::string Parameters::removeExtension(std::string filename) {
    size_t lastDot = filename.find_last_of('.');
    if (lastDot == std::string::npos) {
        return filename; // No extension found
    }
    return filename.substr(0, lastDot);
}


default_random_engine *Parameters::getGenerator() const {
    return generator;
}


int Parameters::getReplica() const {
    return replica;
}

shared_ptr<Parameters> Parameters::cloneWithIncrementedReplica(int newReplica) {
    auto deepCopy = std::make_shared<Parameters>(*this); // Use copy constructor
    deepCopy->replica = newReplica; // Increment newReplica
    deepCopy->generator = new std::default_random_engine(SEED + newReplica);
    return deepCopy;
}
