//
// Created by Manuja DeSilva on 4/26/20.
//
#include "CompactionStyleExperiment.h"


void CompactionStyleExperiment::SpecifyParameters(int parameter) {
    if (parameter == 0) {
        this->options_.compaction_style = rocksdb::kCompactionStyleLevel;
    } else {
        this->options_.compaction_style = rocksdb::kCompactionStyleUniversal;
    }
}

void CompactionStyleExperiment::FillDB() {}

void CompactionStyleExperiment::RunExperiment(int parameter) {
    std::cout << "Writing 100M tuples to database..." << std::endl;
    auto startTime = std::chrono::steady_clock::now();
    for(int i=0; i < 100000000; i++) {
        this->db_->Put(rocksdb::WriteOptions(), std::to_string(i), "Arbitrary");
    }
    auto endTime = std::chrono::steady_clock::now();
    auto totalTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
    if (parameter == 0) {
        std::cout << "Write took " << totalTime << " ms. " <<  "using " << "Level " << " style compaction." << std::endl;
    } else {
        std::cout << "Write took " << totalTime << " ms. " <<  "using " << "Universal " << " style compaction." << std::endl;
    }
    std::cout << "COMPACT_WRITE_BYTES: " << this->options_.statistics->getTickerCount(rocksdb::COMPACT_WRITE_BYTES) << std::endl;

}