//
// Created by Manuja DeSilva on 3/22/20.
//
#include <string>
#include <iostream>
#include <fstream>
#include "Workload.h"
#include "rocksdb/db.h"
#include <chrono>

using namespace std;
using namespace rocksdb;

Workload::Workload(string workloadFilePath) {
    this->workloadFile = fstream (workloadFilePath);
    if (this->workloadFile.is_open()) {
        this->pointQueries = get_float_from_line();
        this->rangeQueries = get_float_from_line();
        this->pointInserts = get_float_from_line();
        this->pointUpdates = get_float_from_line();
        this->pointDeletes = get_float_from_line();
        this->rangeInserts = get_float_from_line();
        this->rangeUpdates = get_float_from_line();
        this->rangeDeletes = get_float_from_line();
        this->datasetSize = get_int_from_line();
        this->numTotalQueries = get_int_from_line();
        this->rangeSize = get_int_from_line();
        workloadFile.close();
        if (this->verify_proportions() == true) {
            cout << "Successfully loaded workload from file. Beginning workload..." << endl;
            this->exec_workload();
        } else {
            cout << "Workload proportions do not add up to 1. Please try again." << endl;
        }
    } else {
        cout << "Unable to open specified file. " << endl;
    }
}
Workload::Workload() {
}

bool Workload::verify_proportions() {
    return
    (
    this->pointQueries +
    this->rangeQueries +
    this->pointInserts +
    this->pointUpdates +
    this->pointDeletes +
    this->rangeInserts +
    this->rangeUpdates +
    this->rangeDeletes
    ) == 1.0;
}

float Workload::get_float_from_line() {
    string line;
    getline(this->workloadFile, line);
    size_t split_pos = line.find("=");
    return stof(line.substr(split_pos + 1));
}

int Workload::get_int_from_line() {
    string line;
    getline(this->workloadFile, line);
    size_t split_pos = line.find("=");
    return stoi(line.substr(split_pos + 1));
}


void Workload::open_database() {
    Options options;
    options.create_if_missing = true;
    Status status = DB::Open(options, "/tmp/testdb", &this->db);
    assert(status.ok());
    cout << "Successfully opened database." << endl;
}

void Workload::generate_data() {
    int index;
    cout << ("Generating " + to_string(this->datasetSize) + " key-value pairs...")<< endl;
    for (index=0; index < this->datasetSize; index++) {
        this->db->Put(WriteOptions(), (to_string(index)), ("Arbitrary" + to_string(index)));
    }
    cout << "Done generating data." << endl;
}

void Workload::exec_point_queries() {
    int numPointQueries = this->numTotalQueries * this->pointQueries;

    auto startTime = chrono::steady_clock::now();
    for (int count=0; count < numPointQueries; count++) {
        string value;
        //get random key to query
        int randomKey = rand() % this->datasetSize;
        this->db->Get(ReadOptions(), (to_string(randomKey)), &value);
    }
    auto endTime = chrono::steady_clock::now();

    cout << "Executing " << numPointQueries << " point queries took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}


void Workload::exec_range_queries(){
    int numRangeQueries = this->numTotalQueries * this->rangeQueries;
    int rangeSize = this->rangeSize;
    int i = 0;
    auto startTime = chrono::steady_clock::now();
    while(i < numRangeQueries){
        int start = rand() % (this->datasetSize - rangeSize);
        for (int count=0; count < rangeSize; count++) {
            string value;
            int key = start;
            this->db->Get(ReadOptions(), (to_string(key)), &value);
            key++;
        }
        i++;
    }
    auto endTime = chrono::steady_clock::now();

    cout << "Executing " << numRangeQueries << " range queries took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

void Workload::exec_point_updates() {
    int numPointUpdates = this->numTotalQueries * this->pointUpdates;

    auto startTime = chrono::steady_clock::now();
    for (int i=0; i < numPointUpdates; i++) {
        string value;
        int randomKey = rand() % this->datasetSize;
        this->db->Put(WriteOptions(), (to_string(randomKey)), ("Update" + to_string(randomKey)));
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Executing " << numPointUpdates << " point updates took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

void Workload::exec_point_deletes() {
    int numPointDeletes = this->numTotalQueries * this->pointDeletes;

    auto startTime = chrono::steady_clock::now();
    for (int i=0; i < numPointDeletes; i++) {
        int randomKey = rand() % this->datasetSize;
        this->db->Delete(WriteOptions(), (to_string(randomKey)));
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Executing " << numPointDeletes << " point deletes took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

void Workload::exec_point_inserts() {
    int numPointInserts = this->numTotalQueries * this->pointInserts;

    auto startTime = chrono::steady_clock::now();
    for (int i=0; i < numPointInserts; i++) {
        int key = this->datasetSize + i;
        this->db->Put(WriteOptions(), to_string(key), ("Arbitrary" + to_string(key)));
    }
    auto endTime = chrono::steady_clock::now();
    this->datasetSize = this->datasetSize + numPointInserts;
    cout << "Executing " << numPointInserts << " point inserts took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

void Workload::exec_range_inserts() {
    int numTotalRangeInserts = this->numTotalQueries * this->rangeInserts;
    int numCompletedRangeInserts = 0;
    auto startTime = chrono::steady_clock::now();
    while (numCompletedRangeInserts < numTotalRangeInserts) {
        for (int i=0; i < this->rangeSize; i++) {
            int key = this->datasetSize + i;
            this->db->Put(WriteOptions(), to_string(key), ("Arbitrary" + to_string(key)));
        }
        this->datasetSize = this->datasetSize + rangeSize;
        numCompletedRangeInserts = numCompletedRangeInserts + 1;
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Executing " << numTotalRangeInserts << " range inserts took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

void Workload::exec_range_updates() {
    int numTotalRangeUpdates = this->numTotalQueries * this->rangeUpdates;
    int numCompletedRangeUpdates = 0;
    auto startTime = chrono::steady_clock::now();
    while (numCompletedRangeUpdates < numTotalRangeUpdates) {
        //pick random range to do updates on
        int randomStartKey = rand() % (this->datasetSize - this->rangeSize);
        for (int i=0; i < this->rangeSize; i++ ) {
            int key = randomStartKey + i;
            this->db->Put(WriteOptions(), to_string(key), "Updated" + to_string(key));
        }
        numCompletedRangeUpdates = numCompletedRangeUpdates + 1;
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Executing " << numTotalRangeUpdates << " range updates took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

void Workload::exec_range_deletes() {
    int numTotalRangeDeletes = this->numTotalQueries * this->rangeDeletes;
    int numCompletedRangeDeletes = 0;
    int startKey = 0;
    auto startTime = chrono::steady_clock::now();
    while (numCompletedRangeDeletes < numTotalRangeDeletes) {
        int endKey = startKey + this->rangeSize;
        this->db->DeleteRange(WriteOptions(), nullptr, to_string(startKey), to_string(endKey));
        startKey = startKey + this->rangeSize;
        numCompletedRangeDeletes = numCompletedRangeDeletes + 1;
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Executing " << numTotalRangeDeletes << " range deletes took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;

}

void Workload::exec_workload() {
    cout << "---PHASE 1: GENERATE DATA---" << endl;
    this->open_database();
    this->generate_data();
    cout << "---PHASE 2: STATISTICS---" << endl;
    this->exec_point_queries();
    this->exec_range_queries();
    this->exec_point_inserts();
    this->exec_point_updates();
    this->exec_point_deletes();
    this->exec_range_inserts();
    this->exec_range_updates();
    this->exec_range_deletes();

}