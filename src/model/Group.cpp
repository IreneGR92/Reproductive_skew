
#include <algorithm>
#include <cassert>
#include <iostream>
#include "Group.h"
#include "FishType.h"

using namespace std;


Group::Group() : mainBreeder(BREEDER) {
    this->parameters = Parameters::instance();

    breederAlive = true;
    cumHelp = Parameters::NO_VALUE;
    fecundity = Parameters::NO_VALUE;
    realFecundity = Parameters::NO_VALUE;

    for (int i = 0; i < parameters->getInitNumHelpers(); ++i) {
        auto individual = Individual(HELPER);
        helpers.emplace_back(individual);
    }

    calculateGroupSize();
}

/* TOTAL NUMBER OF INDIVIDUALS PER GROUP*/

void Group::calculateGroupSize() {
    if (breederAlive) {
        groupSize = helpers.size() + breeders.size() + 1;
    } else {
        groupSize = helpers.size() + breeders.size();
    }
}

/*  DISPERSAL (STAY VS DISPERSE) */

vector<Individual> Group::disperse() {

    vector<Individual> newFloaters;

    for (int i = 0; i < helpers.size();) {
        Individual &helper = helpers[i];

        helper.calcDispersal();

        if (parameters->uniform(*parameters->getGenerator()) < helper.getDispersal()) {
            helper.setInherit(false); //the location of the individual is not the natal territory
            helper.setFishType(FLOATER);
            newFloaters.push_back(helper); //add the individual to the vector floaters in the last position
            helpers.removeIndividual(i);

        } else {
            helper.setFishType(HELPER); //individuals that stay or disperse to this group become helpers
            i++;
        }

    }
    return newFloaters;
}

std::vector<Individual> Group::reassignNoRelatedness(int index) {

    std::vector<Individual> noRelatedHelpers;

    for (int i = 0; i < helpers.size();) {
        Individual &helper = helpers[i];
        if (helper.getAge() == 1) { // all new offspring is assigned to new groups so no related to breeder

            helper.setInherit(false); //the location of the individual is not the natal territory
            noRelatedHelpers.push_back(helper); //add the individual to the vector in the last position
            helpers.removeIndividual(i); // removes the individual from the helpers vector

        } else {
            i++;
        }
        helper.setGroupIndex(index);
    }
    return noRelatedHelpers;
}

/*  CALCULATE ACCEPTANCE RATE FOR IMMIGRANTS */

double Group::getAcceptanceRate() {

    double acceptanceRate;
    double expulssionEffort = 0;
    double gamma;

    for (auto &helper: helpers) {
        if (helper.getGamma() < 0) {
            gamma = 0;
        } else {
            gamma = helper.getGamma();
        }
        expulssionEffort += gamma;

    }

    acceptanceRate = 1 - expulssionEffort;
    if (acceptanceRate < 0) {
        acceptanceRate = 0;
    }

    return acceptanceRate;
}


/*  CALCULATE CUMULATIVE LEVEL OF HELP */

void Group::calculateCumulativeHelp() //Calculate accumulative help of all individuals inside of each group.
{
    cumHelp = 0;

    //Level of help for helpers
    for (Individual &helper: helpers) {
        assert(helper.getFishType() == HELPER);
        helper.calcHelp();
        cumHelp += helper.getHelp();

    }
}

/*  MORTALITY */

void Group::survivalGroup() {
    this->calculateGroupSize();
    //Calculate survival for the helpers
    for (Individual &helper: helpers) {
        helper.calcSurvival(groupSize);
    }

    //Calculate the survival of the mainBreeder
    this->mainBreeder.calcSurvival(groupSize);
}

void Group::mortalityGroup(int &deaths) {

    vector<Individual, std::allocator<Individual>>::iterator helperIt;
    helperIt = helpers.begin();
    int sizeVectorHelpers = helpers.size();
    int counting = 0;
    while (!helpers.empty() && sizeVectorHelpers > counting) {

        //Mortality helpers
        if (parameters->uniform(*parameters->getGenerator()) > helperIt->getSurvival()) {
            *helperIt = helpers[helpers.size() - 1];
            helpers.pop_back();
            counting++;
            deaths++;
        } else
            helperIt++, counting++; //go to next individual
    }

    //Mortality mainBreeder
    if (parameters->uniform(*parameters->getGenerator()) > mainBreeder.getSurvival()) {
        breederAlive = false;
        deaths++;
    }
    this->calculateGroupSize(); //update group size after mortality
}


/* BECOME BREEDER */

void Group::newBreeder(vector<Individual> &floaters, int &newBreederOutsider, int &newBreederInsider, int &inheritance) {

    int i = 0;
    double sumRank = 0;
    double currentPosition = 0; //age of the previous ind taken from Candidates
    double RandP = parameters->uniform(*parameters->getGenerator());
    vector<Individual *> candidates;
    vector<double> position; //vector of age to choose with higher likelihood the ind with higher age

    if (helpers.empty()) {
        return; //TODO: Change behaviour of what happens when there are no helpers
    }

    //    Join the helpers in the group to the vector candidates
    for (auto &helper : helpers) {
        candidates.push_back(&helper);
    }

    //  Check if the candidates meet the age requirements, else remove them from the candidates vector
        candidates.erase(std::remove_if(candidates.begin(), candidates.end(),
                                    [](Individual* candidate) { return !candidate->isViableBreeder(); }), candidates.end());


    //  Choose new mainBreeder
    //      Choose mainBreeder with higher likelihood for the highest rank
    vector<Individual *, std::allocator<Individual *>>::iterator candidate;
    for (candidate = candidates.begin(); candidate < candidates.end(); ++candidate) {
        sumRank += (*candidate)->getAge(); //add all the ranks from the vector candidates
    }

    for (candidate = candidates.begin(); candidate < candidates.end(); ++candidate) {
        position.push_back(static_cast<double>((*candidate)->getAge()) / static_cast<double>(sumRank) +
                           currentPosition); //creates a vector with proportional segments to the rank of each individual
        currentPosition = position[position.size() - 1];
    }


    //      Make the chosen candidate the new mainBreeder
    for (candidate = candidates.begin(); candidate != candidates.end(); ++candidate) {
        if (RandP < position[candidate - candidates.begin()]){ //chooses the candidate with higher age
            mainBreeder = **candidate; //substitute the previous dead mainBreeder
            breederAlive = true;
            mainBreeder.setAgeBecomeBreeder(mainBreeder.getAge());

            if ((*candidate)->isInherit() == 0) //delete the ind from the vector floaters
            {
                newBreederOutsider++;
            } else {
                newBreederInsider++;
                inheritance++; //TODO: At the moment, newBreederInsider and inheritance are equivalent
            }

            **candidate = helpers[helpers.size() - 1]; //delete the ind from the vector helpers
            helpers.pop_back();

            mainBreeder.setFishType(BREEDER); //modify the class
            break;
        }
    }
}


Individual* Group::chooseNewBreeder() {
    double sumAge = 0;
    double currentPosition = 0;
    double randP = parameters->uniform(*parameters->getGenerator());
    std::vector<Individual*> candidates;
    std::vector<double> position;

    if (helpers.empty()) {
        return nullptr; //TODO: Change behaviour of what happens when there are no helpers
    }

    //    Join the helpers in the group to the vector candidates
    for (auto &helper : helpers) {
        candidates.push_back(&helper);
    }

    //  Check if the candidates meet the age requirements, else remove them from the candidates vector
    candidates.erase(std::remove_if(candidates.begin(), candidates.end(),
                                    [](Individual* candidate) { return !candidate->isViableBreeder(); }), candidates.end());


    // Calculate the sum of ages
    for (auto &candidate : candidates) {
        sumAge += candidate->getAge();
    }

    // Create a vector with proportional segments to the age of each individual
    for (auto &candidate : candidates) {
        position.push_back(static_cast<double>(candidate->getAge()) / sumAge + currentPosition);
        currentPosition = position.back();
    }

    // Choose the helper with higher age
    for (auto candidate = candidates.begin(); candidate != candidates.end(); ++candidate) {
        if (randP < position[std::distance(candidates.begin(), candidate)]) {
            Individual* chosenNewBreeder = *candidate;
            candidates.erase(candidate); // Remove the chosen helper from the candidates vector
            helpers.removeIndividual(std::distance(candidates.begin(), candidate)); // Remove the chosen helper from the helpers vector
            return chosenNewBreeder;
        }
    }
}



double Group::getReproductiveShare() {

    double reproductiveShare;

    reproductiveShare = 1 - mainBreeder.getDelta();
    if (reproductiveShare < 0) {
        reproductiveShare = 0;
    }

    return reproductiveShare;
}



/* INCREASE AGE OF ALL GROUP INDIVIDUALS*/
void Group::increaseAge() {
    for (Individual &helper: helpers) {
        helper.increaseAge();
    }

    for (Individual &breeder: breeders) {
        breeder.increaseAge();
    }

    if (breederAlive) {
        mainBreeder.increaseAge(breederAlive);
    }
}


/* REPRODUCTION */

void Group::reproduce(int generation) { // populate offspring generation
    //Calculate fecundity

    fecundity = parameters->getK0() + parameters->getKh() * cumHelp / (1 + cumHelp);

    poisson_distribution<int> PoissonFecundity(fecundity);
    realFecundity = PoissonFecundity(*parameters->getGenerator()); //integer number

    //Reproduction
    if (breederAlive) {
        for (int i = 0; i < realFecundity; i++) { //number of offspring dependent on real fecundity
            Individual offspring = Individual(mainBreeder, HELPER, generation);

            helpers.emplace_back(
                    offspring); //create a new individual as helper in the group. Call construct to assign the mother genetic values to the offspring, construct calls Mutate function.
        }
    }
}

const Individual &Group::getBreeder() const {
    return mainBreeder;
}

int Group::getGroupSize() const {
    return groupSize;
}

bool Group::isBreederAlive() const {
    return breederAlive;
}

double Group::getCumHelp() const {
    return cumHelp;
}

std::vector<double> Group::get(Attribute attribute, bool includeBreeder) const {
    std::vector<double> result;
    if (includeBreeder && isBreederAlive()) {
        result.push_back(mainBreeder.get(attribute));
    }
    for (const Individual &helper: helpers) {
        result.push_back(helper.get(attribute));
    }
    return result;
}

std::vector<double> Group::get(Attribute attribute) const {
    return this->get(attribute, true);
}


bool Group::hasHelpers() const {
    return !helpers.empty();
}

void Group::addHelper(Individual &helper) {
    helper.setFishType(HELPER);
    this->helpers.emplace_back(helper);
}

const IndividualVector &Group::getHelpers() const {
    return helpers;
}

void Group::addHelpers(vector<Individual> &helpers) {
    for (Individual &helper: helpers) {
        this->addHelper(helper);
    }
}




