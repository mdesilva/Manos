//
// Created by Manuja DeSilva on 3/22/20.
//
#include "Workload.h"

Workload::Workload(string workload_file_path) {
    this->workload_file_ = fstream (workload_file_path);
    if (this->workload_file_.is_open()) {
        this->dataset_size_= GetIntFromLine();
        this->num_total_queries_ = GetIntFromLine();
        this->range_size_ = GetIntFromLine();
        this->point_queries_ = GetFloatFromLine();
        this->range_queries_ = GetFloatFromLine();
        this->point_inserts_ = GetFloatFromLine();
        this->point_updates_ = GetFloatFromLine();
        this->point_deletes_ = GetFloatFromLine();
        this->range_inserts_ = GetFloatFromLine();
        this->range_updates_ = GetFloatFromLine();
        this->range_deletes_ = GetFloatFromLine();
        this->memtable_size_ = GetIntFromLine();
        this->cache_size_ = GetIntFromLine();
        this->num_memtables_ = GetIntFromLine();
        this->compaction_strategy_ = GetIntFromLine();
        this->bloom_filter_size_ = GetIntFromLine();
        workload_file_.close();
        if (this->ProportionsAreValid() == true) {
            cout << "Successfully loaded workload from file. Beginning workload..." << endl;
            this->ExecWorkload();
        } else {
            cout << "Workload proportions do not add up to 1. Please try again." << endl;
        }
    } else {
        cout << "Unable to open specified file. " << endl;
    }
}
Workload::Workload() {
}

//TODO: BUG: Function always returning false even though proportion total is 1
bool Workload::ProportionsAreValid() {
    double total = this->point_queries_ + this->range_queries_ + this->point_inserts_ + this->point_updates_ + this->point_deletes_ + this->range_inserts_ + this->range_updates_ + this->range_deletes_;
    cout << "Proportion total: " << total << endl;
    return true;
    //return total == 1;
}

float Workload::GetFloatFromLine() {
    string line;
    getline(this->workload_file_, line);
    size_t split_pos = line.find("=");
    return stof(line.substr(split_pos + 1));
}

int Workload::GetIntFromLine() {
    string line;
    getline(this->workload_file_, line);
    size_t split_pos = line.find("=");
    return stoi(line.substr(split_pos + 1));
}

Options Workload::InitOptions(Options current_options) {
    RocksDBOptions options;
    options.options_ = current_options;
    options.SetNumMemtables(this->num_memtables_);
    options.SetLRUCacheSize(this->cache_size_);
    options.SetMemtableSize(this->memtable_size_);
    options.SetCompactionStrategy(this->compaction_strategy_);
    options.SetBloomFilterSize(this->bloom_filter_size_);
    return options.FinalizeOptions();
}


void Workload::OpenDatabase() {
    Options options;
    options.create_if_missing = true;
    Status status = DB::Open(this->InitOptions(options), "/tmp/testdb", &this->db);
    assert(status.ok());
    cout << "Successfully opened database." << endl;
}

void Workload::GenerateData() {
    int index;
    cout << ("Generating " + to_string(this->dataset_size_) + " key-value pairs...")<< endl;
    auto start_time = chrono::steady_clock::now();
    for (index=0; index < this->dataset_size_; index++) {
        this->db->Put(WriteOptions(), (to_string(index)), ("Arbitrary" + to_string(index)));
    }
    auto end_time = chrono::steady_clock::now();
    cout << "Generated " << this->dataset_size_ << " tuples in " << TOTAL_TIME << " ms." << endl;
}

void Workload::ExecPointQueries() {
    int num_point_queries = this->num_total_queries_ * this->point_queries_;
    auto start_time = chrono::steady_clock::now();
    for (int count=0; count < num_point_queries; count++) {
        string value;
        //get random key to query
        int random_key = rand() % this->dataset_size_;
        this->db->Get(ReadOptions(), (to_string(random_key)), &value);
    }
    auto end_time = chrono::steady_clock::now();

    cout << "Executing " << num_point_queries << " point queries took " << TOTAL_TIME << " ms." << endl;
}


void Workload::ExecRangeQueries(){
    int num_range_queries = this->num_total_queries_ * this->range_queries_;
    int i = 0;
    auto start_time = chrono::steady_clock::now();
    while(i < num_range_queries){
        int start = rand() % (this->dataset_size_ - this->range_size_);
        for (int count=0; count < this->range_size_; count++) {
            string value;
            int key = start;
            this->db->Get(ReadOptions(), (to_string(key)), &value);
            key++;
        }
        i++;
    }
    auto end_time = chrono::steady_clock::now();
    cout << "Executing " << num_range_queries << " range queries took " << TOTAL_TIME << " ms." << endl;
}

void Workload::ExecPointUpdates() {
    int num_point_deletes = this->num_total_queries_ * this->point_updates_;
    auto start_time = chrono::steady_clock::now();
    for (int i=0; i < num_point_deletes; i++) {
        string value;
        int random_key = rand() % this->dataset_size_;
        this->db->Put(WriteOptions(), (to_string(random_key)), ("Update" + to_string(random_key)));
    }
    auto end_time = chrono::steady_clock::now();
    cout << "Executing " << num_point_deletes << " point updates took " << TOTAL_TIME << " ms." << endl;
}

void Workload::ExecPointDeletes() {
    int num_point_deletes = this->num_total_queries_ * this->point_deletes_;
    auto start_time = chrono::steady_clock::now();
    for (int i=0; i < num_point_deletes; i++) {
        int random_key = rand() % this->dataset_size_;
        this->db->Delete(WriteOptions(), (to_string(random_key)));
    }
    auto end_time = chrono::steady_clock::now();
    cout << "Executing " << num_point_deletes << " point deletes took " << TOTAL_TIME << " ms." << endl;
}

void Workload::ExecPointInserts() {
    int num_point_inserts = this->num_total_queries_ * this->point_inserts_;
    auto start_time = chrono::steady_clock::now();
    for (int i=0; i < num_point_inserts; i++) {
        int key = this->dataset_size_ + i;
        this->db->Put(WriteOptions(), to_string(key), ("Arbitrary" + to_string(key)));
    }
    auto end_time = chrono::steady_clock::now();
    this->dataset_size_ = this->dataset_size_ + num_point_inserts;
    cout << "Executing " << num_point_inserts << " point inserts took " << TOTAL_TIME << " ms." << endl;
}

void Workload::ExecRangeInserts() {
    int num_total_range_inserts = this->num_total_queries_ * this->range_inserts_;
    int num_completed_range_inserts = 0;
    auto start_time = chrono::steady_clock::now();
    while (num_completed_range_inserts < num_total_range_inserts) {
        for (int i=0; i < this->range_size_; i++) {
            int key = this->dataset_size_ + i;
            this->db->Put(WriteOptions(), to_string(key), ("Arbitrary" + to_string(key)));
        }
        this->dataset_size_ = this->dataset_size_ + range_size_;
        num_completed_range_inserts = num_completed_range_inserts + 1;
    }
    auto end_time = chrono::steady_clock::now();
    cout << "Executing " << num_total_range_inserts << " range inserts took " << TOTAL_TIME << " ms." << endl;
}

void Workload::ExecRangeUpdates() {
    int num_total_range_updates = this->num_total_queries_ * this->range_updates_;
    int num_completed_range_updates = 0;
    auto start_time = chrono::steady_clock::now();
    while (num_completed_range_updates < num_total_range_updates) {
        //pick random range to do updates on
        int randomstart_key = rand() % (this->dataset_size_ - this->range_size_);
        for (int i=0; i < this->range_size_; i++ ) {
            int key = randomstart_key + i;
            this->db->Put(WriteOptions(), to_string(key), "Updated" + to_string(key));
        }
        num_completed_range_updates = num_completed_range_updates + 1;
    }
    auto end_time = chrono::steady_clock::now();
    cout << "Executing " << num_total_range_updates << " range updates took " << TOTAL_TIME << " ms." << endl;
}

void Workload::ExecRangeDeletes() {
    int num_total_range_deletes = this->num_total_queries_ * this->range_deletes_;
    int num_completed_range_deletes = 0;
    int start_key = 0;
    auto start_time = chrono::steady_clock::now();
    while (num_completed_range_deletes < num_total_range_deletes) {
        int end_key = start_key + this->range_size_;
        this->db->DeleteRange(WriteOptions(), nullptr, to_string(start_key), to_string(end_key));
        start_key = start_key + this->range_size_;
        num_completed_range_deletes = num_completed_range_deletes + 1;
    }
    auto end_time = chrono::steady_clock::now();
    cout << "Executing " << num_total_range_deletes << " range deletes took " << TOTAL_TIME << " ms." << endl;

}

void Workload::ExecWorkload() {
    cout << "---PHASE 1: SET PARAMETERS---" << endl;
    this->OpenDatabase();
    cout << "---PHASE 2: GENERATE DATA---" << endl;
    this->GenerateData();
    cout << "---PHASE 3: STATISTICS---" << endl;
    this->ExecPointQueries();
    this->ExecRangeQueries();
    this->ExecPointInserts();
    this->ExecPointUpdates();
    this->ExecPointDeletes();
    this->ExecRangeInserts();
    this->ExecRangeUpdates();
    this->ExecRangeDeletes();

}