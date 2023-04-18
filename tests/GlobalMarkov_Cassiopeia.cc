#include "graph.hpp"
#include "d_separation.hpp"

#include <iostream>
#include <iomanip>

#include <vector>
#include <set>

using namespace std;

int main() {
    Digraph D{5};
    D.addEdge(1,4);
    D.addEdge(2,4);
    D.addEdge(2,5);
    D.addEdge(3,5);

    // Calculated with Macaulay2, version 1.21 and the package "GraphicalModels".
    auto globalMarkovStatements = vector<vector<set<Vertex>>>{
        {{1, 4}, {3, 5}, {2}}, 
        {{1}, {2, 3, 5}, {}}, 
        {{1, 2, 4}, {3}, {}}, 
        {{1, 3}, {2}, {}}
    };

    auto f{cerr.flags()};

    for (auto statement : globalMarkovStatements) {
        auto J = statement[0], K = statement[1], L = statement[2];
        cerr << "Testing J = {" << J << "}, L = {" << L << "} ...";
        
        auto output = dSeparation(D, J, L);
        if (output != K) {
            cerr << " yields {" << output << "}" << endl;
            return 1;
        }
        cerr << setw(20) << right << "[ok]" << endl;
        cerr.flags(f);
    }

    return 0;
}