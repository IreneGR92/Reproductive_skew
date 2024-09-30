#include "FilePrinter.h"

#include <sstream>


using namespace std;

void FilePrinter::writeMainFile(std::vector<ResultCache *> &results) {
    //print header
    this->printHeader(*this->mainWriter);
    // column headings in output file main
    *this->mainWriter << "Replica" << "\t" << "Generation" << "\t" << "Population" << "\t"
            << "Deaths" << "\t" << "Floaters" << "\t" << "Group_size" << "\t" << "Sub_Breeders"
            << "\t"
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
        const auto &cache = result->getMainCache();
        writeResults(*this->mainWriter, cache);
    }
}

void FilePrinter::writeLastGenerationFile(std::vector<ResultCache *> &results) {
    auto writer = this->lastGenerationWriter;
    //print header
    this->printHeader(*writer);
    // column headings in output file last generation
    *this->lastGenerationWriter << "replica" << "\t" << "generation" << "\t" << "groupID"
            << "\t" << "type" << "\t" << "age" << "\t"
            << "alpha" << "\t" << "beta" << "\t"
            << "gamma" << "\t" << "delta" << "\t" << "drift" << "\t"
            << "dispersal" << "\t" << "help" << "\t" << "survival" << "\t"
            << "inherit" << std::endl;

    //print results

    for (auto &result: results) {
        const auto &cache = result->getLastGenerationCache();

        for (auto &cacheElement: cache) {
            std::ostringstream oss;
            oss << fixed << showpoint
                    << parameters->getReplica() + 1
                    << "\t" << cacheElement.generation
                    << "\t" << cacheElement.groupID
                    << "\t" << cacheElement.individual.getFishType()
                    << "\t" << setprecision(4) << cacheElement.individual.getAge()
                    << "\t" << setprecision(4) << cacheElement.individual.getAlpha()
                    << "\t" << setprecision(4) << cacheElement.individual.getBeta()
                    << "\t" << setprecision(4) << cacheElement.individual.getGamma()
                    << "\t" << setprecision(4) << cacheElement.individual.getDelta()
                    << "\t" << setprecision(4) << cacheElement.individual.getDrift()
                    << "\t" << setprecision(4) << cacheElement.individual.getDispersal()
                    << "\t" << setprecision(4) << cacheElement.individual.getHelp()
                    << "\t" << setprecision(4) << cacheElement.individual.getSurvival()
                    << "\t" << setprecision(4) << cacheElement.individual.isInherit();
            *writer << oss.str() << endl;
        }
    }
}

void FilePrinter::printHeader(std::ofstream &writer) {
    writer << "PARAMETER VALUES" << endl

            << "Bet-hedging_help?:" << "\t" << parameters->isBetHedgingHelp() << endl
            << "Help_obligatory?:" << "\t" << parameters->isHelpObligatory() << endl
            << "Predictable_environment?:" << "\t" << parameters->isPredictableEnvironment() << endl
            << "Evolution_help_after_dispersal?:" << "\t" << parameters->isEvolutionHelpAfterDispersal() << endl
            << "No_group_augmentation?:" << "\t" << parameters->isNoGroupAugmentation() << endl
            << "No_effect_relatedness?:" << "\t" << parameters->isNoRelatedness() << endl
            << "No_effect_age_inheritance?:" << "\t" << parameters->isAgeNoInfluenceInheritance() << endl
            << "Initial_population:" << "\t" << parameters->getMaxColonies() * (parameters->getInitNumHelpers() + 1)
            << endl
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
            << "mOff(Average_mortality_offspring):" << "\t" << parameters->getMOff() << endl
            << "mRate(Change_mortality_offspring):" << "\t" << parameters->getMRate() << endl
            << "mStep(Rate_change_mortality_offspring):" << "\t" << parameters->getMStep() << endl
            << "K0(Base_fecundity):" << "\t" << parameters->getK0() << endl
            << "Kh(Benefit_help_fecundity):" << "\t" << parameters->getKh() << endl
            << "Knb(Benefit_number_breeders_fecundity):" << "\t" << parameters->getKnb() << endl
            << "y(Min_skew):" << "\t" << parameters->getY() << endl
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

void FilePrinter::writeResults(ofstream &writer, const std::vector<std::string> &cache) {
    for (auto &line: cache) {
        writer << line << endl;
    }
}

FilePrinter::FilePrinter(Parameters *parameters) : parameters(parameters) {
    // Create the output files
    this->mainWriter = new std::ofstream("main_" + parameters->getName() + ".txt");
    this->lastGenerationWriter = new std::ofstream("last_generation_" + parameters->getName() + ".txt");
}
