#ifndef D_SEPARATION_HPP
#define D_SEPARATION_HPP
#include "graph.hpp"

#include <iostream>

std::set<Vertex> restrictedBFS(
    const Digraph &D,
    const std::set<std::pair<Edge, Edge>> &illegal_edges,
    const std::set<Vertex> &J
);

std::set<Vertex> dSeparation (
    const Digraph &D,
    const std::set<Vertex> &J,
    const std::set<Vertex> &L
);

inline void prettyPrint(
    std::ostream &out,
    const VertexSet& J,
    const VertexSet& K,
    const VertexSet& L
) {
    out << "J={" << J << "} is d-separated from K={" << K << "} by L={" << L << "}" << std::endl;
}

inline void compactPrettyPrint(
    std::ostream &out,
    const VertexSet& J,
    const VertexSet& K,
    const VertexSet& L
) {
    out << "{" << J << "} _||_ {" << K << "} | {" << L << "}" << std::endl;
}


#endif