#include <vector>
#include <set>

#include "graph.hpp"

using namespace std;

vector<vector<set<Vertex>>> globalMarkovStatements;
Digraph D;

void initTest() {
    D = Digraph{4};

    D.addEdge(1,2);
    D.addEdge(1,3);
    D.addEdge(2,4);
    D.addEdge(3,4);

    globalMarkovStatements = vector<vector<set<Vertex>>>{
        {{1}, {4}, {2, 3}}, 
        {{2}, {3}, {1}}
    };
}