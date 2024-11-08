#ifndef MAINCACHEELEMENT_H
#define MAINCACHEELEMENT_H


class MainCacheElement {
public:
    int generation;
    int population;
    int deaths;
    int totalFloaters;
    double groupSize;
    double numOfSubBreeders;
    double groupExtinction;
    double groupColonizationRate;
    double ageHelpers;
    double ageFloaters;
    double ageDomBreeders;
    double ageSubBreeders;
    double ageBecomeBreeder;
    double alpha;
    double beta;
    double gamma;
    double delta;
    double dispersal;
    double acceptanceRate;
    double help;
    double cumulativeHelp;
    double survivalHelpers;
    double survivalFloaters;
    double survivalDomBreeders;
    double survivalSubBreeders;
    double mk;
    double reproductiveShareRate;
    double fecundityGroupMean;
    double fecundityGroupSD;
    double offspringMainBreeder;
    double offspringOfSubordinateBreeders;
    double relatednessHelpers;
    double relatednessBreeders;
    int newBreederOutsider;
    int newBreederInsider;

    MainCacheElement(int gen, int pop, int dths, int totalFlts, double extint, double colonRate,
                     double grpSize, double numSubBrdrs, double ageHlprs,
                     double ageFltrs, double ageDomBrdrs, double ageSubBrdrs, double ageBcmBrdr, double alph,
                     double bet,
                     double gmm, double dlt, double dsprsl, double accRate, double hlp, double cumHlp, double survHlprs,
                     double survFltrs, double survDomBrdrs, double survSubBrdrs, double m, double reprShareRate,
                     double fecGrpMean, double fecGrpSD, double offMainBrdr, double offSubBrdrs, double relHlprs,
                     double relBrdrs, int newBrdrOut, int newBrdrIn)
        : generation(gen), population(pop), deaths(dths), totalFloaters(totalFlts), groupExtinction(extint),
          groupColonizationRate(colonRate), groupSize(grpSize),
          numOfSubBreeders(numSubBrdrs), ageHelpers(ageHlprs), ageFloaters(ageFltrs), ageDomBreeders(ageDomBrdrs),
          ageSubBreeders(ageSubBrdrs), ageBecomeBreeder(ageBcmBrdr), alpha(alph), beta(bet), gamma(gmm), delta(dlt),
          dispersal(dsprsl), acceptanceRate(accRate), help(hlp), cumulativeHelp(cumHlp), survivalHelpers(survHlprs),
          survivalFloaters(survFltrs), survivalDomBreeders(survDomBrdrs), survivalSubBreeders(survSubBrdrs), mk(m),
          reproductiveShareRate(reprShareRate), fecundityGroupMean(fecGrpMean), fecundityGroupSD(fecGrpSD),
          offspringMainBreeder(offMainBrdr), offspringOfSubordinateBreeders(offSubBrdrs), relatednessHelpers(relHlprs),
          relatednessBreeders(relBrdrs), newBreederOutsider(newBrdrOut), newBreederInsider(newBrdrIn) {
    }
};


#endif //MAINCACHEELEMENT_H
