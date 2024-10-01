#include "ResultCache.h"
#include <sstream>

#include "LastGenerationCacheElement.h"
#include "MainCacheElement.h"

using namespace std;


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
    this->lastGenerationCache.push(element);
}

void ResultCache::writeToCacheMain(MainCacheElement element) {
    this->mainCache.push(element);
}


const queue<LastGenerationCacheElement> &ResultCache::getLastGenerationCache() const {
    return lastGenerationCache;
}

const queue<MainCacheElement> &ResultCache::getMainCache() const {
    return mainCache;
}

void ResultCache::writeToCacheMain(int generation, const Population &population, const Statistics &statistics) {
}

const int ResultCache::getReplica() const {
    return replica;
}
