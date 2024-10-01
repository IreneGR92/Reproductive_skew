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


void ResultCache::clear() {
    this->lastGenerationCache.clear();
    this->mainCache.clear();

    this->lastGenerationCache.shrink_to_fit();
    this->mainCache.shrink_to_fit();
}

void ResultCache::writeToCacheIndividual(Individual individual, int generation, int groupID) {
    auto element = LastGenerationCacheElement(groupID, generation, individual);
    this->lastGenerationCache.push_back(element);
}

void ResultCache::writeToCacheMain(MainCacheElement element) {
    this->mainCache.push_back(element);
}


const vector<LastGenerationCacheElement> &ResultCache::getLastGenerationCache() const {
    return lastGenerationCache;
}

const vector<MainCacheElement> &ResultCache::getMainCache() const {
    return mainCache;
}

void ResultCache::writeToCacheMain(int generation, const Population &population, const Statistics &statistics) {
}
