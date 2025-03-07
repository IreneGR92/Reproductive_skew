#include "FilePrinter.h"

#include <sstream>
#include <iomanip>
#include "Config.h"
#include "spdlog/spdlog.h"
#include "GitHash.h"


using namespace std;


void FilePrinter::writeMainFile(std::vector<std::unique_ptr<ResultCache> > &results) {

    //print header
    this->printHeader(*this->mainWriter);
    // column headings in output file main
    *this->mainWriter << "Replica" << "\t" << "Generation" << "\t" << "Population" << "\t"
            << "Deaths" "\t" << "Floaters" << "\t" << "GroupExtinction" << "\t" << "GroupColonization" << "\t"
            << "Group_size" << "\t" << "Sub_Breeders" << "\t"
            << "Age_H" << "\t" << "Age_F" << "\t" << "Age_DomB" << "\t" << "Age_SubB" << "\t"
            << "Age_New_Breeder" << "\t"
            << "Alpha" << "\t" << "Beta" << "\t" << "Gamma" << "\t" << "Delta" << "\t"
            << "Dispersal" << "\t" << "AcceptRate" << "\t"
            << "Help" << "\t" << "CumHelp" << "\t"
            << "Survival_H" << "\t" << "Survival_F" << "\t" << "Survival_DomB" << "\t"
            << "Survival_SubB" << "\t"
            << "mk" << "\t" << "ReprodShareRate" << "\t" << "FecundityGroup" << "\t"
            << "FecundityGroupSD" << "\t"
            << "OffspringDomBreeder" << "\t" << "OffspringSubBreeders" << "\t"
            << "Relatedness_H" << "\t" << "Relatedness_B" << "\t"
            << "newBreederOutsider" << "\t" << "newBreederInsider" << "\t"
            << std::endl;

    //print results
    for (auto &result: results) {
        auto cache = result->getMainCache();
        while (!cache.empty()) {
            auto cacheElement = cache.front();
            std::ostringstream oss;
            oss << fixed << showpoint
                    << result->getReplica() + 1 // +1 to start from 1 in result files
                    << "\t" << cacheElement.generation
                    << "\t" << cacheElement.population
                    << "\t" << cacheElement.deaths
                    << "\t" << cacheElement.totalFloaters
                    << "\t" << setprecision(PRECISION) << cacheElement.groupExtinction
                    << "\t" << setprecision(PRECISION) << cacheElement.groupColonizationRate
                    << "\t" << setprecision(PRECISION) << cacheElement.groupSize
                    << "\t" << setprecision(PRECISION) << cacheElement.numOfSubBreeders
                    << "\t" << setprecision(PRECISION) << cacheElement.ageHelpers
                    << "\t" << setprecision(PRECISION) << cacheElement.ageFloaters
                    << "\t" << setprecision(PRECISION) << cacheElement.ageDomBreeders
                    << "\t" << setprecision(PRECISION) << cacheElement.ageSubBreeders
                    << "\t" << setprecision(PRECISION) << cacheElement.ageBecomeBreeder
                    << "\t" << setprecision(PRECISION) << cacheElement.alpha
                    << "\t" << setprecision(PRECISION) << cacheElement.beta
                    << "\t" << setprecision(PRECISION) << cacheElement.gamma
                    << "\t" << setprecision(PRECISION) << cacheElement.delta
                    << "\t" << setprecision(PRECISION) << cacheElement.dispersal
                    << "\t" << setprecision(PRECISION) << cacheElement.acceptanceRate
                    << "\t" << setprecision(PRECISION) << cacheElement.help
                    << "\t" << setprecision(PRECISION) << cacheElement.cumulativeHelp
                    << "\t" << setprecision(PRECISION) << cacheElement.survivalHelpers
                    << "\t" << setprecision(PRECISION) << cacheElement.survivalFloaters
                    << "\t" << setprecision(PRECISION) << cacheElement.survivalDomBreeders
                    << "\t" << setprecision(PRECISION) << cacheElement.survivalSubBreeders
                    << "\t" << setprecision(PRECISION) << cacheElement.mk
                    << "\t" << setprecision(PRECISION) << cacheElement.reproductiveShareRate
                    << "\t" << setprecision(PRECISION) << cacheElement.fecundityGroupMean
                    << "\t" << setprecision(PRECISION) << cacheElement.fecundityGroupSD
                    << "\t" << setprecision(PRECISION) << cacheElement.offspringMainBreeder
                    << "\t" << setprecision(PRECISION) << cacheElement.offspringOfSubordinateBreeders
                    << "\t" << setprecision(PRECISION) << cacheElement.relatednessHelpers
                    << "\t" << setprecision(PRECISION) << cacheElement.relatednessBreeders
                    << "\t" << cacheElement.newBreederOutsider
                    << "\t" << cacheElement.newBreederInsider;
            *this->mainWriter << oss.str() << endl;
            cache.pop();
        }
    }
}

void FilePrinter::writeLastGenerationFile(std::vector<std::unique_ptr<ResultCache> > &results) {
    //print header
    this->printHeader(*lastGenerationWriter);
    // column headings in output file last generation
    *this->lastGenerationWriter << "replica" << "\t" << "generation" << "\t" << "groupID"
            << "\t" << "type" << "\t" << "age" << "\t"
            << "alpha" << "\t" << "beta" << "\t"
            << "gamma" << "\t" << "delta" << "\t" << "drift" << "\t"
            << "dispersal" << "\t" << "help" << "\t" << "survival" << "\t"
            << "inherit" << std::endl;

    //print results

    for (auto &result: results) {
        auto cache = result->getLastGenerationCache();

        while (!cache.empty()) {
            auto cacheElement = cache.front();
            std::ostringstream oss;
            oss << fixed << showpoint
                    << result->getReplica() + 1
                    << "\t" << cacheElement.generation
                    << "\t" << cacheElement.groupID
                    << "\t" << cacheElement.individual.getRoleType()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getAge()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getAlpha()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getBeta()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getGamma()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getDelta()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getDrift()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getDispersal()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getHelp()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.getSurvival()
                    << "\t" << setprecision(PRECISION) << cacheElement.individual.isInherit();
            *lastGenerationWriter << oss.str() << endl;
            cache.pop();
        }
    }
}

void FilePrinter::printHeader(std::ofstream &writer) {

    writer  << "GIT HASH:" << "\t" << GIT_HASH << endl
            << "PARAMETER VALUES" << endl
            << "Bet-hedging_help?:" << "\t" << parameters->isBetHedgingHelp() << endl
            << "Help_obligatory?:" << "\t" << parameters->isHelpObligatory() << endl
            << "Predictable_environment?:" << "\t" << parameters->isPredictableEnvironment() << endl
            << "Dominant_breeder_replacement?:" << "\t" << parameters->isDominantBreederReplacement() << endl
            << "No_group_augmentation?:" << "\t" << parameters->isNoGroupAugmentation() << endl
            << "No_effect_relatedness?:" << "\t" << parameters->isNoRelatedness() << endl
            << "No_effect_age_inheritance?:" << "\t" << parameters->isAgeNoInfluenceInheritance() << endl
            << "Initial_population:" << "\t" << parameters->getMaxColonies() * (parameters->getInitNumHelpers() + 1) <<
            endl
            << "Number_of_colonies:" << "\t" << parameters->getMaxColonies() << endl
            << "Number_generations:" << "\t" << parameters->getNumGenerations() << endl
            << "Number_replicates:" << "\t" << parameters->getMaxNumReplicates() << endl
            << "Min_age_become_breeder:" << "\t" << parameters->getMinAgeBecomeBreeder() << endl
            << "Fixed_group_size:" << "\t" << parameters->getFixedGroupSize() << endl
            << "Reduced_relatedness:" << "\t" << parameters->getReducedRelatedness() << endl
            << "ImmigrantsMax:" << "\t" << parameters->getFloatersSampledImmigration() << endl
            << "m(Overall_mortality):" << "\t" << parameters->getM() << endl
            << "X0(intercept):" << "\t" << parameters->getX0() << endl
            << "Xh(Cost_help_survival):" << "\t" << parameters->getXh() << endl
            << "Xn(Benefit_group_size_survival):" << "\t" << parameters->getXn() << endl
            << "Xe(Cost_expulsion_survival):" << "\t" << parameters->getXe() << endl
            << "Xrs(Reproductive_suppression_survival):" << "\t" << parameters->getXrs() << endl
            << "Xf(Added_survival_floaters):" << "\t" << parameters->getXf() << endl
            << "mOff(Average_mortality_offspring):" << "\t" << parameters->getMOff() << endl
            << "mFreq(Freq_change_mortality_offspring):" << "\t" << parameters->getMFreq() << endl
            << "mMagnit(Magnitude_change_mortality_offspring):" << "\t" << parameters->getMMagnit() << endl
            << "K0(Base_fecundity):" << "\t" << parameters->getK0() << endl
            << "Kh(Benefit_help_fecundity):" << "\t" << parameters->getKh() << endl
            << "Knb(Benefit_number_breeders_fecundity):" << "\t" << parameters->getKnb() << endl
            << "initAlpha:" << "\t" << parameters->getInitAlpha() << endl
            << "initBeta:" << "\t" << parameters->getInitBeta() << endl
            << "initGamma:" << "\t" << parameters->getInitGamma() << endl
            << "initDelta:" << "\t" << parameters->getInitDelta() << endl
            << "mutAlpha:" << "\t" << parameters->getMutationAlpha() << endl
            << "mutBeta:" << "\t" << parameters->getMutationBeta() << endl
            << "mutGamma:" << "\t" << parameters->getMutationGamma() << endl
            << "mutDelta:" << "\t" << parameters->getMutationDelta() << endl
            << "mutDrift:" << "\t" << parameters->getMutationDrift() << endl
            << "stepAlpha:" << "\t" << parameters->getStepAlpha() << endl
            << "stepBeta:" << "\t" << parameters->getStepBeta() << endl
            << "stepGamma:" << "\t" << parameters->getStepGamma() << endl
            << "stepDelta:" << "\t" << parameters->getStepDelta() << endl
            << "stepDrift:" << "\t" << parameters->getStepDrift() << endl << endl;
}

FilePrinter::FilePrinter(std::shared_ptr<Parameters> &parameters) : parameters(parameters) {
    // Create the output files
    this->mainWriter = std::make_unique<std::ofstream>(
        Config::GET_OUTPUT_DIR() + "/" + "main_" + parameters->getName() + ".txt");
    this->lastGenerationWriter = std::make_unique<std::ofstream>(
        Config::GET_OUTPUT_DIR() + "/" + "last_generation_" + parameters->getName() + ".txt");
}

FilePrinter::~FilePrinter() {
    // Close the output files
    if (mainWriter->is_open()) {
        mainWriter->close();
    }
    if (lastGenerationWriter->is_open()) {
        lastGenerationWriter->close();
    }
}
