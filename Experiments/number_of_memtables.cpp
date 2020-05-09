//
// Created by Michael Hendrick on 4/25/20.
//

#include "number_of_memtables.h"


void number_of_memtables::SpecifyParameters(int max_write, int min_merge) {
    this->tuner.options_ = this->options_;
    this->tuner.SetMaxWriteBuffer(max_write);
    this->tuner.SetMinWriteBufferToMerge(min_merge);
    this->options_ = this->tuner.FinalizeOptions();
    cout << "The maximum number of memtables, both active and immutable is: "<<  max_write << endl;
    cout << "The minimum number of memtables to be merged before flushing to storage is: "<<  min_merge << endl;
}

void number_of_memtables::getProperty(string property) {
    string value;
    this->db_->GetProperty(property, &value);
    cout << property << ":" << value << endl;
}

void number_of_memtables::readRandom() {
    string value;
    auto startTime = chrono::steady_clock::now();
    for(int i=0; i<50000; i++) {
        int randomKey = rand() % 50000;
        this->db_->Get(ReadOptions(), to_string(randomKey), &value);
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Random read operation took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

void number_of_memtables::RunExperiment(){
    getProperty("rocksdb.cur-size-all-mem-tables");
    auto startTime = chrono::steady_clock::now();
    for(int count=0;count < 10000000; count++) {
        this->db_->Put(WriteOptions(), to_string(count), "Arbitrary");
    }
    auto endTime = chrono::steady_clock::now();
    getProperty("rocksdb.cur-size-all-mem-tables");
    cout << "Write 10M tuples took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
    readRandom();
}