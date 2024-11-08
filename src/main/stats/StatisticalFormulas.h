#ifndef GROUP_AUGMENTATION_STATISTICALFORMULAS_H
#define GROUP_AUGMENTATION_STATISTICALFORMULAS_H

#include<vector>
#include "../model/Group.h"

/**
 * @class StatisticalFormulas
 * @brief A class that provides methods for calculating various statistical measures.
 *
 * This class maintains a vector of double values and provides methods to calculate statistical measures such as mean, standard deviation, correlation, and others.
 */
class StatisticalFormulas {

    std::vector<double> individualValues; ///< The vector of values.

public:

    void addValue(double toAdd);

    void addValues(const std::vector<double> &values);

    double calculateMean();

    double calculateSD();

    double correlation(StatisticalFormulas y);

    double calculateRelatednessHelpers(const std::vector<Group> &groups);

    double calculateRelatednessBreeders(const std::vector<Group> &groups);

    int getMaxValue();

    void merge(StatisticalFormulas statisticalFormulas);

    int size();

    std::vector<double> getValues() const;

};


#endif //GROUP_AUGMENTATION_STATISTICALFORMULAS_H