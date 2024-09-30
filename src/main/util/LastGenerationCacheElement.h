#ifndef LASTGENERATIONCACHEELEMENT_H
#define LASTGENERATIONCACHEELEMENT_H
#include "../model/Individual.h"


class LastGenerationCacheElement {
public:
    int groupID;
    int generation;
    Individual individual;

    LastGenerationCacheElement(int groupID, int generation, Individual individual) : groupID(groupID),
        generation(generation),
        individual(individual) {
    };
};


#endif //LASTGENERATIONCACHEELEMENT_H
