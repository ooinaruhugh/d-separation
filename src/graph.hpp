#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>

#include <map>
#include <set>
#include <ranges>

using Vertex = int;
using VertexSet = std::set<Vertex>;
using Edge = std::pair<Vertex,Vertex>;
using EdgeSet = std::set<Edge>;
using AdjacencyList = std::map<Vertex,std::set<Vertex>>;

struct Digraph {
    AdjacencyList E;

    Digraph() {}

    Digraph(int n) {
        for (int i = 1; i <= n; ++i) {
            E.try_emplace(i);
        }
    }

    VertexSet V() const {
        auto kv = std::views::keys(E);
        return std::set<Vertex>{ kv.begin(), kv.end() };
    }

    void addVertex(Vertex v) {
        E.try_emplace(v);
    }

    void addEdge(Vertex s, Vertex t) {
        E[s].insert(t);
    }

    EdgeSet outgoingEdges(Vertex v) const {
        EdgeSet output;
        for (Vertex u : E.at(v)) {
            output.emplace(v, u);
        }

        return output;
    }

    std::map<Vertex, VertexSet> inLists() const {
        std::map<Vertex, VertexSet> output;
        for (auto v : V()) output[v];

        for (auto s : V()) {
            for (auto t : E.at(s)) 
                output[t].emplace(s);
        }

        return output;
    }

    bool isEdge(Vertex s, Vertex t) const {
        return E.contains(s) ? E.at(s).contains(t) : false;
    }

    void printAdjacencyList() const {
        for (auto s :V()) {
            std::cout << s << " -> ";
            for (auto t : E.at(s)) 
                std::cout << t << " ";
            std::cout << std::endl;
        }
    }
};

struct Graph : Digraph {
    void addEdge(Vertex s, Vertex t) {
        E[s].insert(t);
        E[t].insert(s);
    }
};


inline void printVertexSet(const VertexSet V) {
    for (auto v : V) std::cout << v << " ";
    std::cout << std::endl;
}

inline std::ostream& operator<<(std::ostream &out, const VertexSet &V) {
    for (auto v : V) out << v << ",";
    if (V.size() > 0) out << "\b \b";
    return out;
}

inline std::ostream& operator<<(std::ostream &out, const Edge &e) {
    return out << e.first << " -> " << e.second;
}

inline void printEdgeSet(const EdgeSet E) {
    for (auto [s,t] : E) 
        std::cout << s << " -> " << t << ", ";
    std::cout << std::endl;
}


#endif