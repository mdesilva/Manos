//
// Created by Manuja DeSilva on 3/22/20.
// Workload.h
#include <fstream>
#include "rocksdb/db.h"

using namespace std;
using namespace rocksdb;

class Workload {
public:
    fstream workloadFile;
    DB* db;
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
    void open_database();
    void generate_data();
};
