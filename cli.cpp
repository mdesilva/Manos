#include "rocksdb/db.h"
#include <iostream>
#include <string>
#include <fstream>
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

int insertKey() {
    using namespace rocksdb;
    DB* db;
    Options options;
    options.create_if_missing = true;
    Status status = DB::Open(options, "/tmp/testdb", &db);
    assert(status.ok());
    status = db->Put(WriteOptions(), "key1", "Manuja");
    string value;
    status = db->Get(ReadOptions(), "key1", &value);
    cout << value << endl;
    return 0;
}