//
// Created by Manuja DeSilva on 3/22/20.
// Workload.h
#include <fstream>
#include "rocksdb/db.h"
#include <chrono>

using namespace std;
using namespace rocksdb;

class Workload {
public:
    fstream workloadFile;
    int datasetSize;
    int numTotalQueries;
    DB* db;

    //Each of these floats represent the proportion of the type of query.
    //To get the number of queries for each query type, multiply the query percentage * numTotalQueries.
    float pointQueries;
    float rangeQueries;
    float pointInserts;
    float pointUpdates;
    float pointDeletes;
    float rangeInserts;
    float rangeUpdates;
    float rangeDeletes;


    Workload (string workloadFilePath, int datasetSize, int numTotalQueries);
    bool verify_proportions();
    float get_val_from_line();
    void open_database();
    void generate_data();
    void exec_point_queries();
};
