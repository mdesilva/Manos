//
// Created by Michael Hendrick on 4/27/20.
//

#include "rocksdb/db.h"
#include "rocksdb/utilities/checkpoint.h"

#include <iostream>
#include <chrono>

using namespace std;
using namespace rocksdb;


void readRandom(DB* db, int range) {
    string value;
    auto startTime = chrono::steady_clock::now();
    for(int i=0; i<range; i++) {
        int randomKey = rand() % range;
        db->Get(ReadOptions(), to_string(randomKey), &value);
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Random read operation took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

int main(){
    DB* db;
    Options options;
    options.create_if_missing = true;

    string checkpoint_dir = "/Users/MichaelHendrick/RocksDBBenchmark/tmp/";
    Status status = DB::Open(options, "/tmp/exp0", &db);
    assert(status.ok());
    Checkpoint* checkpoint_ptr;
    Status cp = Checkpoint::Create(db, &checkpoint_ptr);
    assert(cp.ok());

    auto startTime = chrono::steady_clock::now();
    for(int count=0;count < 10000000; count++) {
        db->Put(WriteOptions(), to_string(count), "Arbitrary");
        if(count % 500000 == 0) {
            Status currentPoint = checkpoint_ptr->CreateCheckpoint(checkpoint_dir);
            assert(currentPoint.ok());
            checkpoint_dir = "/Users/MichaelHendrick/RocksDBBenchmark/tmp" + std::to_string(count+rand()) +"/";

        }
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Write 10M tuples took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;

    return 0;
}
