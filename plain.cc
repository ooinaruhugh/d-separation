#include <map>
#include <vector>
#include <set>
#include <ranges>
#include <algorithm>

#include <iostream>

using namespace std;

using Vertex = int;
using Edge = pair<Vertex,Vertex>;
using AdjacencyList = map<Vertex,set<Vertex>>;

void printVertexSet(set<Vertex> V) {
    for (auto v : V) cout << v << " ";
    cout << endl;
}

void printEdgeSet(set<Edge> E) {
    for (auto [s,t] : E) 
        cout << s << " -> " << t << ", ";
    cout << endl;
}

struct Digraph {
    AdjacencyList E;

    Digraph() {

    }

    Digraph(int n) {
        for (int i = 1; i <= n; ++i) {
            E.try_emplace(i);
        }
    }

    set<Vertex> V() {
        auto kv = views::keys(E);
        return set<Vertex>{ kv.begin(), kv.end() };
    }

    void addVertex(Vertex v) {
        E.try_emplace(v);
    }

    void addEdge(Vertex s, Vertex t) {
        E[s].insert(t);
    }

    set<Edge> outgoingEdges(Vertex v) {
        set<Edge> output;
        for (Vertex u : E[v]) {
            output.emplace(v, u);
        }

        return output;
    }

    map<Vertex, set<Vertex>> inLists() {
        map<Vertex, set<Vertex>> output;
        for (auto v : V()) output[v];

        for (auto s : V()) {
            for (auto t : E[s]) 
                E[t].emplace(s);
        }

        return output;
    }

    void printAdjacencyList() {
        for (auto s :V()) {
            cout << s << " -> ";
            for (auto t : E[s]) 
                cout << t << " ";
            cout << endl;
        }
    }
};

struct Graph : Digraph {
    void addEdge(Vertex s, Vertex t) {
        E[s].insert(t);
        E[t].insert(s);
    }
};

set<Vertex> restrictedBFS(
    Digraph D,
    set<pair<Edge, Edge>> illegal_edges,
    set<Vertex> J
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
        // 3. Find all unlabeled edges adjacent to at least one edge such that (u->v,v->w) is legal...
        for (auto [s, t] : frontier) {
            R.emplace(t);

            // *Find* all...
            for (Edge e : D.outgoingEdges(t)) {
                // **unlabeled**
                if (visited.contains(e)) continue;
                // ... such that ... *legal pair*...
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

set<Vertex> dSeparation (
    Digraph D,
    set<Vertex> J,
    set<Vertex> L
) {
    // 1. Construct the table `descendent`.
    auto inLists = D.inLists();
    set<Vertex> descendent;
    for (auto l : L) {
        descendent.emplace(l);

        auto& ancestors = inLists[l];
        descendent.insert(ancestors.begin(), ancestors.end());
    }

    // 2. Construct the directed graph D' adding the flipped edges.
    auto D_prime{D};
    for (auto s : D.V()) {
        for (auto t : D.E[s])
            D_prime.addEdge(t,s);
    }

    // 3a. Construct the illegal list of edges.
    set<pair<Edge, Edge>> illegal_edges;
    for (auto s : D.V()) {
        for (auto t : D.E[s]) {
            illegal_edges.emplace(Edge{s,t}, Edge{t,s});

            // Non-colliders
            for (auto u : D.E[t]) {
                if (L.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
                }
            }
            // Colliders
            for (auto u : inLists[t]) {
                if (!L.contains(t)) {
                    illegal_edges.emplace(Edge{s,t}, Edge{t,u});
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

int main() {
    Digraph D{5};
    D.addEdge(1,4);
    D.addEdge(2,4);
    D.addEdge(2,5);
    D.addEdge(3,5);

    printVertexSet(restrictedBFS(D, set<pair<Edge, Edge>>(), set<Vertex>{1}));
    printVertexSet(dSeparation(D, {1}, {4}));

    return 0;
}