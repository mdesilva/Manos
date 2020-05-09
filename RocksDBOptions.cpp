//
// Created by Manuja DeSilva on 4/26/20.
//
#include "RocksDBOptions.h"

using namespace std;

bool RocksDBOptions::IsParameterValueValid(int value) {
    if (value == -1) {
        return false;
    } else {
        return true;
    }
}

void RocksDBOptions::SetBloomFilterSize(int num_bits) {
    if (this->IsParameterValueValid(num_bits)) {
        this->table_options_.filter_policy.reset(rocksdb::NewBloomFilterPolicy(num_bits, false));
        cout << "Set bloom filter size to " << num_bits << " bits." << endl;
    } else {
        cout << "Using default bloom filter size." << endl;
    }
}

void RocksDBOptions::SetLRUCacheSize(int size_in_mb) {
    if (this->IsParameterValueValid(size_in_mb)) {
        this->table_options_.block_cache = rocksdb::NewLRUCache(size_in_mb << 20);
        cout << "Set LRU cache size to " << size_in_mb << " MB." << endl;
    } else {
        cout << "Using default LRU cache size." << endl;
    }

}

void RocksDBOptions::SetMemtableSize(int size_in_mb) {
    if (this->IsParameterValueValid(size_in_mb)) {
        this->options_.write_buffer_size = size_in_mb << 20;
        cout << "Set memtable size to " << size_in_mb << " MB." << endl;
    } else {
        cout << "Using default memtable size. " << endl;
    }
}

void RocksDBOptions::SetCompactionStrategy(int type) {
    if (type == 0) {
        this->options_.compaction_style = rocksdb::kCompactionStyleLevel;
        cout << "Using level style compaction" << endl;
    } else if (type == 1) {
        this->options_.compaction_style = rocksdb::kCompactionStyleUniversal;
        cout << "Using universal style compaction" << endl;
    } else {
        cout << "Defaulting to level style compaction." << endl;
        return;
    }
}

void RocksDBOptions::SetNumMemtables(int num_memtables) {
    if (this->IsParameterValueValid(num_memtables)) {
        this->options_.max_write_buffer_number = num_memtables;
        cout << "Set number of memtables to " << num_memtables << endl;
    } else {
        cout << "Using default number of memtables." << endl;
    }
}

void RocksDBOptions::SetMaxBackgroundCompactions(int num) {
        this->options_.max_background_compactions = num;
}

void RocksDBOptions::SetMaxBackgroundFlushes(int num) {
    this->options_.max_background_flushes = num;
}

void RocksDBOptions::SetBackgroundFlushThreads(int num) {
    this->options_.env->SetBackgroundThreads(num, rocksdb::Env::Priority::HIGH);
}

void RocksDBOptions::SetBackgroundCompactThreads(int num_memtables) {
    this->options_.env->SetBackgroundThreads(num, rocksdb::Env::Priority::LOW);
}
void RocksDBOptions::SetMaxWriteBuffer(int num) {
    this->options_.max_write_buffer_number = num;
}

void RocksDBOptions::SetMinWriteBufferToMerge(int num) {
    this->options_.min_write_buffer_number_to_merge = num;
}


rocksdb::Options RocksDBOptions::FinalizeOptions() {
    this->options_.table_factory.reset(NewBlockBasedTableFactory(this->table_options_));
    return this->options_;
}

//set this->options_ = rocksdb::Options manually
//get options manually after setting