#include "graph.hpp"
#include "d_separation.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

#include <vector>
#include <set>

using namespace std;

extern void initTest();
extern vector<vector<set<Vertex>>> globalMarkovStatements;
extern Digraph D;

int main() {
    initTest();
    
    auto f{cerr.flags()};

    for (auto statement : globalMarkovStatements) {
        auto J = statement[0], K = statement[1], L = statement[2];
        stringstream ss;
        ss << "Testing J = {" << J << "}, L = {" << L << "} ...";
        cerr << setw(80) << left << ss.str();
        cerr.flags(f);

        auto output = dSeparation(D, J, L);
        if (output != K) {
            cerr << " yields {" << output << "}" << endl;
            return 1;
        }
        cerr << "[ok]" << endl;
    }

    return 0;
}