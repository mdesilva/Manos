//
// Created by Manuja DeSilva on 4/25/20.
//

#include "rocksdb/db.h"
#include "rocksdb/table.h"
#include "rocksdb/filter_policy.h"
#include <iostream>
#include <chrono>

using namespace rocksdb;
using namespace std;

void readRandom(DB* db) {
    string value;
    auto startTime = chrono::steady_clock::now();
    for(int i=0; i<500000; i++) {
        int randomKey = rand() % 500000;
        db->Get(ReadOptions(), to_string(randomKey), &value);
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Random read operation took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

int main() {
    DB* db;
    Options options;
    BlockBasedTableOptions table_options;
    int numBitsForBloomFilter = 1000;
    table_options.filter_policy.reset(NewBloomFilterPolicy(numBitsForBloomFilter,false));
    options.table_factory.reset(NewBlockBasedTableFactory(table_options));
    options.create_if_missing = true;
    options.statistics = CreateDBStatistics();
    Status status = DB::Open(options, "/tmp/exp1", &db);
    assert(status.ok());
    cout << "Using bloom filter size " << numBitsForBloomFilter << endl;
    for(int i=0; i < 10000000; i++) {
        db->Put(WriteOptions(), to_string(i), "Arbitrary");
    }
    readRandom(db);
    cout << "BLOOM_FILTER_USEFUL: " << options.statistics->getTickerCount(BLOOM_FILTER_USEFUL) << endl;
    cout << "# of times bloom filter did not avoid read and data actually existed (true positives) " << options.statistics->getTickerCount(BLOOM_FILTER_FULL_TRUE_POSITIVE) << endl;
}
