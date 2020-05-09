//
// Created by Michael Hendrick on 4/25/20.
//

#include "compaction_threads.h"


void compaction_threads::SpecifyParameters(int flush_threads, int compact_threads) {
    this->tuner.options_ = this->options_;
    this->tuner.SetMaxBackgroundCompactions(compact_threads);
    this->tuner.SetMaxBackgroundFlushes(flush_threads);
    this->tuner.SetBackgroundFlushThreads(flush_threads);
    this->tuner.SetBackgroundCompactThreads(flush_threads);
    this->options_ = this->tuner.FinalizeOptions();
    cout << "The number of flush threads: "<<  flush_threads << endl;
    cout << "The number of compaction threads: "<<  compact_threads << endl;
}

void compaction_threads::RunExperiment() {
    auto startTime = chrono::steady_clock::now();
    for(int count=0;count < 10000000; count++) {
        this->db_->Put(WriteOptions(), to_string(count), "Arbitrary");
    }
    auto endTime = chrono::steady_clock::now();
    cout << "Write 10M tuples took " << chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count() << " ms." << endl;
}