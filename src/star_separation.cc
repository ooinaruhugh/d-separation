#include <map>
#include <vector>
#include <set>
#include <ranges>
#include <algorithm>

#include <iostream>

#include "graph.hpp"
#include "utility.hpp"

using namespace std;

using TaggedEdge = pair<Edge, bool>;

VertexSet starReachability(
    const Digraph &D,
    const set<pair<Edge, Edge>> &illegal_edges,
    const VertexSet &J
) {
    set<Vertex> R;
    set<TaggedEdge> frontier, next_frontier, visited;

    auto D_prime{D};

    // 1. Add a dummy vertex for each node j in J.
    for (Vertex j : J) {
        D_prime.addVertex(-j);
        D_prime.addEdge(-j, j);

        frontier.emplace(
            make_pair(-j, j),
            false
        );

        // cerr << "Adding the dummy edge " << Edge{-j,j} << " to the frontier queue." << endl;

        R.emplace(j);
        R.emplace(-j);
    }

    // 2. Construct the directed graph D' adding the flipped edges.
    // (Move this step here from the other algorithm, so this algorithm knows the original D)
    for (auto s : D.V()) {
        for (auto t : D.E.at(s))
            D_prime.addEdge(t,s);
    }

    while (true) {
        // 3. Find all unlabeled edges adjacent to at least one edge such that (u->v, v->w) is legal...
        for (auto [e, passedACollider] : frontier) {
            auto [s,t] = e;

            R.emplace(t);

            // Check whether [s,t] is an "s->t" edge or an "s<-t" edge
            bool s_to_t = s > 0 ? D.isEdge(s,t) : true;

            // *Find* all...
            for (Edge f : D_prime.outgoingEdges(t)) {
                if(f.first != t) throw exception();


                auto [_, u] = f;
                auto u_to_t = D.isEdge(u, t);
                bool t_is_collider = s > 0 ? (s_to_t && u_to_t) : false;

                // cerr << "Checking the pair s="
                //      << s
                //      << (s_to_t ? " -> " : " <- ")
                //      << "t="
                //      << t
                //      << (u_to_t ? " <- " : " -> ")
                //      << "u="
                //      << u
                //      << (passedACollider 
                //             ? " that passed a collider already."
                //             : " that did not pass a collider already.")
                //      << endl;

                if (t_is_collider && passedACollider) continue;
                TaggedEdge F{f, passedACollider || t_is_collider};

                // **unlabeled**
                if (visited.contains(F))
                    continue;

                // ... such that (s->t, e = t->...) is a *legal pair*.
                if (!illegal_edges.contains(make_pair(Edge{s,t}, f)))
                    next_frontier.emplace(F);
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

VertexSet starSeparation (
    const Digraph &D,
    const VertexSet &J,
    const VertexSet &L
) {
    // 1. Construct the table `descendent`.
    auto inLists = D.inLists();
    set<Vertex> descendent;
    for (auto v : L) {
        descendent.emplace(v);

        auto& ancestors = inLists[v];
        descendent.insert(ancestors.begin(), ancestors.end());
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
            // Handle all the (outgoing) edges s -> t
            for (auto u : D.E.at(t)) {
                // Non-colliders (case: s -> t -> u, if t is in L, this is illegal)
                if (L.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                }
            }

            for (auto u : inLists[t]) {
                // Colliders (case: s -> t <- u, if t is *not* `descendent`, this is illegal)
                if (!descendent.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                }
            }
        }

        for (auto t : inLists[s]) {
            // Handle all the (incoming) edges s <- t
            for (auto u : D.E.at(t)) {
                // Non-colliders (case: s <- t -> u, if t is `descendent`, this is illegal)
                if (L.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                }
            }
            for (auto u : inLists[t]) {
                // Non-collider (case: s <- t <- u, if t is `descendent`, this is illegal)
                if (L.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                }
            }
        }
    }

    // 3. Using algorithm 1, ...
    auto K_prime = starReachability(D, illegal_edges, J);

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