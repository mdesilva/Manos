//
// Created by Manuja DeSilva on 4/23/20.
//

#include "rocksdb/db.h"
#include "rocksdb/table.h"
#include <iostream>
#include <chrono>
#include "Experiments/BlockCacheExperiment.h"

using namespace std;
using namespace rocksdb;

void read(DB* db) {
    string value;
    auto startTime = chrono::steady_clock::now();
    for(int i=0; i<50000; i++) {
        db->Get(ReadOptions(), to_string(i), &value);
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Read operation took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

void readRandom(DB* db) {
    string value;
    auto startTime = chrono::steady_clock::now();
    for(int i=0; i<50000; i++) {
        int randomKey = rand() % 50000;
        db->Get(ReadOptions(), to_string(randomKey), &value);
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Random read operation took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

int main() {
    DB* db;
    Options options;
    int cacheSize = 256; //in MB
    auto cache = NewLRUCache(cacheSize << 20); //lru cache size
    BlockBasedTableOptions table_options;
    table_options.block_cache = cache;
    options.table_factory.reset(NewBlockBasedTableFactory(table_options));
    options.create_if_missing = true;
    options.write_buffer_size = 64 << 20;
    options.statistics = CreateDBStatistics();
    Status status = DB::Open(options, "/tmp/exp1", &db);
    assert(status.ok());
    cout << "Using LRU cache size of " << cacheSize << " MB." << endl;
    for(int i=0; i < 10000000; i++) {
        db->Put(WriteOptions(), to_string(i), "Arbitrary");
    }
    readRandom(db);
    uint64_t block_cache_hit_read_1 = options.statistics->getTickerCount(BLOCK_CACHE_HIT);
    cout << "BLOCK_CACHE_HIT READ 1 " << block_cache_hit_read_1 << endl;
    readRandom(db);
    uint64_t block_cache_hit_read_2 = options.statistics->getTickerCount(BLOCK_CACHE_HIT);
    cout << "BLOCK_CACHE_HIT READ 2 " << block_cache_hit_read_2 - block_cache_hit_read_1 << endl;
    cout << "BLOCK_CACHE_HIT AGG " << block_cache_hit_read_2 << endl;
    cout << "L0 HIT " << options.statistics->getTickerCount(GET_HIT_L0) << endl;
    cout << "L1 HIT " << options.statistics->getTickerCount(GET_HIT_L1) << endl;
    cout << "MEMTABLE HIT " << options.statistics->getTickerCount(MEMTABLE_HIT) << endl;
    cout << "MEMTABLE MISS " << options.statistics->getTickerCount(MEMTABLE_MISS) << endl;
}

//int main() {
//    vector<int> cache_sizes;
//    cache_sizes.assign({8, 128, 256});
//    BlockCacheExperiment experiment;
//    experiment.experiment_values_ = cache_sizes;
//    experiment.RunExperiments();
//}