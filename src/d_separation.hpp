#ifndef D_SEPARATION_HPP
#define D_SEPARATION_HPP
#include "graph.hpp"

extern VertexSet dSeparation (
    const Digraph &D,
    const VertexSet &J,
    const VertexSet &L
);

#endif