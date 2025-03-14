
#include <iostream>
#include <cassert>

#include "Individual.h"
#include "RoleType.h"
#include "spdlog/spdlog.h"

//Constructor for reproduction of a Breeder
Individual::Individual(Individual &individual, RoleType roleType, int &generation) : parameters(individual.parameters) {

    if (individual.roleType != BREEDER) {
        spdlog::error("only breeders can reproduce");
    }

    assert(individual.roleType == BREEDER);

    this->alpha = individual.alpha;
    this->beta = individual.beta;
    this->gamma = individual.gamma;
    this->delta = individual.delta;
    this->drift = individual.getDrift();

    this->dispersal = Parameters::NO_VALUE;
    this->help = Parameters::NO_VALUE;

    this->initializeIndividual(roleType);

    this->mutate(generation);
}

//Constructor for initial creation
Individual::Individual(RoleType roleType, const std::shared_ptr<Parameters> &parameters) : parameters(parameters) {


    this->alpha = parameters->getInitAlpha();
    this->beta = parameters->getInitBeta();
    this->gamma = parameters->getInitGamma();
    this->delta = parameters->getInitDelta();
    this->drift = parameters->driftUniform(*parameters->getGenerator());
    this->initializeIndividual(roleType);
}

void Individual::initializeIndividual(RoleType type) {
    this->dispersal = Parameters::NO_VALUE;
    this->help = 0;
    this->survival = Parameters::NO_VALUE;
    this->roleType = type;
    this->inherit = true;
    this->age = 1;
    this->ageBecomeBreeder = Parameters::NO_VALUE;
    this->id = parameters->nextId();

}

void Individual::setGroupIndex(int groupIndex) {
    this->groupIndex = groupIndex;
}


/* BECOME FLOATER (STAY VS DISPERSE) */

void Individual::calcDispersal() {
    if (age == 1) {
        this->dispersal = beta;
        if (this->dispersal > 1) { this->dispersal = 1; } else if (this->dispersal < 0.5) { this->dispersal = 0.5; }

    } else {
        this->dispersal = 0;
    }
}

/*DISPLAY LEVEL OF HELP*/

void Individual::calcHelp() {
    if (roleType == HELPER) {
        help = alpha;
        if (help < 0) { help = 0; }
    } else {
        help = Parameters::NO_VALUE;
        spdlog::error("floaters get a help value");
    }
}


/*SURVIVAL*/

void Individual::calcSurvival(const int &groupSize, double delta, const bool &hasPotentialImmigrants) {

    double thisGroupSize;
    double Xn, Xe, Xh, Xrs, Xf, X0, m;

    if (parameters->isNoGroupAugmentation()) {
        thisGroupSize = parameters->getFixedGroupSize();
    } else {
        thisGroupSize = groupSize;
    }

    if (roleType == FLOATER) {
        Xn = 0;     // benefit of group size
        Xe = 0;     // cost of expulsion of immigrants
        Xh = 0;     // cost of help
        Xrs = 0;    // cost of reproductive suppression
        Xf = parameters->getXf(); //additional survival for floaters
    } else if (roleType == BREEDER) {
        Xn = parameters->getXn();
        Xe = parameters->getXe();
        Xh = 0;
        Xrs = parameters->getXrs();
        Xf = 0;
    } else { //HELPER
        Xn = parameters->getXn();
        Xe = parameters->getXe();
        Xh = parameters->getXh();
        Xrs = 0;
        Xf = 0;
    }

    X0 = parameters->getX0(); // base survival without the effect of help or group size
    m = parameters->getM(); // base mortality
    const double gamma = hasPotentialImmigrants ? this->gamma : 0; // no cost of expulsion if no potential immigrants


    this->survival = (1 - m) / (1 + exp(-X0 - Xn * thisGroupSize + Xe * gamma + Xrs * delta + Xh * this->help - Xf));

    if (survival < 0 && survival > 1) {
        spdlog::error("Survival rate is not between 0 and 1");
    }
    assert(survival >= 0 && survival <= 1);
    if (this->survival > 0.95) { this->survival = 0.95; }
    else if (this->survival < 0) { this->survival = 0; } //prevent survival to be close to 1 or negative
}


/*REPRODUCTION*/

void Individual::mutate(int generation) // mutate genome of offspring
{
    auto rng = *parameters->getGenerator();
    std::normal_distribution<double> NormalA(0, parameters->getStepAlpha());
    std::normal_distribution<double> NormalB(0, parameters->getStepBeta());
    std::normal_distribution<double> NormalG(0, parameters->getStepGamma());
    std::normal_distribution<double> NormalD(0, parameters->getStepDelta());
    std::normal_distribution<double> NormalDrift(0, parameters->getStepDrift());

    // Alpha
    if (parameters->uniform(rng) < parameters->getMutationAlpha()) {
        alpha += NormalA(rng);
    }

    // Beta
    if (parameters->uniform(rng) < parameters->getMutationBeta()) {
        beta += NormalB(rng);
    }

    // Gamma
    if (parameters->uniform(rng) < parameters->getMutationGamma()) {
        gamma += NormalG(rng);
    }

    //Delta
    if (parameters->uniform(rng) < parameters->getMutationDelta()) {
        delta += NormalD(rng);
    }

    // Drift
    if (parameters->uniform(rng) < parameters->getMutationDrift()) {
        drift += NormalDrift(rng);
    }
}


/* INCREASE AGE */
//for main breeders
void Individual::increaseAge(bool alive) {
    if (alive) {
        this->age++;
    } else {
        this->age = Parameters::NO_VALUE;
    }
}

//for subordinate breeders, helpers and floaters
void Individual::increaseAge() {
    this->increaseAge(true);
}


/* GETTERS AND SETTERS */

double Individual::getAlpha() const {
    return alpha;
}

double Individual::getBeta() const {
    return beta;
}

double Individual::getGamma() const {
    return gamma;
}

double Individual::getDelta() const {
    return delta;
}

double Individual::getDrift() const {
    return drift;
}

double Individual::getDispersal() const {
    return dispersal;
}

double Individual::getHelp() const {
    return help;
}

double Individual::getSurvival() const {
    return survival;
}


RoleType Individual::getRoleType() const {
    return roleType;
}

void Individual::setRoleType(RoleType type) {
    Individual::roleType = type;
    if (type == BREEDER) {
        this->dispersal = Parameters::NO_VALUE;
        this->help = 0;
    }
    if (type == FLOATER) {
        this->help = 0;
    }
}

int Individual::getAge() const {
    return age;
}

void Individual::setAgeBecomeBreeder() {
    Individual::ageBecomeBreeder = this->age;
}

bool Individual::isInherit() const {
    return inherit;
}

void Individual::setInherit(bool inherit) {
    Individual::inherit = inherit;
}

double Individual::get(Attribute type) const {
    switch (type) {
        case ALPHA:
            return this->alpha;
        case BETA:
            return this->beta;
        case GAMMA:
            return this->gamma;
        case DELTA:
            return this->delta;
        case HELP:
            return this->help;
        case DISPERSAL:
            return this->dispersal;
        case SURVIVAL:
            return this->survival;
        case DRIFT:
            return this->drift;
        case AGE:
            return this->age;
        case AGE_BECOME_BREEDER:
            return this->ageBecomeBreeder;
    }

    assert(false);
}

int Individual::getGroupIndex() const {
    return groupIndex;
}

bool Individual::isViableBreeder() {
    if (age > parameters->getMinAgeBecomeBreeder() - 1) {
        return true;
    } else {
        return false;
    }
}

bool Individual::operator==(const Individual &other) const {
    return (this->id == other.id);
}






