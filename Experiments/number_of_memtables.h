//
// Created by Michael Hendrick on 5/4/20.
//

#include "Experiment.h"

using namespace rocksdb;
using namespace std;

class number_of_memtables: public Experiment {
public:
    void SpecifyParameters(int a, int b);
    void RunExperiment();
    void readRandom();
    void getProperty(string param1);

};