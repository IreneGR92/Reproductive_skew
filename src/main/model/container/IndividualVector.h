#ifndef GROUP_AUGMENTATION_INDIVIDUALVECTOR_H
#define GROUP_AUGMENTATION_INDIVIDUALVECTOR_H

#include <vector>
#include <algorithm>
#include "../Individual.h"
#include "../Attribute.h"

/**
 * @class IndividualVector
 * @brief A class that represents a vector of Individual objects.
 *
 * This class extends std::vector<Individual> and provides additional methods to manipulate and access the individuals.
 */
struct IndividualVector : std::vector<Individual> {

    /**
     * @brief Gets the attribute values of the individuals in the vector.
     * @param attribute The attribute to get the values of.
     * @return A vector of attribute values.
     */
    std::vector<double> get(Attribute) const;

    /**
     * @brief Merges another vector of individuals into this vector.
     * @param vector The vector of individuals to merge.
     */
    void merge(std::vector<Individual> vector);

    /**
     * @brief Removes an individual from the vector.
     * @param itemIndex The index of the individual to remove.
     */
    void removeIndividual(size_t itemIndex);

};

#endif //GROUP_AUGMENTATION_INDIVIDUALVECTOR_H