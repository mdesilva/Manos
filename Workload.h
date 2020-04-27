//
// Created by Manuja DeSilva on 3/22/20.
// Workload.h
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include "rocksdb/db.h"
#include "RocksDBOptions.h"
#define TOTAL_TIME chrono::duration_cast<chrono::milliseconds>(end_time-start_time).count()

using namespace rocksdb;
using namespace std;

class Workload {
public:
    fstream workload_file_;
    int dataset_size_;
    int num_total_queries_;
    int range_size_;
    DB* db;

    //Each of these floats represent the proportion of the type of query.
    //To get the number of queries for each query type, multiply the query percentage * num_total_queries_.
    float point_queries_;
    float range_queries_;
    float point_inserts_;
    float point_updates_;
    float point_deletes_;
    float range_inserts_;
    float range_updates_;
    float range_deletes_;

    //DB Options
    int memtable_size_ = -1;
    int cache_size_ = -1;
    int compaction_strategy_ = -1;
    int bloom_filter_size_ = -1;
    int num_memtables_ = -1;

    Workload (string workload_file_Path);
    Workload ();
    bool ProportionsAreValid();
    float GetFloatFromLine();
    int GetIntFromLine();
    Options InitOptions(rocksdb::Options options);
    void OpenDatabase();
    void GenerateData();
    void ExecPointQueries();
    void ExecRangeQueries();
    void ExecPointInserts();
    void ExecPointUpdates();
    void ExecPointDeletes();
    void ExecRangeInserts();
    void ExecRangeUpdates();
    void ExecRangeDeletes();
    void ExecWorkload();
};
