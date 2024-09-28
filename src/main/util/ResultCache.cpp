

#include "ResultCache.h"
#include <iostream>
#include <sstream>

using namespace std;


void ResultCache::writeToCacheMain(const string &textLine) {
    this->mainCache.push_back(textLine);
}


void ResultCache::writeToCacheLastGeneration(Simulation *simulation, const Population &populationObj) {

    int groupID = 0;
    int counter = 0;

    for (auto const &group: populationObj.getGroups()) {
        if (counter < 100) {
            this->writeToCacheIndividual(group.getMainBreeder(), simulation->getGeneration(), groupID);

            for (auto const &helper: group.getHelpers()) {
                this->writeToCacheIndividual(helper, simulation->getGeneration(), groupID);
            }
            counter++;
        }
        groupID++;
    }
    for (auto const &floater: populationObj.getFloaters()) {
        this->writeToCacheIndividual(floater, simulation->getGeneration(), groupID);
    }


}


void ResultCache::writeToCacheIndividual(Individual individual, int generation, int groupID) {
    std::ostringstream oss;
    oss << fixed << showpoint
        << parameters->getReplica() + 1
        << "\t" << generation
        << "\t" << groupID
        << "\t" << individual.getFishType()
        << "\t" << setprecision(4) << individual.getAge()
        << "\t" << setprecision(4) << individual.getAlpha()
        << "\t" << setprecision(4) << individual.getBeta()
        << "\t" << setprecision(4) << individual.getGamma()
        << "\t" << setprecision(4) << individual.getDelta()
        << "\t" << setprecision(4) << individual.getDrift()
        << "\t" << setprecision(4) << individual.getDispersal()
        << "\t" << setprecision(4) << individual.getHelp()
        << "\t" << setprecision(4) << individual.getSurvival()
        << "\t" << setprecision(4) << individual.isInherit();
    this->lastGenerationCache.push_back(oss.str());
}


const vector<string> &ResultCache::getLastGenerationCache() const {
    return lastGenerationCache;
}

const vector<string> &ResultCache::getMainCache() const {
    return mainCache;
}

#include "ResultCache.h"
