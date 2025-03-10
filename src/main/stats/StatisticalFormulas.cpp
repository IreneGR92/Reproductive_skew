
#include "StatisticalFormulas.h"
#include "../util/Parameters.h"
#include <cmath>
#include <numeric>
#include <algorithm>
#include <cassert>


double StatisticalFormulas::calculateMean() {

    double sum = std::accumulate(individualValues.begin(), individualValues.end(), 0.0);
    double counter = individualValues.size();

    if (counter > 0) {
        return sum / counter;
    } else {
        return 0; //TODO: or -1?
    }
}

double StatisticalFormulas::calculateSD() {
    double mean = this->calculateMean();
    double stdev;

    std::vector<double> diff(size());
    std::transform(individualValues.begin(), individualValues.end(), diff.begin(),
                   [mean](double x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    if (!individualValues.empty()) {
        stdev = std::sqrt(sq_sum / individualValues.size());
    } else {
        stdev = 0;
    }
    return stdev;
}


double StatisticalFormulas::correlation(StatisticalFormulas y) {

    double meanX = this->calculateMean();
    double meanY = y.calculateMean();
    double SD_X = this->calculateSD();
    double SD_Y = y.calculateSD();
    double Xi, Yi, X, Y;
    double sumProductXY = 0.0;
    double counter = this->size();
    assert(size() == y.size());
    double correlation;

    for (int i = 0; i < counter; i++) {
        Xi = this->getValues().at(i);
        Yi = y.getValues().at(i);
        X = (Xi - meanX);
        Y = (Yi - meanY);
        sumProductXY += X * Y;
    }

    if (SD_X * SD_Y * counter == 0) {
        correlation = 0;
    } else {
        correlation = sumProductXY / (SD_X * SD_Y * counter);
    }

    assert(abs(correlation) <= 1 && "[ERROR] correlation out of range");

    return correlation;
}

template <typename GetIndividualsFunc>
double calculateRelatedness(const std::vector<Group> &groups, GetIndividualsFunc getIndividuals) {
    double correlation;
    int counter = 0;
    double meanX = 0, meanY = 0, stdevX = 0, stdevY = 0, sumX = 0.0, sumY = 0.0;
    double sumProductXY = 0, sumProductXX = 0, sumProductYY = 0;

    // Calculate sums and means
    for (const Group &group: groups) {
        if (group.isBreederAlive()) {
            auto individuals = getIndividuals(group);
            if (!individuals.empty()) {
                double mainBreederDrift = group.getMainBreeder().getDrift();
                for (const Individual &individual: individuals) {
                    sumX += individual.getDrift();
                    sumY += mainBreederDrift;
                    counter++;
                }
            }
        }
    }

    if (counter != 0) {
        meanX = sumX / counter;
        meanY = sumY / counter;
    }

    // Calculate products for standard deviation and correlation
    for (const Group &group: groups) {
        if (group.isBreederAlive()) {
            auto individuals = getIndividuals(group);
            if (!individuals.empty()) {
                double mainBreederDrift = group.getMainBreeder().getDrift();
                for (const Individual &individual: individuals) {
                    double X = (individual.getDrift() - meanX);
                    double Y = (mainBreederDrift - meanY);

                    sumProductXY += X * Y;
                    sumProductXX += X * X;
                    sumProductYY += Y * Y;
                }
            }
        }
    }

    if (counter != 0) {
        stdevX = sqrt(sumProductXX / counter);
        stdevY = sqrt(sumProductYY / counter);
    }

    if (stdevX * stdevY * counter == 0) {
        correlation = 999; // TODO: Interpret as NA in R code
    } else {
        correlation = sumProductXY / (stdevX * stdevY * counter);
    }

    return correlation;
}

double StatisticalFormulas::calculateRelatednessHelpers(const std::vector<Group> &groups) {
    return calculateRelatedness(groups, [](const Group &group) { return group.getHelpers(); });
}

double StatisticalFormulas::calculateRelatednessBreeders(const std::vector<Group> &groups) {
    return calculateRelatedness(groups, [](const Group &group) { return group.getSubordinateBreeders(); });
}


int StatisticalFormulas::getMaxValue() {
    int max = *max_element(individualValues.begin(), individualValues.end());
    return max;
}


void StatisticalFormulas::addValue(double toAdd) {
    this->individualValues.push_back(toAdd);
}

void StatisticalFormulas::merge(const StatisticalFormulas statisticalFormulas) {

    std::vector<double> result(this->individualValues.size() + statisticalFormulas.getValues().size());

    std::merge(this->individualValues.begin(), this->individualValues.end(), statisticalFormulas.getValues().begin(),
               statisticalFormulas.getValues().end(), result.begin());
    this->individualValues = result;


}

int StatisticalFormulas::size() {
    return this->individualValues.size();
}


void StatisticalFormulas::addValues(const std::vector<double> &values) {
    for (double value: values) {
        if (value != Parameters::NO_VALUE) {
            this->addValue(value);
        }
    }
}

std::vector<double> StatisticalFormulas::getValues() const {
    return this->individualValues;
}



