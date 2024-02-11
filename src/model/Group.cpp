
#include <algorithm>
#include <cassert>
#include <iostream>
#include "Group.h"
#include "FishType.h"

using namespace std;


Group::Group() : mainBreeder(BREEDER) {
    this->parameters = Parameters::instance();

    mainBreederAlive = true;
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
    if (mainBreederAlive) {
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

    for (Individual &breeder: breeders) {
        breeder.calcSurvival(groupSize);
    }

    //Calculate the survival of the mainBreeder
    this->mainBreeder.calcSurvival(groupSize);
}

void Group::mortalityGroup(int &deaths) {
    this->mortalityGroupVector(deaths, helpers);
    this->mortalityGroupVector(deaths, breeders);

    //Mortality mainBreeder
    if (parameters->uniform(*parameters->getGenerator()) > mainBreeder.getSurvival()) {
        mainBreederAlive = false;
        deaths++;
    }
    this->calculateGroupSize(); //update group size after mortality
}


/* BECOME BREEDER */

//void Group::newBreeder(int &newBreederOutsider, int &newBreederInsider, int &inheritance) {
//
//    this->transferBreedersToHelpers();
//
//    double sumRank = 0;
//    double currentPosition = 0; //age of the previous ind taken from Candidates
//    double RandP = parameters->uniform(*parameters->getGenerator());
//    vector<Individual *> candidates;
//    vector<double> position; //vector of age to choose with higher likelihood the ind with higher age
//    int anyViableCandidate = 0;
//
//    if (helpers.empty()) {
//        mainBreederAlive = false;
//    } else {
//        //    Join the helpers in the group to the vector candidates
//        for (auto &helper: helpers) {
//            candidates.push_back(&helper);
//            anyViableCandidate += helper.isViableBreeder();
//        }
//
//        //  Check if the candidates meet the age requirements
//        //If none do, take a random candidate
//        if (anyViableCandidate == 0) {
//            std::shuffle(helpers.begin(), helpers.end(), *parameters->getGenerator());
//            mainBreeder = helpers.back(); //substitute the previous dead mainBreeder
//            mainBreederAlive = true;
//            mainBreeder.setAgeBecomeBreeder(mainBreeder.getAge());
//            mainBreeder.setFishType(BREEDER); //modify the class
//            if (mainBreeder.isInherit() == 0) //delete the ind from the vector floaters
//            {
//                newBreederOutsider++;
//            } else {
//                newBreederInsider++;
//                inheritance++; //TODO: At the moment, newBreederInsider and inheritance are equivalent
//            }
//            helpers.pop_back();
//
//
//            //If any does, remove the ones that do not meet them from the candidates vector
//        } else {
//            candidates.erase(std::remove_if(candidates.begin(), candidates.end(),
//                                            [](Individual *candidate) { return !candidate->isViableBreeder(); }),
//                             candidates.end());
//
//
//            //  Choose new mainBreeder
//            //      Choose mainBreeder with higher likelihood for the highest rank
//            vector<Individual *, std::allocator<Individual *>>::iterator candidate;
//            for (candidate = candidates.begin(); candidate < candidates.end(); ++candidate) {
//                sumRank += (*candidate)->getAge(); //add all the ranks from the vector candidates
//            }
//
//            for (candidate = candidates.begin(); candidate < candidates.end(); ++candidate) {
//                position.push_back(static_cast<double>((*candidate)->getAge()) / static_cast<double>(sumRank) +
//                                   currentPosition); //creates a vector with proportional segments to the rank of each individual
//                currentPosition = position[position.size() - 1];
//            }
//
//
//            //      Make the chosen candidate the new mainBreeder
//            for (candidate = candidates.begin(); candidate != candidates.end(); ++candidate) {
//                if (RandP < position[candidate - candidates.begin()]) { //chooses the candidate with higher age
//                    mainBreeder = **candidate; //substitute the previous dead mainBreeder
//                    mainBreederAlive = true;
//                    mainBreeder.setAgeBecomeBreeder(mainBreeder.getAge());
//                    mainBreeder.setFishType(BREEDER); //modify the class
//
//                    if ((*candidate)->isInherit() == 0) //delete the ind from the vector floaters
//                    {
//                        newBreederOutsider++;
//                    } else {
//                        newBreederInsider++;
//                        inheritance++; //TODO: At the moment, newBreederInsider and inheritance are equivalent
//                    }
//
//                    **candidate = helpers[helpers.size() - 1]; //delete the ind from the vector helpers
//                    helpers.pop_back();
//
//                    break;
//                }
//            }
//        }
//        assert(mainBreeder.getFishType() == BREEDER);
//    }


void Group::transferBreedersToHelpers() {
    // Move breeders to the helper vector
    for (auto &breeder: breeders) {
        // Change the fish type of the breeder to helper
        breeder.setFishType(HELPER);
        // Add the breeder to the helpers vector
        helpers.emplace_back(breeder);
    }
    // Clear the breeders vector
    breeders.clear();

    // Move the main breeder also to the helper vector
    if (mainBreederAlive) { //TODO: This assumes that the main breeder is chosen again every round, change?
        // Change the fish type of the mainBreeder to helper
        mainBreeder.setFishType(HELPER);
        // Add the mainBreeder to the helpers vector
        helpers.emplace_back(mainBreeder);
        // Set mainBreederAlive to false as mainBreeder is no longer a breeder
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

    if (mainBreederAlive) {
        mainBreeder.increaseAge(mainBreederAlive);
    }
}


/* REPRODUCTION */

void Group::reproduce(int generation) { // populate offspring generation
    //Calculate fecundity

    fecundity = parameters->getK0() + parameters->getKh() * cumHelp / (1 + cumHelp);

    poisson_distribution<int> PoissonFecundity(fecundity);
    realFecundity = PoissonFecundity(*parameters->getGenerator()); //integer number

    //Reproduction
    if (mainBreederAlive) {
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
    return mainBreederAlive;
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

Individual *Group::selectBreeder(int &newBreederOutsider, int &newBreederInsider, int &inheritance) {
    double sumRank = 0;
    double currentPosition = 0; //age of the previous ind taken from Candidates
    double RandP = parameters->uniform(*parameters->getGenerator());
    vector<Individual *> candidates;
    vector<double> position; //vector of age to choose with higher likelihood the ind with higher age
    int anyViableCandidate = 0;


    Individual *selectedBreeder = nullptr;

    if (helpers.empty()) {
        return nullptr;
    } else {
        //    Join the helpers in the group to the vector candidates
        for (auto &helper: helpers) {
            candidates.push_back(&helper);
            anyViableCandidate += helper.isViableBreeder();
        }

        //  Check if the candidates meet the age requirements
        //If none do, take a random candidate
        if (anyViableCandidate == 0) {
            std::shuffle(helpers.begin(), helpers.end(), *parameters->getGenerator());
            selectedBreeder = &helpers.back(); //substitute the previous dead mainBreeder
            selectedBreeder->setAgeBecomeBreeder();
            selectedBreeder->setFishType(BREEDER); //modify the class
            if (selectedBreeder->isInherit() == 0) //delete the ind from the vector floaters
            {
                newBreederOutsider++;
            } else {
                newBreederInsider++;
                inheritance++; //TODO: At the moment, newBreederInsider and inheritance are equivalent
            }
            helpers.pop_back();


            //If any does, remove the ones that do not meet them from the candidates vector
        } else {
            candidates.erase(std::remove_if(candidates.begin(), candidates.end(),
                                            [](Individual *candidate) { return !candidate->isViableBreeder(); }),
                             candidates.end());


            //  Choose new breeder
            //      Choose breeder with higher likelihood for the highest rank
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
                if (RandP < position[candidate - candidates.begin()]) { //chooses the candidate with higher age


                    selectedBreeder = *candidate; //substitute the previous dead mainBreeder

                    selectedBreeder->setAgeBecomeBreeder();
                    selectedBreeder->setFishType(BREEDER); //modify the class

                    if ((*candidate)->isInherit() == 0) //delete the ind from the vector floaters
                    {
                        newBreederOutsider++;
                    } else {
                        newBreederInsider++;
                        inheritance++; //TODO: At the moment, newBreederInsider and inheritance are equivalent
                    }


                    // Find the iterator pointing to the selected breeder
                    auto it = std::find(helpers.begin(), helpers.end(), *selectedBreeder);

                    // Use std::rotate to move the selected breeder to the end of the vector
                    std::rotate(it, it + 1, helpers.end());


                    selectedBreeder = &helpers.back();
                    helpers.pop_back();
                    break;
                }
            }

        }

        if (selectedBreeder->getFishType() != BREEDER) {
            cout << "Selected breeder is not a breeder" << endl;
            for (auto &helper: helpers) {
                cout << "Helper age: " << helper.getAge() << endl;

            }

            cout << "break" << endl;
        }
        assert(selectedBreeder->getFishType() == BREEDER);

    }

    return selectedBreeder;
}

void Group::reassignBreeders(int &newBreederOutsider, int &newBreederInsider, int &inheritance) {

    this->transferBreedersToHelpers();

    if (!helpers.empty()) {

        //select main breeder
        auto selectedBreeder = selectBreeder(newBreederOutsider, newBreederInsider, inheritance);

        if (selectedBreeder != nullptr) {
            mainBreeder = *selectedBreeder;
            mainBreederAlive = true;
        } else {
            mainBreederAlive = false;
        }


        //select subordinate breeders
        int reproductiveShare = round(getReproductiveShare() * helpers.size());

        for (int i = 0; i < reproductiveShare; i++) {

            selectedBreeder = selectBreeder(newBreederOutsider, newBreederInsider, inheritance);
            if (selectedBreeder != nullptr) {
                breeders.emplace_back(*selectedBreeder);
            }
        }

    }

}

void Group::mortalityGroupVector(int &deaths, IndividualVector &individuals) {
    vector<Individual, std::allocator<Individual>>::iterator individualsIt;
    individualsIt = individuals.begin();
    int size = individuals.size();
    int counting = 0;
    while (!individuals.empty() && size > counting) {

        //Mortality of individuals
        if (parameters->uniform(*parameters->getGenerator()) > individualsIt->getSurvival()) {
            *individualsIt = individuals[individuals.size() - 1];
            individuals.pop_back();
            counting++;
            deaths++;
        } else
            individualsIt++, counting++; //go to next individual
    }
}




