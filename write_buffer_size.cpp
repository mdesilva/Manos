//
// Created by Manuja DeSilva on 4/23/20.
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

int main(){
    DB* db;
    Options options;
    options.create_if_missing = true;
    options.write_buffer_size = 512 << 10;
    Status status = DB::Open(options, "/tmp/exp0", &db);
    assert(status.ok());
    cout << "Write buffer size for memtable: "<<  options.write_buffer_size << endl;
    getProperty("rocksdb.cur-size-active-mem-table", db);
    auto startTime = chrono::steady_clock::now();
    for(int count=0;count < 10000000; count++) {
        db->Put(WriteOptions(), to_string(count), "Arbitrary");
    }
    auto endTime = chrono::steady_clock::now();
    getProperty("rocksdb.cur-size-active-mem-table", db);
    cout << "Write 10M tuples took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
    return 0;
}