#include "Tests.h"
#include "Query.h"
#include "Helpers.h"

#include <locale>
#include <chrono>
#include <iostream>
#include <string>

//#define RUN_TESTS

using namespace std;
using namespace qac;
using namespace tests;

void init() {
    std::setlocale(LC_ALL, "en_US.UTF-8");
}

void tests_run() {
#ifdef RUN_TESTS
    test_TST();
#endif
}

void loadQueryLog(Query& query, string filename) {
    cout << "Loading query logs..." << endl;
    query.loadHistory(filename);
}

void buildCache(Query& query) {
    cout << "Building cache for most popular queries..." << endl;
    query.buildCache();
}

int main() {
    init();
    tests_run();

    Query query;

    cout << "---Initialization---" << endl;
    {
        auto start = chrono::system_clock::now();
        loadQueryLog(query, "query_logs.txt"); // change to argv
        buildCache(query);
        auto duration = chrono::duration_cast<chrono::seconds>
                (std::chrono::system_clock::now() - start);
        cout << "Initialization time: " << duration.count() << " sec" << endl;
    }
    cout << "---Initialization complete---" << endl << endl;

    cout << "Enter query (Press ENTER to get suggestions, type \'...\' to exit): " << endl;
    string user_q;
    while (true) {
        cout << "===>" << endl;
        cout << "QUERY: ";
	if (!std::getline(cin, user_q)) {
            break;
        }
        helpers::trim(user_q);
        if (user_q == "...") {
            break;
        }
        if (!helpers::filter(user_q)) {
            cout << "Sorry, bad input query, please try again ..." << endl;
            cout << "<===" << endl; // no standard 'defer' in c++ ...
            continue;
        }

        cout << "Processing request...";
        auto start = chrono::system_clock::now();
        string corr_q;
        auto queries = query.suggest(user_q/*, corr_q*/);

        auto duration = chrono::duration_cast<chrono::milliseconds>
                (std::chrono::system_clock::now() - start);
        cout << " (" << duration.count() << " ms)" << endl;
//        if (!corr_q.empty()) {
//            cout << "Did you mean: " << corr_q << endl;
//        }
        if (!queries.empty()) {
            cout << queries.size() << " suggestions:" << endl;
            for (auto q : queries) {
                cout << q << endl;
            }
        } else {
            cout << "No suggestions for this query, sorry..." << endl;
        }
        cout << "<===" << endl;
    }
}
