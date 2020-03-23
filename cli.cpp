#include <iostream>
#include "Workload.h"

using namespace std;

/*
 * RocksDB benchmark CLI tool
 * argument 1 - file path to workload parameters, following the structure defined in workloads/template.txt
 * argument 2 - # of key-value pairs to generate. (default=100)
 */

int main(int argc, char **argv) {
    int workloadSize;
    if (argc < 2) {
        cout << "Please specify a file that contains workload information." << endl;
        return -1;
    }

    if (argc < 3) {
        cout << "Workload size not defined. Defaulting to 100 key value pairs." << endl;
        workloadSize = 100;
    } else {
        workloadSize = stoi(argv[2]);
    }
    string filePath = argv[1];
    Workload workload (filePath, workloadSize);
    return 0;
}
