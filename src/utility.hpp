#ifndef UTILITY_HPP
#define UTILITY_HPP
#include "graph.hpp"

#include <iostream>

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

// void (*printIndependenceStatement)(
//     std::ostream &, 
//     const VertexSet&,
//     const VertexSet&,
//     const VertexSet&
// ) = prettyPrint;

#define printIndependenceStatement prettyPrint

#endif