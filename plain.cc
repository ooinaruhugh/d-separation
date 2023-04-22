/*
    Apparently, this little demo became a "Enumerate all possible partitions of the vertex set of a digraph,
    compute the d-separation using some algorithm and output those results that are non-empty".
*/

#include <map>
#include <vector>
#include <set>
#include <ranges>
#include <algorithm>

#include <iterator>

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

    auto J = VertexSet{1};
    auto L = VertexSet{4,5};

    printIndependenceStatement(cout, J, dSeparation(D, J, L), L);

    return 0;
}