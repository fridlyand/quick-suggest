#include "Query.h"
#include "Helpers.h"

#include <fstream>
#include <iostream>
#include <algorithm>

using namespace qac;
using namespace std;
using namespace helpers;

namespace {
    int getFileSize(ifstream& ifs) {
        ifs.seekg(0, ios_base::end);
        int size = ifs.tellg();
        ifs.seekg(0, ios_base::beg);
    }

    void removeBOM(ifstream& ifs) {
        char a,b,c;
        a = ifs.get();
        b = ifs.get();
        c = ifs.get();
        if(a!=(char)0xEF || b!=(char)0xBB || c!=(char)0xBF) {
            ifs.seekg(0);
        }
    }
}

void Query::loadHistory(string filename) {
    ifstream ifs{filename, ios_base::binary};
    if (!ifs) {
        cout << "Can't open log file." << endl;
        return;
    }

//    int size = getFileSize(ifs);
//    string buffer;
//    buffer.resize(size, ' ');
//    char* begin = &*begin(buffer);
//    ifs.read(begin, size);

    removeBOM(ifs);
    for(std::string line; getline(ifs, line);)
    {
        trim(line);
        if (filter(line)) {
            log.insert(line);

            for (auto word : splitWords(line)) {
                unigrams.insert(word);
            }
        }
    }

    ofstream ofs("unigrams.txt");
    auto keys = unigrams.keys();
    print_queue(keys, ofs);
    ofs.flush();
}

vector<string> Query::suggest(string query/*, string& corr_q*/) {
    string corr_q;
    for (auto word : splitWords(query)) {
        corr_q += corrector->correct(word) + " ";
    }
    trim(corr_q);
    if (corr_q == query) {
        corr_q.clear();
    }

    TST::Queue keys;

    while (true) {
        if (query.empty()) {
            keys = cache[0];
        } else if (query.size() == 1) {
            keys = cache.at(static_cast<int>(query[0]) - static_cast<int>('a') + 1);
        } else {
            keys = log.keysWithPrefix(query);
        }

        if (query == corr_q) {
//            if (old_keys.top().second > pow(keys.top().second, 2)) { // building some model, TODO: need description.
//                keys = old_keys;
//            }
            break;
        }

        if (!keys.empty()) {
            break;
        }
        if (!corr_q.empty()) {
            query = corr_q;
        } else if (keys.empty()) {
            break;
        }
    }
    vector<string> sugs;
    int count = sugg_limit;
    while (!keys.empty() && count--) {
        if (keys.top().first.find(corr_q) == 0) {
            corr_q.clear();
        }
        sugs.push_back(keys.top().first);
        keys.pop();
    }
    return sugs;
}

void Query::buildCache() {
    for (auto count = 0; count < cache.size(); ++count) {
        if (count) {
            cache[count] = log.keysWithPrefix(string(1, static_cast<char>('a' + count - 1)));
        } else {
            cache[count] = log.keys();
        }
    }
}

Query::Query() : corrector(new SpellCorrector(*this)) { }