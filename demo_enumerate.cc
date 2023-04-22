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
#include "utility.hpp"
#include "d_separation.hpp"
#include "star_separation.hpp"

using namespace std;

int main() {
    Digraph D{5};
    D.addEdge(1,4);
    D.addEdge(2,4);
    D.addEdge(2,5);
    D.addEdge(3,5);

    auto d_results = vector<VertexSet>(), star_results = vector<VertexSet>();

    // Hacky-hacky enumerate all subsets of {1,...,5} ...
    for (int i = 1; i < 32; ++i) {
        for (int j = 1; j < 32; ++j) {
            auto J = VertexSet{};
            auto L = VertexSet{};

            for (int k = 0; k <= 4; ++k) {
                if (i & (1 << k)) J.insert(k + 1);
                if (j & (1 << k)) L.insert(k + 1);
            }

            // ... and filter the non-disjoint pairs.
            vector<Vertex> result{};
            set_intersection(J.begin(), J.end(), L.begin(), L.end(), back_inserter(result));
            if (!result.empty()) continue;

            auto K = dSeparation(D, J, L);
            auto K_star = starSeparation(D, J, L);
            
            if (K.empty() && K_star.empty()) continue;
            
            cout << "d: "; printIndependenceStatement(cout, J, K, L);
            cout << "*: "; printIndependenceStatement(cout, J, K_star, L);
                
            if (!K.empty()) d_results.emplace_back(K);
            if (!K_star.empty()) star_results.emplace_back(K_star);
        }
    }

    cout << "Number of non-trivial d-separation statements: " << d_results.size() << endl;
    cout << "Number of non-trivial *-separation statements: " << star_results.size() << endl;

    return 0;
}