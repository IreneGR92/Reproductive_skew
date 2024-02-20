
#include <iostream>
#include <assert.h>
#include "Statistics.h"
#include "../model/Population.h"
#include "../model/Group.h"
#include "../model/Individual.h"

using namespace std;

/* CALCULATE STATISTICS */
void Statistics::calculateStatistics(const Population &populationObj) {

    //Counters
    population = 0, totalFloaters = 0, totalHelpers = 0, totalMainBreeders = 0, totalSubordinateBreeders = 0;

    //Relatedness
    relatedness = 0.0;


    IndividualVector mainBreeders;
    IndividualVector subordinateBreeders;
    IndividualVector allBreeders;
    IndividualVector helpers;
    IndividualVector individualsAll;
    std::vector<double> groupSizes;
    std::vector<double> numSubBreeders;
    std::vector<double> cumHelp;
    std::vector<double> acceptanceRates;
    std::vector<double> reproductiveShareRates;
    std::vector<double> offspringMainBreeders;
    std::vector<double> offspringSubordinateBreeders;
    std::vector<double> totalOffspringGroups;


    for (const Individual &helper: helpers) {
        if (helper.getFishType() != HELPER) {
            cout << "helper wrong class";
        }
    }

    for (const Individual &floater: populationObj.getFloaters()) {
        if (floater.getFishType() != FLOATER) {
            cout << "floater wrong class";
        }
    }

    for (const Individual &breeder: allBreeders) {
        if (breeder.getFishType() != BREEDER) {
            cout << "breeder wrong class";
        }
    }


    for (const Group &group: populationObj.getGroups()) {
        if (group.isBreederAlive()) {
            mainBreeders.push_back(group.getMainBreeder());
        }
        subordinateBreeders.merge(group.getSubordinateBreeders());
        helpers.merge(group.getHelpers());

        groupSizes.push_back(group.getGroupSize());
        numSubBreeders.push_back(group.getSubordinateBreeders().size());
        cumHelp.push_back(group.getCumHelp());
        acceptanceRates.push_back(group.getAcceptanceRate());
        reproductiveShareRates.push_back(group.getReproductiveShareRate());
        offspringMainBreeders.push_back(group.getOffspringMainBreeder());
        offspringSubordinateBreeders.push_back(group.getOffspringSubordinateBreeders());
        totalOffspringGroups.push_back(group.getTotalOffspringGroup());
    }

    individualsAll.merge(helpers);
    individualsAll.merge(populationObj.getFloaters());
    individualsAll.merge(mainBreeders);
    individualsAll.merge(subordinateBreeders);
    allBreeders.merge(mainBreeders);
    allBreeders.merge(subordinateBreeders);

    //Counters
    totalHelpers = helpers.size();
    totalFloaters = populationObj.getFloaters().size();
    totalMainBreeders = mainBreeders.size();
    totalSubordinateBreeders = subordinateBreeders.size();
    population = totalMainBreeders + totalSubordinateBreeders + totalHelpers + totalFloaters;
    assert(population > 0);

    //Initialize the stats

    //Genes
    alpha.addValues(individualsAll.get(ALPHA));
    alphaAge.addValues(individualsAll.get(ALPHA_AGE));
    beta.addValues(individualsAll.get(BETA));
    gamma.addValues(individualsAll.get(GAMMA));
    delta.addValues(individualsAll.get(DELTA));

    //Phenotypes
    age.addValues(individualsAll.get(AGE));
    ageDomBreeders.addValues(mainBreeders.get(AGE));
    ageSubBreeders.addValues(subordinateBreeders.get(AGE));
    ageHelpers.addValues(helpers.get(AGE));
    ageFloaters.addValues(populationObj.getFloaters().get(AGE));
    ageBecomeBreeder.addValues(allBreeders.get(AGE_BECOME_BREEDER));

    help.addValues(helpers.get(HELP));
    dispersal.addValues(helpers.get(DISPERSAL));
    fecundity.addValues(individualsAll.get(FECUNDITY)); //TODO: make only for breeders?

    survival.addValues(individualsAll.get(SURVIVAL));
    survivalDomBreeders.addValues(mainBreeders.get(SURVIVAL));
    survivalSubBreeders.addValues(subordinateBreeders.get(SURVIVAL));
    survivalHelpers.addValues(helpers.get(SURVIVAL));
    survivalFloaters.addValues(populationObj.getFloaters().get(SURVIVAL));


    //Group attributes
    groupSize.addValues(groupSizes);
    numOfSubBreeders.addValues(numSubBreeders);
    cumulativeHelp.addValues(cumHelp);
    acceptanceRate.addValues(acceptanceRates);
    reproductiveShareRate.addValues(reproductiveShareRates);
    offspringMainBreeder.addValues(offspringMainBreeders);
    offspringOfSubordinateBreeders.addValues(offspringSubordinateBreeders);
    totalOffspringGroup.addValues(totalOffspringGroups);



    //Correlations in different levels
    relatedness = calculateRelatedness(populationObj.getGroups());

}

double
Statistics::calculateRelatedness(const std::vector<Group> &groups) {

    //Relatedness
    double correlation;          //relatedness related
    int counter = 0;
    double meanX = 0, meanY = 0, stdevX = 0, stdevY = 0, sumX = 0.0, sumY = 0.0;
    double sumProductXY = 0, sumProductXX = 0, sumProductYY = 0;

    for (const Group &group: groups) {
        for (const Individual &helper: group.getHelpers()) {
            if (group.isBreederAlive() && group.hasHelpers()) {
                sumX += helper.getDrift();
                sumY += group.getMainBreeder().getDrift();
                counter++;
            }
        }
    }

    if (counter != 0) {
        meanX = sumX / counter;
        meanY = sumY / counter;
    }

    for (const Group &group: groups) {
        for (const Individual &helper: group.getHelpers()) {
            if (!std::isnan(helper.getDispersal()) || !std::isnan(helper.getHelp())) {
                double X = (helper.getDrift() - meanX);
                double Y = (group.getMainBreeder().getDrift() - meanY);

                sumProductXY += X * Y;
                sumProductXX += X * X;
                sumProductYY += Y * Y;
            }
        }
    }
    if (counter != 0) {
        stdevX = sqrt(sumProductXX / counter);
        stdevY = sqrt(sumProductYY / counter);
    }

    if (stdevX * stdevY * counter == 0) {
        correlation = 0;
    } else {
        correlation = sumProductXY / (stdevX * stdevY * counter);
    }
    //assert (abs(correlation) >= 0);
    //assert (abs(correlation) <= 1);
    return correlation;

}


/* PRINT STATISTICS TO CONSOLE */

void Statistics::printHeadersToConsole() {
    // column headings on screen
    cout << setw(6) << "gen" << setw(9) << "pop" << setw(9) << "deaths" << setw(9)
         << "float" << setw(9) << "group" << setw(9) << "maxGroup" << setw(9) << "subBreed" << setw(9)
         << "age" << setw(9)
         << "alpha" << setw(9) << "alphaAge" << setw(9) << "beta" << setw(9)
         << "gamma" << setw(9) << "delta" << setw(9)
         << "disper" << setw(9) << "immRate" << setw(9)
         << "help" << setw(9) << "surv" << setw(9)
         << "skew" << setw(9)<< "offsDom" << setw(9) << "offsSub" << setw(9)
         << "relat" << endl;
}


void Statistics::printToConsole(int generation, int deaths) {
    // show values on screen
    std::cout << fixed << showpoint
              << setw(6) << generation
              << setw(9) << population
              << setw(9) << deaths
              << setw(9) << totalFloaters
              << setw(9) << setprecision(2) << groupSize.calculateMean()
              << setw(9) << groupSize.getMaxValue()
              << setw(9) << numOfSubBreeders.calculateMean()
              << setw(9) << setprecision(2) << age.calculateMean()
              << setw(9) << setprecision(4) << alpha.calculateMean()
              << setw(9) << setprecision(4) << alphaAge.calculateMean()
              << setw(9) << setprecision(4) << beta.calculateMean()
              << setw(9) << setprecision(4) << gamma.calculateMean()
              << setw(9) << setprecision(4) << delta.calculateMean()
              << setw(9) << setprecision(4) << dispersal.calculateMean()
              << setw(9) << setprecision(2) << acceptanceRate.calculateMean()
              << setw(9) << setprecision(4) << help.calculateMean()
              << setw(9) << setprecision(2) << survival.calculateMean()
              << setw(9) << setprecision(2) << reproductiveShareRate.calculateMean()
              << setw(9) << setprecision(2) << offspringMainBreeder.calculateMean()
              << setw(9) << setprecision(2) << offspringOfSubordinateBreeders.calculateMean()
              << setw(9) << setprecision(2) << relatedness
              << endl;
}


/* PRINT STATISTICS TO FILE */

void Statistics::printHeadersToFile() {
    // column headings in output file main
    *parameters->getMainWriter() << "Replica" << "\t" << "Generation" << "\t" << "Population" << "\t"
                                 << "Deaths" << "\t" << "Floaters" << "\t" << "Group_size" << "\t" << "Sub_Breeders" << "\t"
                                 << "Age_H" << "\t" << "Age_F" << "\t" << "Age_DomB" << "\t" << "Age_SubB" << "\t" << "Age_New_Breeder" << "\t"
                                 << "Alpha" << "\t" << "AlphaAge" << "\t"
                                 << "Beta" << "\t" << "Gamma" << "\t" << "Delta" << "\t"
                                 << "Dispersal" << "\t" << "AcceptRate" << "\t"
                                 << "Help" << "\t" << "CumHelp" << "\t"
                                 << "Survival_H" << "\t" << "Survival_F" << "\t"<< "Survival_DomB" << "\t"<< "Survival_SubB" << "\t"
                                 << "ReprodShareRate" << "\t" << "OffspringDomBreeder" << "\t"
                                 << "OffspringSubBreeders" << "\t" << "TotalOffspringGroup" << "\t"
                                 << "Relatedness" << "\t"
                                 << "newBreederOutsider" << "\t" << "newBreederInsider" << "\t"
                                 //<< "inheritance"
                                 << endl;

    // column headings in output file last generation
    *parameters->getLastGenerationWriter() << "replica" << "\t" << "generation" << "\t" << "groupID"
                                           << "\t" << "type" << "\t" << "age" << "\t"
                                           << "alpha" << "\t" << "alphaAge" << "\t" << "beta" << "\t"
                                           << "gamma" << "\t" << "delta" << "\t" << "drift"<< "\t"
                                           << "dispersal" << "\t" << "help" << "\t" << "survival" << "\t"
                                           << "fecundity" << "\t" << "inherit" << endl;
}

void Statistics::printToFile(int replica, int generation, int deaths, int newBreederOutsider,
                             int newBreederInsider, int inheritance) {

    *parameters->getMainWriter() << fixed << showpoint
                                 << replica
                                 << "\t" << generation
                                 << "\t" << population
                                 << "\t" << deaths
                                 << "\t" << totalFloaters
                                 << "\t" << setprecision(4) << groupSize.calculateMean()
                                 << "\t" << setprecision(4) << numOfSubBreeders.calculateMean()
                                 << "\t" << setprecision(4) << ageHelpers.calculateMean()
                                 << "\t" << setprecision(4) << ageFloaters.calculateMean()
                                 << "\t" << setprecision(4) << ageDomBreeders.calculateMean()
                                 << "\t" << setprecision(4) << ageSubBreeders.calculateMean()
                                 << "\t" << setprecision(4) << ageBecomeBreeder.calculateMean()
                                 << "\t" << setprecision(4) << alpha.calculateMean()
                                 << "\t" << setprecision(4) << alphaAge.calculateMean()
                                 << "\t" << setprecision(4) << beta.calculateMean()
                                 << "\t" << setprecision(4) << gamma.calculateMean()
                                 << "\t" << setprecision(4) << delta.calculateMean()
                                 << "\t" << setprecision(4) << dispersal.calculateMean()
                                 << "\t" << setprecision(4) << acceptanceRate.calculateMean()
                                 << "\t" << setprecision(4) << help.calculateMean()
                                 << "\t" << setprecision(4) << cumulativeHelp.calculateMean()
                                 << "\t" << setprecision(4) << survivalHelpers.calculateMean()
                                 << "\t" << setprecision(4) << survivalFloaters.calculateMean()
                                 << "\t" << setprecision(4) << survivalDomBreeders.calculateMean()
                                 << "\t" << setprecision(4) << survivalSubBreeders.calculateMean()
                                 << "\t" << setprecision(4) << reproductiveShareRate.calculateMean()
                                 << "\t" << setprecision(4) << offspringMainBreeder.calculateMean()
                                 << "\t" << setprecision(4) << offspringOfSubordinateBreeders.calculateMean()
                                 << "\t" << setprecision(4) << totalOffspringGroup.calculateMean()
                                 << "\t" << setprecision(4) << relatedness
                                 << "\t" << newBreederOutsider
                                 << "\t" << newBreederInsider
                                 //<< "\t" << inheritance
                                 << endl;
}


void Statistics::printToFileLastGeneration(Simulation *simulation, const Population &populationObj) {

    int groupID = 0;
    int counter = 0;

    for (auto const &group: populationObj.getGroups()) {
        if (counter < 100) {
            this->printIndividual(group.getMainBreeder(), simulation->getGeneration(), groupID, simulation->getReplica());

            for (auto const &helper: group.getHelpers()) {
                this->printIndividual(helper, simulation->getGeneration(), groupID, simulation->getReplica());
            }
            counter++;
        }
        groupID++;
    }
    for (auto const &floater: populationObj.getFloaters()) {
        this->printIndividual(floater, simulation->getGeneration(), groupID, simulation->getReplica());
    }


}

Statistics::Statistics() {
    this->parameters = Parameters::instance();

}

void Statistics::printIndividual(Individual individual, int generation, int groupID, int replica) {
    *parameters->getLastGenerationWriter() << fixed << showpoint
                                           << replica + 1
                                           << "\t" << generation
                                           << "\t" << groupID
                                           << "\t" << individual.getFishType()
                                           << "\t" << setprecision(4) << individual.getAge()
                                           << "\t" << setprecision(4) << individual.getAlpha()
                                           << "\t" << setprecision(4) << individual.getAlphaAge()
                                           << "\t" << setprecision(4) << individual.getBeta()
                                           << "\t" << setprecision(4) << individual.getGamma()
                                           << "\t" << setprecision(4) << individual.getDelta()
                                           << "\t" << setprecision(4) << individual.getDrift()
                                           << "\t" << setprecision(4) << individual.getDispersal()
                                           << "\t" << setprecision(4) << individual.getHelp()
                                           << "\t" << setprecision(4) << individual.getSurvival()
                                           << "\t" << setprecision(4) << individual.getFecundity()
                                           << "\t" << setprecision(4) << individual.isInherit()
                                           << endl;
}
