//
// Created by Manuja DeSilva on 3/22/20.
// Workload.h
#include <fstream>

using namespace std;

class Workload {
public:
    fstream workloadFile;
    float pointQueries;
    float rangeQueries;
    float pointInserts;
    float pointUpdates;
    float pointDeletes;
    float rangeInserts;
    float rangeUpdates;
    float rangeDeletes;

    Workload (string workloadFilePath);
    bool verify_proportions();
    float get_val_from_line();
};
