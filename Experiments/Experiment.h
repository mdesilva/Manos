//
// Created by Manuja DeSilva on 4/25/20.
//

#include "rocksdb/db.h"
#include "rocksdb/table.h"
#include <iostream>
#include <chrono>

#define DEFAULT_WORKLOAD_SIZE 10000000

/*
 * Conduct num_experiments testing the effect of tuning some RocksDB parameter, subsituting experiment_values for the parameter for each experiment, on a specified workload
 */
class Experiment {
public:
    std::vector<int> experiment_values_; //The number of experiments conducted will be derived from the size of experiment_values_
    rocksdb::DB* db_;
    rocksdb::Options options_;
    rocksdb::BlockBasedTableOptions table_options_;

    void RunExperiments();
    void CreateDBWithOptions();
    //To override the default FillDB function, create your own FillDB function in your experiment sub class.
    //If running experiments on db fill, create empty FillDB function in sub class.
    virtual void FillDB();
    bool DBNotEmpty();
    void DeleteDB();

    virtual void RunExperiment(int parameter) {};
    virtual void SpecifyParameters(int parameter){};
    virtual ~Experiment() {};
};

