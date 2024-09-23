#include "Population.h"
#include <vector>

const std::vector<Group> &Population::getGroups() const {
    return groups;
}

const IndividualVector &Population::getFloaters() const {
    return floaters;
}

int Population::getDeaths() const {
    return deaths;
}

int Population::getEmigrants() const {
    return emigrants;
}

void Population::reset() {
    this->deaths = 0;
    this->inheritance = 0;
    this->newBreederInsider = 0;
    this->newBreederOutsider = 0;
    this->emigrants = 0;
}

Population::Population(std::default_random_engine *rng) : rng(rng) {
    for (int i = 0; i < Parameters::instance()->getMaxColonies(); i++) {
        Group group(rng);
        this->groups.emplace_back(group);
    }
}

void Population::disperse() {

    for (int i = 0; i < groups.size(); i++) {
        Group &group = groups[i];
        this->floaters.merge(group.disperse());
    }
    this->emigrants = floaters.size(); // After all floater are created, the number of emigrants is set to the number of floaters.

    this->reassignNoRelatedHelpers();
}

void Population::reassignNoRelatedHelpers() {

    int groupID = 0;

    std::vector<Individual> noRelatedHelpers;
    IndividualVector allNoRelatedHelpers;
    std::vector<int> noRelatednessGroupsID;

    // Helpers just born are reassigned to random groups. Groups receive as many helpers as helpers left the group for reassignment.
    for (int i = 0; i < groups.size(); i++) {
        Group &group = groups[i];

        noRelatedHelpers = group.noRelatedHelpersToReassign(i);
        for (int j = 0; j < noRelatedHelpers.size(); j++) {
            noRelatednessGroupsID.push_back(groupID);
        }
        allNoRelatedHelpers.merge(noRelatedHelpers);
        groupID++;
    }

    // Assign helpers to random group while maintaining the same group size
    if (!allNoRelatedHelpers.empty()) {

        int selectGroupID;
        int timeout = 0;
        while (!allNoRelatedHelpers.empty()) {
            int selectGroupIndex = 0;
            if (noRelatednessGroupsID.size() > 1) {
                std::uniform_int_distribution<int> uniformIntDistribution(0, noRelatednessGroupsID.size() - 1);
                selectGroupIndex = uniformIntDistribution(
                        *rng); // selects a random index the noRelatednessGroupsID vector
            }
            selectGroupID = noRelatednessGroupsID[selectGroupIndex]; // translates the index to the ID of a group from the noRelatednessGroupsID vector

            auto indexLastIndividual = allNoRelatedHelpers.size() - 1;

            if (selectGroupID != allNoRelatedHelpers[indexLastIndividual].getGroupIndex() || timeout > 5000) {
                noRelatednessGroupsID.erase(noRelatednessGroupsID.begin() +
                                            selectGroupIndex); //remove the group ID from the vector to not draw it again
                groups[selectGroupID].addHelper(
                        allNoRelatedHelpers[indexLastIndividual]); //add the no related helper to the helper vector in a randomly selected group
                allNoRelatedHelpers.pop_back(); //remove the no related helper from its vector

            } else {
                timeout++; //if not other group to put the helper than the original one, do it anyway
            }
        }
    }
}


void Population::immigrate() {

// Shuffle the group indices. This is done to ensure that the immigration process does not favor any particular group due to their position in the groups vector.
    std::vector<int> indices(groups.size());
    std::iota(indices.begin(), indices.end(), 0); // Fill it with consecutive numbers
    std::shuffle(indices.begin(), indices.end(), *rng);

// Loop through the groups in a random order
    if (!floaters.empty()) { // checks if there are any floaters available for immigration.
        for (int i: indices) {
            Group &group = groups[i];
            // Add new helpers to the group
            auto newHelpers = group.getAcceptedFloaters(
                    floaters); //  gets a list of floaters that are accepted by the current group.
            group.addHelpers(newHelpers);

        }
    }
}


void Population::help() {
    for (Group &group: groups) {
        //Calculate help & cumulative help for group
        group.calculateCumulativeHelp();
    }
}

void Population::survivalGroup() {
    for (Group &group: groups) {
        group.survivalGroup();
    }
}


void Population::survivalFloaters() {
    for (Individual &floater: floaters) {
        floater.calcSurvival(0, 0);
    }
}

void Population::mortalityGroup() {
    for (Group &group: groups) {
        group.mortalityGroup(deaths);
    }
}

void Population::mortalityFloaters() {

    std::vector<Individual, std::allocator<Individual>>::iterator floaterIt;
    floaterIt = floaters.begin();
    int size = floaters.size();
    for (int count = 0; !floaters.empty() && size > count; count++) {

        //Mortality floaters
        if (parameters->uniform(*rng) > floaterIt->getSurvival()) {
            *floaterIt = floaters[floaters.size() - 1];
            floaters.pop_back();
            deaths++;
        } else {
            floaterIt++;
        }
    }
}

void Population::reassignBreeder() {
    for (Group &group: groups) {
        group.reassignBreeders(newBreederOutsider, newBreederInsider, inheritance);
    }
}

void Population::increaseAge() {
    for (Group &group: groups) {
        group.increaseAge();
    }
    this->increaseAgeFloaters();
}

void Population::increaseAgeFloaters() {
    for (Individual &floater: floaters) {
        floater.increaseAge();
    }
}

double Population::getOffspringSurvival() {



    std::normal_distribution<double> NormalDist(0, parameters->getMStep());


    double offspringSurvival = parameters->getMOff();

    //predictable environment
    if (parameters->isPredictableEnvironment()) {
        conditionCheckCounter++;
        int interval = static_cast<int>(1.0 / parameters->getMRate());
        if (changeCounter < (conditionCheckCounter / interval)) {
            double modification = abs(NormalDist(*rng));
            if (negativeModification) {
                modification = -modification;
            }
            offspringSurvival += modification;
            changeCounter++;
            negativeModification = !negativeModification; // Toggle the flag
        }


        //unpredictable environment
    } else if (parameters->uniform(*rng) < parameters->getMRate()) {
        offspringSurvival += NormalDist(*rng);
    }

    if (offspringSurvival < 0) {
        offspringSurvival = 0;
    }

    return offspringSurvival;
}

void Population::reproduce(int generation) {
    this->mk = getOffspringSurvival();
    for (Group &group: groups) {
        group.reproduce(generation, mk);
    }
}

double Population::getMk() const {
    return mk;
}

int Population::getNewBreederOutsider() const {
    return newBreederOutsider;
}

int Population::getNewBreederInsider() const {
    return newBreederInsider;
}

int Population::getInheritance() const {
    return inheritance;
}



