//
// Created by Manuja DeSilva on 4/25/20.
//

#include "Experiment.h"

using namespace rocksdb;
using namespace std;

class BlockCacheExperiment: public Experiment {
public:
    void SpecifyParameters(int parameter);
    void RunExperiment(int parameter);
};