#include <map>
#include <vector>
#include <set>
#include <ranges>
#include <algorithm>

#include <iostream>

#include "graph.hpp"
#include "d_separation.hpp"

using namespace std;

int main() {
    Digraph D{5};
    D.addEdge(1,4);
    D.addEdge(2,4);
    D.addEdge(2,5);
    D.addEdge(3,5);

    // printVertexSet(restrictedBFS(D, set<pair<Edge, Edge>>(), set<Vertex>{1}));
    // cout << "----" << endl;
    printVertexSet(dSeparation(D, {1}, {2,3}));

    return 0;
}