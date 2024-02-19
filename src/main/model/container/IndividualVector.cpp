#include <algorithm>
#include "IndividualVector.h"

using namespace std;

/**
 * @brief Get the attribute values of the individuals in the vector.
 *
 * @param type The attribute to get the values for.
 * @return std::vector<double> A vector containing the attribute values of the individuals.
 *
 * If the attribute type is DISPERSAL, only the attribute values of individuals with age 1 are returned.
 * For other attribute types, the attribute values of all individuals are returned.
 */
std::vector<double> IndividualVector::get(Attribute type) const {
    std::vector<double> result;
    for (const auto &it: *this) {

        if (type == Attribute::DISPERSAL) {
            if (it.getAge() == 1) {
                result.push_back(it.get(type));
            }
        } else {
            result.push_back(it.get(type));
        }
    }
    return result;
}

/**
 * @brief Merge another vector of individuals into this vector.
 *
 * @param vector The vector of individuals to be merged.
 */
void IndividualVector::merge(std::vector<Individual> vector) {
    this->insert(end(), vector.begin(), vector.end());
}

/**
 * @brief Remove an individual from the vector.
 *
 * @param itemIndex The index of the individual to be removed.
 *
 * The individual at the given index is swapped with the last individual in the vector, and then the last individual is removed.
 * This is a common technique to efficiently remove an item from a vector without preserving the order of the items.
 */
void IndividualVector::removeIndividual(size_t itemIndex) {
    std::swap(this->at(itemIndex), back());
    pop_back();
}