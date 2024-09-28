
#include <algorithm>
#include <cassert>
#include <vector>
#include "Group.h"

using namespace std;


Group::Group(Parameters *parameters) : mainBreeder(BREEDER, parameters), parameters(parameters) {
    mainBreederAlive = true;
    cumHelp = 0;
    acceptanceRate = Parameters::NO_VALUE;
    acceptedFloatersSize = Parameters::NO_VALUE;
    reproductiveShareRate = Parameters::NO_VALUE;
    fecundityGroup = Parameters::NO_VALUE;
    offspringMainBreeder = 0;
    offspringSubordinateBreeders = 0;
    totalOffspringGroup = 0;
    groupSize = 0;

    for (int i = 0; i < parameters->getInitNumHelpers(); ++i) {
        auto individual = Individual(HELPER, parameters);
        helpers.emplace_back(individual);
    }

    this->calculateGroupSize();
}


/* TOTAL NUMBER OF INDIVIDUALS PER GROUP*/

void Group::calculateGroupSize() {
    if (mainBreederAlive) {
        groupSize = helpers.size() + subordinateBreeders.size() + 1;
    } else {
        groupSize = helpers.size() + subordinateBreeders.size();
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

int Group::countHelpersAgeOne() {
    int count = 0;
    for (Individual &helper: helpers) {
        if (helper.getAge() == 1) {
            count++;
        }
    }
    return count;
}

int Group::calculateHelpersToReassign() {
    int helpersToReassign;
    if (parameters->isNoRelatedness()) {
        helpersToReassign = countHelpersAgeOne();

    } else if (parameters->getReducedRelatedness() == 3) {

        helpersToReassign = round(countHelpersAgeOne() / 3);

    } else if (parameters->getReducedRelatedness() == 2) {
        double value = static_cast<double>(countHelpersAgeOne()) / 2;
        if (value != floor(value)) { // Check if the value is not an integer
            if (parameters->uniform(*parameters->getGenerator()) < 0.5) {
                helpersToReassign = floor(value);
            } else {
                helpersToReassign = ceil(value);
            }
        } else {
            helpersToReassign = value;// If the value is an integer, just assign it normally
        }
    } else {
        helpersToReassign = 0; // Any other value than 2 or 3 of reduced relatedness will not reassign helpers
    }
    return helpersToReassign;
}


std::vector<Individual> Group::noRelatedHelpersToReassign(int index) {

    std::vector<Individual> noRelatedHelpers;

    //Obtain the number of helpers to reassign
    int helpersToReassign = calculateHelpersToReassign();

    //Reassign the helpers
    for (int i = 0; i < helpersToReassign; i++) {
        Individual *helper = &helpers.back(); // since offspring are added at the end of the helper vector, access last helper
        //helper->setInherit(false); //the location of the individual is not the natal territory //TODO: consider reassigned helpers insiders/outsiders?
        noRelatedHelpers.push_back(*helper); //add the individual to the vector in the last position
        helper->setGroupIndex(index);
        assert(helper->getAge() == 1);
        helpers.pop_back(); // Remove the last helper from the helpers vector
    }
    return noRelatedHelpers;
}

/*  ACCEPTANCE OF IMMIGRANTS */

void Group::calcAcceptanceRate() {

    this->transferBreedersToHelpers();

    double expulsionEffort = 0;
    double gamma;
    double counter = 0;

    if (helpers.empty()) {
        acceptanceRate = 1; //if no group members alive, all immigrants are free to colonise the territory
    } else {
        for (auto &helper: helpers) {
            if (helper.getGamma() < 0) {
                gamma = 0;
            } else if (helper.getGamma() > 1) {
                gamma = 1;
            } else {
                gamma = helper.getGamma();
            }
            expulsionEffort += gamma;
            counter++;
        }

        double meanExpulsionEffort = expulsionEffort / counter;

        acceptanceRate = 1 - meanExpulsionEffort;
        if (acceptanceRate < 0) { acceptanceRate = 0; }
    }
}

// Calculates the proportion of floaters that should be considered for immigration into the current group, based on the biasFloatBreeder parameter, the total number of colonies and the acceptance rate of the group.
std::vector<Individual> Group::getAcceptedFloaters(IndividualVector &floaters) {

// Shuffle the floaters vector
    std::shuffle(floaters.begin(), floaters.end(), *parameters->getGenerator());

// Take a sample of floaters based on biasFloatBreeder
    int numSampledFloaters = parameters->getFloatersSampledImmigration();
    if (numSampledFloaters > floaters.size()) {
        numSampledFloaters = round(floaters.size() / parameters->getMaxColonies());
    }
    std::vector<Individual> sampleFloaters(floaters.begin(), floaters.begin() + numSampledFloaters);

// Calculate the number of floaters that should be accepted by the group
    this->calcAcceptanceRate();
    acceptedFloatersSize = round(sampleFloaters.size() * acceptanceRate);

// Take a subsample of floaters based on the acceptance rate of the group
    std::vector<Individual> acceptedFloaters(sampleFloaters.begin(), sampleFloaters.begin() + acceptedFloatersSize);
    // Remove the selected floaters from the original floaters vector
    floaters.erase(floaters.begin(), floaters.begin() + acceptedFloatersSize);

    return acceptedFloaters;
}

void Group::transferBreedersToHelpers() {
    // Move breeders to the helper vector
    for (auto &breeder: subordinateBreeders) {
        // Change the fish type of the breeder to helper
        breeder.setFishType(HELPER);
        // Add the breeder to the helpers vector
        helpers.emplace_back(breeder);
    }
    // Clear the breeders vector
    subordinateBreeders.clear();

    // Move the main breeder also to the helper vector
    if (mainBreederAlive) { //TODO: This assumes that the main breeder is chosen again every round, change?
        // Change the fish type of the mainBreeder to helper
        mainBreeder.setFishType(HELPER);
        // Add the mainBreeder to the helpers vector
        helpers.emplace_back(mainBreeder);
        // Set mainBreederAlive to false as mainBreeder is no longer a breeder
        mainBreederAlive = false;
    }
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
    double delta;

    if (mainBreeder.getDelta() < 0) {
        delta = 0;
    } else {
        delta = mainBreeder.getDelta();
    };


    //Calculate survival for the helpers
    for (Individual &helper: helpers) {
        helper.calcSurvival(groupSize, 0);
    }

    //Calculate the survival for the subordinate breeders
    for (Individual &breeder: subordinateBreeders) {
        breeder.calcSurvival(groupSize, 0);
    }

    //Calculate the survival of the dominant breeder
    this->mainBreeder.calcSurvival(groupSize, delta);
}

void Group::mortalityGroup(int &deaths) {

    //Mortality helpers
    this->mortalityGroupVector(deaths, helpers);

    //Mortality subordinate breeders
    this->mortalityGroupVector(deaths, subordinateBreeders);

    //Mortality mainBreeder
    if (mainBreederAlive && parameters->uniform(*parameters->getGenerator()) > mainBreeder.getSurvival()) {
        mainBreederAlive = false;
        deaths++;
    }
    this->calculateGroupSize(); //update group size after mortality
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


/* BECOME BREEDER */

void Group::reassignBreeders(int &newBreederOutsider, int &newBreederInsider, int &inheritance) {

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
        this->calcReproductiveShareRate();
        int reproductiveShare = round(reproductiveShareRate * helpers.size());

        for (int i = 0; i < reproductiveShare; i++) {

            selectedBreeder = selectBreeder(newBreederOutsider, newBreederInsider, inheritance);
            if (selectedBreeder != nullptr) {
                subordinateBreeders.emplace_back(*selectedBreeder);
            }
        }
    } else {
        this->calcReproductiveShareRate();
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
            if (selectedBreeder->isInherit() == false) {
                newBreederOutsider++;
            } else {
                newBreederInsider++;
                inheritance++; //TODO: At the moment, newBreederInsider and inheritance are equivalent
            }
            helpers.pop_back();


            //If any does, remove the ones that do not meet them from the candidates vector
        } else {
            // remove non-viable candidates
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


            //      Make the chosen candidate the new breeder
            for (candidate = candidates.begin(); candidate != candidates.end(); ++candidate) {
                if (RandP < position[candidate - candidates.begin()]) { //chooses the candidate with higher age


                    selectedBreeder = *candidate;
                    selectedBreeder->setAgeBecomeBreeder();
                    selectedBreeder->setFishType(BREEDER); //modify the class

                    if ((*candidate)->isInherit() == false) {
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

        assert(selectedBreeder->getFishType() == BREEDER);
    }
    return selectedBreeder;
}

void Group::calcReproductiveShareRate() {
    if (mainBreederAlive) {
        reproductiveShareRate = parameters->getY() - mainBreeder.getDelta();
        if (reproductiveShareRate < 0) {
            reproductiveShareRate = 0;
        } else if (reproductiveShareRate > parameters->getY()) {
            reproductiveShareRate = parameters->getY();
        }
    } else {
        reproductiveShareRate = Parameters::NO_VALUE;
    }
}

/* INCREASE AGE OF ALL GROUP INDIVIDUALS*/

void Group::increaseAge() {
    for (Individual &helper: helpers) {
        helper.increaseAge();
    }

    for (Individual &breeder: subordinateBreeders) {
        breeder.increaseAge();
    }

    if (mainBreederAlive) {
        mainBreeder.increaseAge(mainBreederAlive);
    }
}


/* REPRODUCTION */

void Group::calcFecundity(double mk) {

    assert (cumHelp >= 0);
    double initFecundity; //TODO: we store the actual fecundity of the group, no the calculated one, issue for debugging?

    if (getBreedersSize() > 0) {
        //Calculate fecundity
        if (mk > 1 && parameters->isBetHedgingHelp()) { //TODO: Benign environment counted as 1 instead of mOff, change?
            initFecundity = mk * (parameters->getK0() - parameters->getKh() * cumHelp / (1 + cumHelp) +
                                  parameters->getKnb() * subordinateBreeders.size() / (1 + subordinateBreeders.size()));
        } else if (parameters->isHelpObligatory()){
            initFecundity = mk * parameters->getK0() + mk * (parameters->getKh() * cumHelp / (1 + cumHelp)) *
                    (1 + (parameters->getKnb() * subordinateBreeders.size() / (1 + subordinateBreeders.size())));
        } else {
            initFecundity = mk * (parameters->getK0() + parameters->getKh() * cumHelp / (1 + cumHelp) +
                                  parameters->getKnb() * subordinateBreeders.size() / (1 + subordinateBreeders.size()));
        }

        if (initFecundity < 0) {
            initFecundity = 0;
        }

        // Transform fecundity to an integer number
        std::poisson_distribution<int> PoissonFecundity(initFecundity);
        fecundityGroup = PoissonFecundity(*parameters->getGenerator()); //integer number
    } else {
        fecundityGroup = 0;
    }
}


void Group::reproduce(int generation, double mk) { // populate offspring generation

    std::vector<Individual *> breedersPointers;
    int randomIndex;
    this->calcFecundity(mk);
    offspringMainBreeder = 0;
    offspringSubordinateBreeders = 0;

    for (Individual &breeder: subordinateBreeders) {
        breedersPointers.push_back(&breeder);
    }
    if (mainBreederAlive) {
        breedersPointers.push_back(&mainBreeder);
    }

    std::uniform_int_distribution<int> distribution(0, breedersPointers.size() - 1);
    if (!breedersPointers.empty()) {
        for (int i = 0; i < fecundityGroup; i++) {
            // Generate a random index
            randomIndex = distribution(*parameters->getGenerator());
            // Access the random individual
            Individual *randomIndividual = breedersPointers[randomIndex];
            //Reproduction
            Individual offspring = Individual(*randomIndividual, HELPER, generation);
            helpers.emplace_back(offspring);
            if (randomIndex == breedersPointers.size() - 1) {
                offspringMainBreeder++;
            } else {
                offspringSubordinateBreeders++;
            }
        }
    }
}


/* GETTERS */

const Individual &Group::getMainBreeder() const {
    return mainBreeder;
}

bool Group::isBreederAlive() const {
    return mainBreederAlive;
}

int Group::getGroupSize() const {
    return groupSize;
}

int Group::getBreedersSize() const {
    if (mainBreederAlive) {
        return subordinateBreeders.size() + 1;
    } else {
        return subordinateBreeders.size();
    }
}

double Group::getCumHelp() const {
    return cumHelp;
}

double Group::getAcceptanceRate() const {
    return acceptanceRate;
}

double Group::getReproductiveShareRate() const {
    return reproductiveShareRate;
}

double Group::getFecundityGroup() const {
    return fecundityGroup;
}

int Group::getOffspringMainBreeder() const {
    return offspringMainBreeder;
}

int Group::getOffspringSubordinateBreeders() const {
    return offspringSubordinateBreeders;
}

int Group::getTotalOffspringGroup() const {
    return totalOffspringGroup;
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

bool Group::hasSubordinateBreeders() const {
    return !subordinateBreeders.empty();
}

void Group::addHelper(Individual &helper) {
    helper.setFishType(HELPER);
    this->helpers.emplace_back(helper);
}

const IndividualVector &Group::getHelpers() const {
    return helpers;
}

const IndividualVector &Group::getSubordinateBreeders() const {
    return subordinateBreeders;
}

void Group::addHelpers(vector<Individual> &helpers) {
    for (Individual &helper: helpers) {
        this->addHelper(helper);
    }
}









