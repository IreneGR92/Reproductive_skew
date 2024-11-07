#include "Statistics.h"
#include "spdlog/spdlog.h"


using namespace std;

/* CALCULATE STATISTICS */
void Statistics::calculateStatistics(const Population &populationObj) {
    //Counters
    population = 0, totalFloaters = 0, totalHelpers = 0, totalMainBreeders = 0, totalSubordinateBreeders = 0;

    //Relatedness
    relatednessHelpers = 0.0, relatednessBreeders = 0.0;


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
    std::vector<double> fecundityGroups;
    std::vector<double> offspringMainBreeders;
    std::vector<double> offspringSubordinateBreeders;
    std::vector<double> totalOffspringGroups;


    for (const Individual &helper: helpers) {
        if (helper.getFishType() != HELPER) {
            spdlog::warn("helper wrong class");
        }
    }

    for (const Individual &floater: populationObj.getFloaters()) {
        if (floater.getFishType() != FLOATER) {
            spdlog::warn("floater wrong class");
        }
    }

    for (const Individual &breeder: allBreeders) {
        if (breeder.getFishType() != BREEDER) {
            spdlog::warn("breeder wrong class");
        }
    }

    mk = populationObj.getMk();

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
        fecundityGroups.push_back(group.getFecundityGroup());
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
    //    assert(population > 0);

    //Initialize the stats

    //Genes
    alpha.addValues(individualsAll.get(ALPHA));
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
    fecundityGroup.addValues(fecundityGroups);
    offspringMainBreeder.addValues(offspringMainBreeders);
    offspringOfSubordinateBreeders.addValues(offspringSubordinateBreeders);
    totalOffspringGroup.addValues(totalOffspringGroups);


    //Correlations in different levels
    relatednessHelpers = calculateRelatednessHelpers(populationObj.getGroups());
    relatednessBreeders = calculateRelatednessBreeders(populationObj.getGroups());
}

double
Statistics::calculateRelatednessHelpers(const std::vector<Group> &groups) {
    //TODO: generalize relatedness calculation

    //Relatedness
    double correlation;
    int counter = 0;
    double meanX = 0, meanY = 0, stdevX = 0, stdevY = 0, sumX = 0.0, sumY = 0.0;
    double sumProductXY = 0, sumProductXX = 0, sumProductYY = 0;
    double X, Y;

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
                X = (helper.getDrift() - meanX);
                Y = (group.getMainBreeder().getDrift() - meanY);

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
        correlation = 999; //TODO: make R code interpret this as NA
    } else {
        correlation = sumProductXY / (stdevX * stdevY * counter);
    }

//    assert(abs(correlation) <= 1);
    return correlation;
}


double Statistics::calculateRelatednessBreeders(const std::vector<Group> &groups) {

    //Relatedness
    double correlation;
    int counter = 0;
    double meanX = 0, meanY = 0, stdevX = 0, stdevY = 0, sumX = 0.0, sumY = 0.0;
    double sumProductXY = 0, sumProductXX = 0, sumProductYY = 0;
    double X, Y;

    for (const Group &group: groups) {
        for (const Individual &breeder: group.getSubordinateBreeders()) {
            if (group.isBreederAlive() && group.hasSubordinateBreeders()) {
                sumX += breeder.getDrift();
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
        for (const Individual &breeder: group.getSubordinateBreeders()) {
            if (!std::isnan(breeder.getDispersal()) || !std::isnan(breeder.getHelp())) {

                X = (breeder.getDrift() - meanX);
                Y = (group.getMainBreeder().getDrift() - meanY);

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
        correlation = 999; //TODO: make R code interpret this as NA
    } else {
        correlation = sumProductXY / (stdevX * stdevY * counter);
    }

//    assert(abs(correlation) <= 1);
    return correlation;
}


/* PRINT STATISTICS TO CONSOLE */

void Statistics::printHeadersToConsole() {
    // column headings on screen
    spdlog::debug(
            "{:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9}",
            "gen", "pop", "deaths", "emig", "float", "group", "maxGroup", "subBreed", "age", "alpha", "beta", "gamma",
            "delta", "disper", "immRate", "help", "surv", "survOff", "skew", "offpr", "offsDom", "offsSub", "relatH",
            "relatB");
}

void Statistics::printToConsole(int generation, int deaths, int emigrants) {
    // show values on screen
    spdlog::debug(
            "{:<9} {:<9} {:<9} {:<9} {:<9} {:<9.2f} {:<9} {:<9.2f} {:<9.2f} {:<9.4f} {:<9.4f} {:<9.4f} {:<9.4f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f}",
            generation, population, deaths, emigrants, totalFloaters, groupSize.calculateMean(),
            groupSize.getMaxValue(),
            numOfSubBreeders.calculateMean(), age.calculateMean(), alpha.calculateMean(), beta.calculateMean(),
            gamma.calculateMean(), delta.calculateMean(), dispersal.calculateMean(), acceptanceRate.calculateMean(),
            help.calculateMean(), survival.calculateMean(), mk, reproductiveShareRate.calculateMean(),
            fecundityGroup.calculateMean(), offspringMainBreeder.calculateMean(),
            offspringOfSubordinateBreeders.calculateMean(),
            relatednessHelpers, relatednessBreeders);
}

MainCacheElement Statistics::generateMainCacheElement(int generation, int deaths, int newBreederOutsider,
                                                      int newBreederInsider) {
    return {
            generation,
            population,
            deaths,
            totalFloaters,
            groupSize.calculateMean(),
            numOfSubBreeders.calculateMean(),
            ageHelpers.calculateMean(),
            ageFloaters.calculateMean(),
            ageDomBreeders.calculateMean(),
            ageSubBreeders.calculateMean(),
            ageBecomeBreeder.calculateMean(),
            alpha.calculateMean(),
            beta.calculateMean(),
            gamma.calculateMean(),
            delta.calculateMean(),
            dispersal.calculateMean(),
            acceptanceRate.calculateMean(),
            help.calculateMean(),
            cumulativeHelp.calculateMean(),
            survivalHelpers.calculateMean(),
            survivalFloaters.calculateMean(),
            survivalDomBreeders.calculateMean(),
            survivalSubBreeders.calculateMean(),
            mk,
            reproductiveShareRate.calculateMean(),
            fecundityGroup.calculateMean(),
            fecundityGroup.calculateSD(),
            offspringMainBreeder.calculateMean(),
            offspringOfSubordinateBreeders.calculateMean(),
            relatednessHelpers,
            relatednessBreeders,
            newBreederOutsider,
            newBreederInsider
    };
}
