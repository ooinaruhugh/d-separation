#ifndef STAR_SEPARATION_HPP
#define STAR_SEPARATION_HPP
#include "graph.hpp"

extern "C" VertexSet starSeparation (
    const Digraph &D,
    const VertexSet &J,
    const VertexSet &L
);

#endif