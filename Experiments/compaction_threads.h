//
// Created by Michael Hendrick on 5/4/20.
//


#include "Experiment.h"

using namespace rocksdb;
using namespace std;


class compaction_threads: public Experiment {
public:
    void SpecifyParameters(int a, int b);
    void RunExperiment();
};