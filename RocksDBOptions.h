//
// Created by Manuja DeSilva on 4/26/20.
//
#include <iostream>
#include "rocksdb/db.h"
#include "rocksdb/table.h"
#include "rocksdb/options.h"
#include "rocksdb/filter_policy.h"

/*
 * Helper class to adjust RocksDB knobs. Initialize class and then set options_ to a rocksdb::Options instance.
 * Perform knob adjustments using methods below, and then get properly built options object by calling FinalizeOptions().
 */
class RocksDBOptions {
public:
    std::string valid_options_ [5] = {"SetMemtableSize", "SetLRUCacheSize", "SetCompactionStrategy", "SetBloomFilterSize", "SetNumMemtables"};
    rocksdb::Options options_;
    rocksdb::BlockBasedTableOptions table_options_;
    std::map<std::string, void (*)()> func_mapping_;

    bool IsParameterValueValid(int value);
    void SetMemtableSize(int size_in_mb);
    void SetLRUCacheSize(int size_in_mb);
    void SetCompactionStrategy(int type);
    void SetBloomFilterSize(int num_bits);
    void SetNumMemtables(int num_memtables);
    void SetMaxBackgroundCompactions(int num);
    void SetMaxBackgroundFlushes(int num);
    void SetBackgroundFlushThreads(int num);
    void SetBackgroundCompactThreads(int num);
    void SetMaxWriteBuffer(int num);
    void SetMinWriteBufferToMerge(int num);


    rocksdb::Options FinalizeOptions();
};

