//
// Created by Michael Hendrick on 4/25/20.
//

#include "rocksdb/db.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace rocksdb;

int main(){
    DB* db;
    Options options;
    options.create_if_missing = true;
    int flush_threads = 1;
    int compact_threads = 2;
    options.max_background_compactions = compact_threads;
    options.max_background_flushes = flush_threads;
    options.env->SetBackgroundThreads(flush_threads, rocksdb::Env::Priority::HIGH); // flush threads
    options.env->SetBackgroundThreads(compact_threads, rocksdb::Env::Priority::LOW); // compaction threads
    Status status = DB::Open(options, "/tmp/exp0", &db);
    assert(status.ok());
    cout << "The number of flush threads: "<<  flush_threads << endl;
    cout << "The number of compaction threads: "<<  compact_threads << endl;

    auto startTime = chrono::steady_clock::now();
    for(int count=0;count < 10000000; count++) {
        db->Put(WriteOptions(), to_string(count), "Arbitrary");
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Write 10M tuples took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;

    return 0;
}