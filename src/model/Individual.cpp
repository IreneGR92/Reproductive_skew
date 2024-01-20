
#include <iostream>
#include <cassert>

#include "Individual.h"
#include "IndRole.h"

//Constructor for reproduction of a Breeder
Individual::Individual(Individual &individual, IndRole fishType, int &generation) {

    assert(individual.fishType == BREEDER);

    this->alpha = individual.alpha;
    this->alphaAge = individual.alphaAge;
    this->beta = individual.beta;
    this->betaAge = individual.betaAge;
    this->gamma = individual.gamma;
    this->gammaAge = individual.gammaAge;
    this->drift = individual.getDrift();

    this->dispersal = Parameters::NO_VALUE;
    this->help = Parameters::NO_VALUE;

    this->initializeIndividual(fishType);

    this->mutate(generation);
}

//Constructor for initial creation
Individual::Individual(IndRole indRole) {

    auto param = Parameters::instance();

    this->alpha = param->getInitAlpha();
    this->alphaAge = param->getInitAlphaAge();
    this->beta = param->getInitBeta();
    this->betaAge = param->getInitBetaAge();
    this->gamma = param->getInitGamma();
    this->gammaAge = param->getInitGammaAge();
    this->drift = param->driftUniform(*param->getGenerator());
    this->initializeIndividual(indRole);
}

void Individual::initializeIndividual(IndRole type) {
    this->parameters = Parameters::instance();
    this->dispersal = Parameters::NO_VALUE;
    this->help = 0;
    this->survival = Parameters::NO_VALUE;
    this->fishType = type;
    this->inherit = true;
    this->age = 1;
    this->ageBecomeBreeder = Parameters::NO_VALUE;

}

void Individual::setGroupIndex(int groupIndex) {
    Individual::groupIndex = groupIndex;
}


/* BECOME FLOATER (STAY VS DISPERSE) */

void Individual::calcDispersal() {
    if (age == 1) {
        this->dispersal = beta;
    } else {
        this->dispersal = 0;
    }
}

/*DISPLAY LEVEL OF HELP*/

void Individual::calcHelp() {
    if (fishType == HELPER) {
        if (!parameters->isReactionNormHelp()) {
            help = alpha;

        } else {
            help = alpha + alphaAge * age;
        }
        if (help < 0) { help = 0; }

    } else {
        help = Parameters::NO_VALUE;
        std::cout << "Error: floaters get a help value" << std::endl;
    }
}


/*SURVIVAL*/

void Individual::calculateSurvival(const int &groupSize) {

    if (fishType == FLOATER) {
        this->survival = (1 - parameters->getM() * parameters->getN()) / (1 + exp(-parameters->getX0()));
    } else if (fishType == HELPER) {
        this->survival = (1 - parameters->getM()) /
                         (1 + exp(-parameters->getX0() - parameters->getXsn() * groupSize +
                                  parameters->getXsh() * this->help));
    } else {
        this->survival = Parameters::NO_VALUE;
    }
}


void Individual::mutate(int generation) // mutate genome of offspring
{
    auto rng = *parameters->getGenerator();
    std::normal_distribution<double> NormalA(0, parameters->getStepAlpha()); //TODO: could be simplified
    std::normal_distribution<double> NormalB(0, parameters->getStepBeta());
    std::normal_distribution<double> NormalG(0, parameters->getStepGamma());
    std::normal_distribution<double> NormalD(0, parameters->getStepDrift());

    // Alpha
    double mutationAlpha;
    double mutationAlphaAge;

    if (parameters->isEvolutionHelpAfterDispersal() && generation < 25000) {
        mutationAlpha = 0;
        mutationAlphaAge = 0;
    } else {
        mutationAlpha = parameters->getMutationAlpha();
        mutationAlphaAge = parameters->getMutationAlphaAge();
    }

    if (parameters->uniform(rng) < mutationAlpha) {
        alpha += NormalA(rng);
    }
    if (parameters->isReactionNormHelp()) {
        if (parameters->uniform(rng) < mutationAlphaAge) {
            alphaAge += NormalA(rng);
        }
    }

    // Beta
    if (parameters->uniform(rng) < parameters->getMutationBeta()) {
        beta += NormalB(rng);

            if (beta < 0.5) { beta = 0.5; }
            else if (beta > 1) { beta = 1; }

    }

//    // Gamma
//    if (parameters->uniform(rng) < parameters->getMutationGamma()) {
//        gamma += NormalG(rng);
//        if (!parameters->isReactionNormTask()) {
//            if (gamma < 0) { gamma = 0; }
//            else if (gamma > 1) { gamma = 1; }
//        }
//    }
//    if (parameters->isReactionNormTask()) {
//        if (parameters->uniform(rng) < parameters->getMutationGammaAge()) {
//            gammaAge += NormalG(rng);
//        }
//    }

    // Drift
    if (parameters->uniform(rng) < parameters->getMutationDrift()) {
        drift += NormalD(rng);
    }
}


/* INCREASE AGE */
//for breeders
void Individual::increaseAge(bool alive) {
    if (alive) {
        this->age++;
    } else {
        this->age = Parameters::NO_VALUE;
    }
}

//for helpers and floaters
void Individual::increaseAge() {
    this->increaseAge(true);
}


/* GETTERS AND SETTERS */

double Individual::getAlpha() const {
    return alpha;
}

double Individual::getAlphaAge() const {
    return alphaAge;
}

double Individual::getBeta() const {
    return beta;
}

double Individual::getBetaAge() const {
    return betaAge;
}

double Individual::getGamma() const {
    return gamma;
}

double Individual::getGammaAge() const {
    return gammaAge;
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

void Individual::setHelp(double help_) {
    Individual::help = help_;
}

double Individual::getSurvival() const {
    return survival;
}

IndRole Individual::getIndRole() const {
    return fishType;
}

void Individual::setIndRole(IndRole type) {
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

void Individual::setAgeBecomeBreeder(int ageBecomeBreeder_) {
    Individual::ageBecomeBreeder = ageBecomeBreeder_;
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
        case ALPHA_AGE:
            return this->alphaAge;
        case BETA:
            return this->beta;
        case BETA_AGE:
            return this->betaAge;
        case GAMMA:
            return this->gamma;
        case GAMMA_AGE:
            return this->gammaAge;
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





