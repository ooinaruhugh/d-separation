#ifndef STAR_SEPARATION_HPP
#define STAR_SEPARATION_HPP
#include "graph.hpp"

#include <iostream>

std::set<Vertex> starSeparation (
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
    out << "{" << J << "} _||_ {" << K << "} | {" << L << "}" << std::endl;
}

inline void compactPrettyPrint(
    std::ostream &out,
    const VertexSet& J,
    const VertexSet& K,
    const VertexSet& L
) {
    out << "{{" << J << "}, {" << K << "}, {" << L << "}}" << std::endl;
}

void (*printIndependenceStatement)(
    std::ostream &, 
    const VertexSet&,
    const VertexSet&,
    const VertexSet&
) = prettyPrint;

#endif