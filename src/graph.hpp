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
using AdjacencyList = std::map<Vertex,VertexSet>;

struct Digraph {
    AdjacencyList E;

    Digraph() {}

    Digraph(int n) {
        for (int i = 1; i <= n; ++i) {
            E.try_emplace(i);
        }
    }

    VertexSet V() const {
#ifdef __cpp_lib_ranges
        auto kv = std::views::keys(E);
        return std::set<Vertex>{ kv.begin(), kv.end() };
#else
        VertexSet V; 

        for (auto const& [key, val] : E) {
            V.emplace(key);
        }

        return V;
#endif
    }

    void addVertex(Vertex v) {
        E.try_emplace(v);
    }

    void removeVertex(Vertex v) {
        E.erase(v);
    }

    void addEdge(Vertex s, Vertex t) {
        try {
            if (!E.contains(s)) throw s;
            if (!E.contains(t)) throw t;

            E.at(s).insert(t);
        } catch (std::out_of_range e) {
            std::cerr << "Vertex " << s << " does not exist." << std::endl;
        } catch (Vertex v) {
            std::cerr << "Vertex " << v << " does not exist" << std::endl;
        }
    }

    void removeEdge(Vertex s, Vertex t) {
        try {
            E[s].erase(t);
        } catch (std::out_of_range e) {
            std::cerr << "Vertex " << s << " does not exist." << std::endl;
        }
    }

    EdgeSet outgoingEdges(Vertex v) const {
        EdgeSet output;
        for (Vertex u : E.at(v)) {
            output.emplace(v, u);
        }

        return output;
    }

    AdjacencyList inLists() const {
        AdjacencyList output;
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