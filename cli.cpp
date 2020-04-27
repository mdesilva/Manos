#include <iostream>
#include "Workload.h"
#include <cstdlib>


using namespace std;

/*
 * RocksDB benchmark CLI tool
 * argument 1 - flag
 * argument 2 - file path to workload parameters, following the structure defined in workloads/template.txt.
 * If defined, use file to create workload instead of interactive session.
 */


int main(int argc, char **argv) {
    if (argc == 3) {
        //Potentially read in workload from file
        string flag = argv[1];
        if (flag == "-f") {
            Workload workload (argv[2]);
            return 0;
        } else {
            cout << "Unrecognized flag. Use -f to specify a workload parameter file." << endl;
            return -1;
        }
    } else {
        //Interactive session
        Workload workload;

        cout << "Specify workload proportion information as decimals less than or equal to 1." << endl;
        cout << "Proportion of point queries: ";
        cin >> workload.pointQueries;
        cout << "Proportion of range queries: ";
        cin >> workload.rangeQueries;
        cout << "Proportion of point inserts: ";
        cin >> workload.pointInserts;
        cout << "Proportion of point updates: ";
        cin >> workload.pointUpdates;
        cout << "Proportion of point deletes: ";
        cin >> workload.pointDeletes;
        cout << "Proportion of range inserts: ";
        cin >> workload.rangeInserts;
        cout << "Proportion of range updates: ";
        cin >> workload.rangeUpdates;
        cout << "Proportion of range deletes: ";
        cin >> workload.rangeDeletes;

        if (!workload.ProportionsAreValid()) {
            cout << "Workload proportions do not add up to 1. Please try again.";
            return -1;
        }

        cout << "Beginning interactive benchmarking session..." << endl;
        cout << "Dataset size (# of key-value pairs): ";
        cin >> workload.datasetSize;
        cout << "Number of total queries: ";
        cin >> workload.numTotalQueries;
        cout << "Range size: ";
        cin >> workload.rangeSize;

        cout << "Now beginning option definitions. To skip setting and use default, type -1.";
        cout << "Options: Memtable size (MB)";
        cin >> workload.memtable_size;
        cout << "Options: Cache size (MB)";
        cin >> workload.cache_size;
        cout << "Options: Compaction strategy (0 for level, 1 for universal)";
        cin >> workload.compaction_strategy;
        cout << "Options: Bloom filter size (# of bits)";
        cin >> workload.bloom_filter_size;
        cout << "Options: # of Memtables: ";
        cin >> workload.num_memtables;

        cout << "\nAll parameters specified. Beginning workload...\n" << endl;
        workload.exec_workload();
        return 0;
    }
}