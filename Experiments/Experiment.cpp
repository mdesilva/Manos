//
// Created by Manuja DeSilva on 4/25/20.
//

#include "Experiment.h"

void Experiment::CreateDBWithOptions() {
    this->options_.create_if_missing = true;
    this->options_.statistics = rocksdb::CreateDBStatistics();
    rocksdb::Status status = rocksdb::DB::Open(this->options_, "/tmp/exp", &this->db_);
    assert(status.ok());
}

void Experiment::FillDB() {
    std::cout << "Writing to DB with default workload of " << DEFAULT_WORKLOAD_SIZE <<  " tuples..." << std::endl;
    for(int i=0; i < DEFAULT_WORKLOAD_SIZE; i++) {
        this->db_->Put(rocksdb::WriteOptions(), std::to_string(i), "Arbitrary");
    }
}

bool Experiment::DBNotEmpty() {
    if(this->options_.statistics->getTickerCount(rocksdb::BYTES_WRITTEN) > 0) {
        return true;
    } else {
        return false;
    }
}


void Experiment::RunExperiments() {
    std::cout << "Running experiments..." << std::endl;
    for(int i=0; i<this->experiment_values_.size();i++) {
        int parameter = this->experiment_values_.at(i);
        this->SpecifyParameters(parameter);
        this->CreateDBWithOptions();
        this->FillDB();
        this->RunExperiment(parameter);
        this->DeleteDB();
    }
}

void Experiment::DeleteDB() {
    rocksdb::Status status = this->db_->Close();
    std::remove("/tmp/exp");
}