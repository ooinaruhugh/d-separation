#include <vector>
#include <set>

#include "graph.hpp"

using namespace std;

vector<vector<set<Vertex>>> globalMarkovStatements;
Digraph D;

void initTest() {
    D = Digraph{5};

    D.addEdge(1,4);
    D.addEdge(2,4);
    D.addEdge(2,5);
    D.addEdge(3,5);

    globalMarkovStatements = vector<vector<set<Vertex>>>{
        {{1, 4}, {3, 5}, {2}}, 
        {{1}, {2, 3, 5}, {}}, 
        {{1, 2, 4}, {3}, {}}, 
        {{1, 3}, {2}, {}}
    };
}