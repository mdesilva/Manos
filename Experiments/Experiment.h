//
// Created by Manuja DeSilva on 4/25/20.
//

#include "rocksdb/db.h"
#include "rocksdb/table.h"
#include <iostream>
#include <chrono>

/*
 * Conduct num_experiments testing the effect of tuning some RocksDB parameter, subsituting experiment_values for the parameter for each experiment, on a specified workload
 */
class Experiment {
public:
    std::vector<int> experiment_values_;
    rocksdb::DB* db_;
    rocksdb::Options options_;
    rocksdb::BlockBasedTableOptions table_options_;

    void RunExperiments();
    void CreateDBWithOptions();
    void FillDB();
    void DeleteDB();

    virtual void RunExperiment(int parameter) {};
    virtual void SpecifyParameters(int parameter){};
    virtual ~Experiment() {};
};

