//
// Created by Manuja DeSilva on 4/26/20.
//
#include "Experiment.h"

class CompactionStyleExperiment: public Experiment {
public:
    //create map between parameter values and compaction styles
//    std::map<int,
    virtual void FillDB();
    void SpecifyParameters(int parameter);
    void RunExperiment(int parameter);
};