#include <map>
#include <vector>
#include <set>
#include <ranges>
#include <algorithm>

#include <iostream>

#include "graph.hpp"

using namespace std;

set<Vertex> restrictedBFS(
    const Digraph &D,
    const set<pair<Edge, Edge>> &illegal_edges,
    const set<Vertex> &J
) {
    set<Vertex> R;
    set<Edge> frontier, next_frontier, visited;

    auto D_prime{D};

    // 1. Add a dummy vertex for each node j in J.
    for (Vertex j : J) {
        D_prime.addVertex(-j);
        D_prime.addEdge(-j, j);

        frontier.emplace(-j, j);

        R.emplace(j);
        R.emplace(-j);
    }

    while (true) {
        // 3. Find all unlabeled edges adjacent to at least one edge such that (u->v, v->w) is legal...
        for (auto [s, t] : frontier) {
            R.emplace(t);

            // *Find* all...
            for (Edge e : D.outgoingEdges(t)) {
                // **unlabeled**
                if (visited.contains(e)) continue;
                // ... such that (s->t, e = t->...) is a *legal pair*.
                if (!illegal_edges.contains(make_pair(Edge{s,t}, e)))
                    next_frontier.emplace(e);
            }
        }

        visited.insert(frontier.begin(), frontier.end());

        if (next_frontier.empty()) {
            return R;
        }

        frontier = next_frontier;
        next_frontier.clear();
    }
}

// Calculates a maximal set K that is d-separated from J by L.
set<Vertex> dSeparation (
    const Digraph &D,
    const set<Vertex> &J,
    const set<Vertex> &L
) {    
    // 1. Construct the table `descendent`.
    auto inLists = D.inLists();
    set<Vertex> descendent;
    for (auto v : L) {
        descendent.emplace(v);

        auto& ancestors = inLists[v];
        descendent.insert(ancestors.begin(), ancestors.end());
    }

    // 2. Construct the directed graph D' adding the flipped edges.
    auto D_prime{D};
    for (auto s : D.V()) {
        for (auto t : D.E.at(s))
            D_prime.addEdge(t,s);
    }

    // 3a. Construct the illegal list of edges.
    set<pair<Edge, Edge>> illegal_edges;
    for (auto s : D.V()) {
        for (auto t : D.V()) {
            illegal_edges.emplace(Edge{s,t}, Edge{t,s});
        }
    }

    for (auto s : D.V()) {
        for (auto t : D.E.at(s)) {
            // Non-colliders (case: s -> t -> u, if t is `descendent`, this is illegal)
            // cerr << Edge{s,t} << endl;
            for (auto u : D.E.at(t)) {
                if (descendent.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                    // cerr << "Add " << s << " -> " << t << " -> " << u << endl;
                } else {
                    // cerr << "Don't add " << s << " -> " << t << " -> " << u << endl;
                }
            }
            // Colliders (case: s -> t <- u, if t is *not* `descendent`, this is illegal)
            for (auto u : inLists[t]) {
                if (!descendent.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                    // cerr << "Add " << s << " -> " << t << " <- " << u << endl;
                }
            }
        }

        for (auto t : inLists[s]) {
            // cerr << Edge{t,s} << endl;
            // Non-colliders (case: s <- t -> u, if t is `descendent`, this is illegal)
            for (auto u : D.E.at(t)) {
                if (descendent.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                    // cerr << "Add " << s << " -> " << t << " -> " << u << endl;
                } else {
                    // cerr << "Don't add " << s << " -> " << t << " -> " << u << endl;
                }
            }
            // Non-collider (case: s <- t <- u, if t is `descendent`, this is illegal)
            for (auto u : inLists[t]) {
                if (descendent.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                    // cerr << "Add " << s << " -> " << t << " <- " << u << endl;
                }
            }
        }
    }

    // 3. Using algorithm 1, ...
    auto K_prime = restrictedBFS(D_prime, illegal_edges, J);

    // 4.
    auto K = D.V();
    for (auto v : K_prime) {
        K.erase(v);
    }
    for (auto v : J) {
        K.erase(v);
    }
    for (auto v : L) {
        K.erase(v);
    }

    return K;
}
