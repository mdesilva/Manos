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

Workload::Workload(string workloadFilePath, int datasetSize, int numTotalQueries) {
    this->workloadFile = fstream (workloadFilePath);
    this->datasetSize = datasetSize;
    this->numTotalQueries = numTotalQueries;
    if (this->workloadFile.is_open()) {
        this->pointQueries = get_val_from_line();
        this->rangeQueries = get_val_from_line();
        this->pointInserts = get_val_from_line();
        this->pointUpdates = get_val_from_line();
        this->pointDeletes = get_val_from_line();
        this->rangeInserts = get_val_from_line();
        this->rangeUpdates = get_val_from_line();
        this->rangeDeletes = get_val_from_line();
        workloadFile.close();
        if (this->verify_proportions() == true) {
            cout << "Successfully loaded workload." << endl;
            this->open_database();
            this->generate_data();
        } else {
            cout << "Workload proportions do not add up to 1. Please check your numbers." << endl;
        }
    } else {
        cout << "Unable to open file";
    }
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

float Workload::get_val_from_line() {
    string line;
    getline(this->workloadFile, line);
    size_t split_pos = line.find("=");
    return stof(line.substr(split_pos + 1));
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
        this->db->Put(WriteOptions(), ("key" + to_string(index)), ("Manuja" + to_string(index)));
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
        this->db->Get(ReadOptions(), ("key" + to_string(randomKey)), &value);
    }
    auto endTime = chrono::steady_clock::now();

    cout << "Executing " << numPointQueries << " point queries took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}

