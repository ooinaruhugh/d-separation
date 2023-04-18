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

    // printVertexSet(restrictedBFS(D, set<pair<Edge, Edge>>(), set<Vertex>{1}));
    // cout << "----" << endl;

    // Hacky-hacky enumerate all subsets of {1,...,5}
    for (int i = 1; i < 32; ++i) {
        for (int j = 1; j < 32; ++j) {
            auto J = VertexSet{};
            auto L = VertexSet{};

            for (int k = 0; k <= 4; ++k) {
                if (i & (1 << k)) J.insert(k + 1);
                if (j & (1 << k)) L.insert(k + 1);
            }

            vector<Vertex> result{};
            set_intersection(J.begin(), J.end(), L.begin(), L.end(), back_inserter(result));
            if (!result.empty()) continue;

            auto K = dSeparation(D, J, L);
            
            if (K.empty()) continue;
            prettyPrint(cout, J, dSeparation(D, J, L), L);
        }
    }

    return 0;
}