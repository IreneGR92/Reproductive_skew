#include "Statistics.h"
#include "spdlog/spdlog.h"


using namespace std;

/* CALCULATE STATISTICS */
void Statistics::calculateStatistics(const Population &populationObj) {
    // Counters
    population = 0, totalFloaters = 0, totalHelpers = 0, totalMainBreeders = 0, totalSubordinateBreeders = 0, emptyGroupsCount = 0;

    // Relatedness
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
        if (helper.getRoleType() != HELPER) {
            spdlog::warn("helper wrong class");
        }
    }

    for (const Individual &floater: populationObj.getFloaters()) {
        if (floater.getRoleType() != FLOATER) {
            spdlog::warn("floater wrong class");
        }
    }

    for (const Individual &breeder: allBreeders) {
        if (breeder.getRoleType() != BREEDER) {
            spdlog::warn("breeder wrong class");
        }
    }

    mk = populationObj.getMk();

    for (const Group &group: populationObj.getGroups()) {
        if (!group.isBreederAlive() && group.getHelpers().empty() && group.getSubordinateBreeders().empty()) {
            emptyGroupsCount++;
        }
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

    // Counters
    totalHelpers = helpers.size();
    totalFloaters = populationObj.getFloaters().size();
    totalMainBreeders = mainBreeders.size();
    totalSubordinateBreeders = subordinateBreeders.size();
    population = totalMainBreeders + totalSubordinateBreeders + totalHelpers + totalFloaters;
    groupExtinction = static_cast<double>(emptyGroupsCount) / static_cast<double>(parameters->getMaxColonies());
    groupColonizationRate = static_cast<double>(populationObj.getGroupColonization()) / static_cast<double>(parameters->getMaxColonies());



    // Initialize the stats

    // Genes
    alpha.addValues(individualsAll.get(ALPHA));
    beta.addValues(individualsAll.get(BETA));
    gamma.addValues(individualsAll.get(GAMMA));
    delta.addValues(individualsAll.get(DELTA));

    // Phenotypes
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

    // Group attributes
    groupSize.addValues(groupSizes);
    numOfSubBreeders.addValues(numSubBreeders);
    cumulativeHelp.addValues(cumHelp);
    acceptanceRate.addValues(acceptanceRates);
    reproductiveShareRate.addValues(reproductiveShareRates);
    fecundityGroup.addValues(fecundityGroups);
    offspringMainBreeder.addValues(offspringMainBreeders);
    offspringOfSubordinateBreeders.addValues(offspringSubordinateBreeders);
    totalOffspringGroup.addValues(totalOffspringGroups);

    // Relatedness
    relatednessHelpers = relatedness.calculateRelatednessHelpers(populationObj.getGroups());
    relatednessBreeders = relatedness.calculateRelatednessBreeders(populationObj.getGroups());
}




/* PRINT STATISTICS TO CONSOLE */

void Statistics::printHeadersToConsole() {
    // column headings on screen
    spdlog::debug(
            "{:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9} {:<9}",
            "gen", "pop", "deaths", "emig", "float","extinct", "colon", "group", "maxGroup",  "subBreed",  "age", "alpha", "beta", "gamma",
            "delta", "disper", "immRate", "help", "surv", "survOff", "skew", "offpr", "offsDom", "offsSub", "relatH",
            "relatB");
}

void Statistics::printToConsole(int generation, int deaths, int emigrants) {
    // show values on screen
    spdlog::debug(
            "{:<9} {:<9} {:<9} {:<9} {:<9} {:<9.2f} {:<9.2f} {:<9.2f} {:<9} {:<9.2f} {:<9.2f} {:<9.4f} {:<9.4f} {:<9.4f} {:<9.4f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f} {:<9.2f}",
            generation, population, deaths, emigrants, totalFloaters, groupExtinction, groupColonizationRate,
            groupSize.calculateMean(),groupSize.getMaxValue(), numOfSubBreeders.calculateMean(),
            age.calculateMean(), alpha.calculateMean(), beta.calculateMean(),
            gamma.calculateMean(), delta.calculateMean(), dispersal.calculateMean(), acceptanceRate.calculateMean(),
            help.calculateMean(), survival.calculateMean(), mk, reproductiveShareRate.calculateMean(),
            fecundityGroup.calculateMean(), offspringMainBreeder.calculateMean(),
            offspringOfSubordinateBreeders.calculateMean(),
            relatednessHelpers, relatednessBreeders);
}


/* GENERATE MAIN RESULTS CALCULATIONS */

MainCacheElement Statistics::generateMainCacheElement(int generation, int deaths, int newBreederOutsider,
                                                      int newBreederInsider) {
    return {
            generation,
            population,
            deaths,
            totalFloaters,
            groupExtinction,
            groupColonizationRate,
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
