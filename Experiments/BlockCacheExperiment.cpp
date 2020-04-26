//
// Created by Manuja DeSilva on 4/25/20.
//

#include "BlockCacheExperiment.h"


void BlockCacheExperiment::SpecifyParameters(int cache_size) {
    this->table_options_.block_cache = NewLRUCache(cache_size << 20);
    this->options_.table_factory.reset(NewBlockBasedTableFactory(this->table_options_));
}

void BlockCacheExperiment::RunExperiment(int cache_size) {
    string value;
    auto startTime = chrono::steady_clock::now();
    for(int i=0; i<50000; i++) {
        this->db_->Get(ReadOptions(), to_string(i), &value);
    }
    auto endTime = chrono::steady_clock::now();
    auto totalTime = chrono::duration_cast<chrono::milliseconds>(endTime-startTime).count();
    cout << "Read took " << totalTime << " ms" << " using an LRU cache size of " << cache_size << " MB." << endl;
}