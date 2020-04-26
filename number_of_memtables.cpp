//
// Created by Michael Hendrick on 4/25/20.
//

#include "rocksdb/db.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace rocksdb;

void getProperty(string property, DB* db) {
    string value;
    db->GetProperty(property, &value);
    cout << property << ":" << value << endl;
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

int main(){
    DB* db;
    Options options;
    options.create_if_missing = true;
    options.max_write_buffer_number = 5;
    options.min_write_buffer_number_to_merge = 4;
    Status status = DB::Open(options, "/tmp/exp0", &db);
    assert(status.ok());
    cout << "The maximum number of memtables, both active and immutable is: "<<  options.write_buffer_size << endl;
    cout << "The minimum number of memtables to be merged before flushing to storage is: "<<  options.write_buffer_size << endl;

    getProperty("rocksdb.cur-size-all-mem-tables", db);
    auto startTime = chrono::steady_clock::now();
    for(int count=0;count < 10000000; count++) {
        db->Put(WriteOptions(), to_string(count), "Arbitrary");
    }
    auto endTime = chrono::steady_clock::now();
    getProperty("rocksdb.cur-size-all-mem-tables", db);
    cout << "Write 10M tuples took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
    readRandom(db);

    return 0;
}