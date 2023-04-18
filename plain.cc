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

    auto J = VertexSet{1};
    auto L = VertexSet{4};
    prettyPrint(cout, J, dSeparation(D, J, L), L);

    return 0;
}