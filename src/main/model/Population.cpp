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
    this->groupColonization = 0;
}

Population::Population(const std::shared_ptr<Parameters> &parameters) : parameters(parameters), deaths(0),
                                                                        groupColonization(0),
                                                                        newBreederOutsider(0),
                                                                        newBreederInsider(0),
                                                                        inheritance(0),
                                                                        emigrants(0), mk(0) {
    for (int i = 0; i < parameters->getMaxColonies(); i++) {
        Group group(parameters);
        this->groups.emplace_back(group);
    }
}

void Population::disperse() {
    for (auto &group: groups) {
        this->floaters.merge(group.disperse());
    }
    this->emigrants = floaters.size();
    // After all floater are created, the number of emigrants is set to the number of floaters.

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
                        *parameters->getGenerator()); // selects a random index the noRelatednessGroupsID vector
            }
            selectGroupID = noRelatednessGroupsID[selectGroupIndex];
            // translates the index to the ID of a group from the noRelatednessGroupsID vector

            auto indexLastIndividual = allNoRelatedHelpers.size() - 1;

            if (selectGroupID != allNoRelatedHelpers[indexLastIndividual].getGroupIndex() || timeout > 5000) {
                noRelatednessGroupsID.erase(noRelatednessGroupsID.begin() +
                                            selectGroupIndex);
                //remove the group ID from the vector to not draw it again
                groups[selectGroupID].addHelper(
                        allNoRelatedHelpers[indexLastIndividual]);
                //add the no related helper to the helper vector in a randomly selected group
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
    std::shuffle(indices.begin(), indices.end(), *parameters->getGenerator());

    if (!floaters.empty()) {
        // Loop through the groups in a random order
        for (int i: indices) {

            Group &group = groups[i];

            // Check if the group is empty, if so, floaters are recolonizing the territory
            if (!group.isBreederAlive() && group.getHelpers().empty() && group.getSubordinateBreeders().empty()) {
                groupColonization++;
            }

            // Add new helpers to the group
            group.moveAcceptedFloaters(floaters);
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
        floater.calcSurvival(0, 0, false);
    }
}

void Population::mortalityGroup() {
    for (Group &group: groups) {
        group.mortalityGroup(deaths);
    }
}

void Population::mortalityFloaters() {
    auto floaterIt = floaters.begin();
    int size = floaters.size();
    for (int count = 0; !floaters.empty() && size > count; count++) {
        //Mortality floaters
        if (parameters->uniform(*parameters->getGenerator()) > floaterIt->getSurvival()) {
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
    auto rng = *parameters->getGenerator();
    std::normal_distribution<double> NormalDist(0, parameters->getMMagnit());


    double offspringSurvival = parameters->getMOff();

    //predictable environment
    if (parameters->isPredictableEnvironment()) {
        conditionCheckCounter++;
        int interval = static_cast<int>(1.0 / parameters->getMFreq());
        if (changeCounter < (conditionCheckCounter / interval)) {
            double modification = abs(NormalDist(rng));
            if (negativeModification) {
                modification = -modification;
            }
            offspringSurvival += modification;
            changeCounter++;
            negativeModification = !negativeModification; // Toggle the flag
        }


        //unpredictable environment
    } else if (parameters->uniform(rng) < parameters->getMFreq()) {
        offspringSurvival += NormalDist(rng);
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

int Population::getGroupColonization() const {
    return groupColonization;
}
