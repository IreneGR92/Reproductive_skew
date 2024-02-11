#ifndef GROUP_AUGMENTATION_STATISTICALFORMULAS_H
#define GROUP_AUGMENTATION_STATISTICALFORMULAS_H

#include<vector>

/**
 * @class StatisticalFormulas
 * @brief A class that provides methods for calculating various statistical measures.
 *
 * This class maintains a vector of double values and provides methods to calculate statistical measures such as mean, standard deviation, correlation, and others.
 */
class StatisticalFormulas {

    std::vector<double> individualValues; ///< The vector of values.

public:

    /**
     * @brief Adds a value to the vector.
     * @param toAdd The value to add.
     */
    void addValue(double toAdd);

    /**
     * @brief Adds multiple values to the vector.
     * @param values The vector of values to add.
     */
    void addValues(const std::vector<double> &values);

    /**
     * @brief Calculates the mean of the values in the vector.
     * @return The mean of the values.
     */
    double calculateMean();

    /**
     * @brief Calculates the standard deviation of the values in the vector.
     * @return The standard deviation of the values.
     */
    double calculateSD();

    /**
     * @brief Calculates the correlation between the values in this vector and another vector.
     * @param y The other vector of values.
     * @return The correlation between the two vectors.
     */
    double correlation(StatisticalFormulas y);

    /**
     * @brief Returns the maximum value in the vector.
     * @return The maximum value.
     */
    int getMaxValue();

    /**
     * @brief Merges another StatisticalFormulas object into this object.
     * @param statisticalFormulas The other StatisticalFormulas object.
     */
    void merge(StatisticalFormulas statisticalFormulas);

    /**
     * @brief Returns the size of the vector.
     * @return The size of the vector.
     */
    int size();

    /**
     * @brief Returns the values in the vector.
     * @return The values in the vector.
     */
    std::vector<double> getValues() const;

};


#endif //GROUP_AUGMENTATION_STATISTICALFORMULAS_H