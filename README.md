# Manos - A CLI tool for benchmarking RocksDB

Benchmark RocksDB using both bounded and unbounded data workloads.

## Prerequisites
RocksDB: https://github.com/facebook/rocksdb.git </br>
C++11

## Compiling
To compile the project, run the following command: `g++ cli.cpp Workload.cpp -std=c++11 -L ${ROCKSDBDIR} -lrocksdb -o cli`, where ${ROCKSDBDIR} should be replaced with the path to your clone of the RocksDB repo. 

## Running the CLI
TBA
