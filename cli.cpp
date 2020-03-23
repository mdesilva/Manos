#include <iostream>
#include "Workload.h"

using namespace std;

/*
 * Workload information will be contained in a text file. CLI must define the path to that file.
 */

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Please specify a file that contains workload information" << endl;
        return -1;
    }
    string filePath = argv[1];
    Workload workload (filePath);
    return 0;
}
