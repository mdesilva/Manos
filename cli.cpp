#include <iostream>
#include "Workload.h"
#include <cstdlib>


using namespace std;

/*
 * RocksDB benchmark CLI tool
 * argument 1 - file path to workload parameters, following the structure defined in workloads/template.txt
 * argument 2 - # of key-value pairs to generate. (default=100)
 */

int main(int argc, char **argv) {
    int datasetSize;
    int numTotalQueries;

    if (argc < 2) {
        cout << "Please specify a file that contains workload information." << endl;
        return -1;
    }

    if (argc < 3) {
        cout << "Please specify a size for the range query." << endl;
        return -1;
    }

    if (argc < 4) {
        cout << "Workload size not defined. Defaulting to 1000 key value pairs." << endl;
        datasetSize = 1000;
    } else {
        datasetSize = stoi(argv[3]);
    }

    if (argc < 5) {
        cout << "Number of total queries not defined. Defaulting to 100 total queries. " << endl;
        numTotalQueries = 100;
    } else {
        numTotalQueries = stoi(argv[4]);
    }

    string filePath = argv[1];
    int rangeSize = stoi(argv[2]);
    Workload workload (filePath, datasetSize, rangeSize, numTotalQueries);
    workload.exec_point_queries();
    workload.exec_range_queries();
    //exec point inserts
    workload.exec_point_updates();
    workload.exec_point_deletes();
    return 0;
}
