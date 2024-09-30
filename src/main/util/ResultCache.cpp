#include "ResultCache.h"
#include <iostream>
#include <sstream>

#include "LastGenerationCacheElement.h"

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
    auto element = LastGenerationCacheElement(groupID, generation, individual);
    this->lastGenerationCache.push_back(element);
}


const vector<LastGenerationCacheElement> &ResultCache::getLastGenerationCache() const {
    return lastGenerationCache;
}

const vector<string> &ResultCache::getMainCache() const {
    return mainCache;
}

#include "ResultCache.h"
