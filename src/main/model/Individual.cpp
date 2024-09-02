
#include <algorithm>
#include <iostream>
#include <cassert>

#include "Individual.h"
#include "FishType.h"

//Constructor for reproduction of a Breeder
Individual::Individual(Individual &individual, FishType fishType, int &generation) {

    if (individual.fishType != BREEDER) {
        std::cout << "Error: only breeders can reproduce" << std::endl;
    }

    assert(individual.fishType == BREEDER);

    this->alpha = individual.alpha;
    this->beta = individual.beta;
    this->gamma = individual.gamma;
    this->delta = individual.delta;
    this->drift = individual.getDrift();

    this->dispersal = Parameters::NO_VALUE;
    this->help = Parameters::NO_VALUE;
    this->fecundity = Parameters::NO_VALUE;

    this->initializeIndividual(fishType);

    this->mutate(generation);
}

//Constructor for initial creation
Individual::Individual(FishType fishType) {

    auto param = Parameters::instance();

    this->alpha = param->getInitAlpha();
    this->beta = param->getInitBeta();
    this->gamma = param->getInitGamma();
    this->delta = param->getInitDelta();
    this->drift = param->driftUniform(*param->getGenerator());
    this->initializeIndividual(fishType);
}

void Individual::initializeIndividual(FishType type) {
    this->parameters = Parameters::instance();
    this->dispersal = Parameters::NO_VALUE;
    this->help = 0;
    this->survival = Parameters::NO_VALUE;
    this->fishType = type;
    this->inherit = true;
    this->age = 1;
    this->ageBecomeBreeder = Parameters::NO_VALUE;
    this->id = Parameters::instance()->nextId();

}

void Individual::setGroupIndex(int groupIndex) {
    Individual::groupIndex = groupIndex;
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
    if (fishType == HELPER) {
        help = alpha;
        if (help < 0) { help = 0; }
    } else {
        help = Parameters::NO_VALUE;
        std::cout << "Error: floaters get a help value" << std::endl;
    }
}


/*SURVIVAL*/

void Individual::calcSurvival(const int &groupSize, double delta) {

    double thisGroupSize;
    double Xn, Xe, Xh, Xrs, X0, X1;

    if (parameters->isNoGroupAugmentation()) {
        thisGroupSize = parameters->getFixedGroupSize();
    } else {
        thisGroupSize = groupSize;
    }

    X0 = parameters->getX0();           // min survival
    X1 = (1 - X0) - parameters->getM(); //  X0 + X1 max survival

    if (fishType == FLOATER) {
        Xn = 0;     // effect of group size
        Xe = 0;     // effect of expulsion
        Xh = 0;     // effect of help
        Xrs = 0;    // effect of reproductive suppression
    } else if (fishType == BREEDER) {
        Xn = parameters->getXn();
        Xe = parameters->getXe();
        Xh = 0;
        Xrs = parameters->getXrs();
    } else { //HELPER
        Xn = parameters->getXn();
        Xe = parameters->getXe();
        Xh = parameters->getXh();
        Xrs = 0;
    }

    if (Xn + Xe + Xh + Xrs == 0) {
        this->survival = X0;
    } else {
        this->survival = X0 + ((Xn * X1 / (1 + exp(-thisGroupSize))) + (Xh * X1 / (1 + exp(this->help))) +
                               (Xe * X1 / (1 + exp(this->gamma))) + (Xrs * X1 / (1 + exp(delta)))) /
                              (Xn + Xe + Xh + Xrs);
    }

    assert(survival >= 0 && survival <= 1);
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
    double mutationAlpha;

    if (parameters->isEvolutionHelpAfterDispersal() && generation < 25000) {
        mutationAlpha = 0;
    } else if (parameters->isNoHelp()) { //TODO: if initAlpha is not 0, this is not correct
        mutationAlpha = 0;
    } else {
        mutationAlpha = parameters->getMutationAlpha();
    }

    if (parameters->uniform(rng) < mutationAlpha) {
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

double Individual::getFecundity() const {
    return fecundity;
}

FishType Individual::getFishType() const {
    return fishType;
}

void Individual::setFishType(FishType type) {
    Individual::fishType = type;
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
        case FECUNDITY:
            return this->fecundity;

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






