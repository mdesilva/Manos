#include <iostream>
#include "Workload.h"

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
        cout << "Workload size not defined. Defaulting to 100 key value pairs." << endl;
        datasetSize = 100;
    } else {
        datasetSize = stoi(argv[2]);
    }

    if (argc < 4) {
        cout << "Number of total queries not defined. Defaulting to 100 total queries. " << endl;
        numTotalQueries = 100;
    } else {
        numTotalQueries = stoi(argv[3]);
    }

    string filePath = argv[1];
    Workload workload (filePath, datasetSize, numTotalQueries);
    workload.exec_point_queries();
    return 0;
}
