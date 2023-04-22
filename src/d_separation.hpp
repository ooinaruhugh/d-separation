#ifndef D_SEPARATION_HPP
#define D_SEPARATION_HPP
#include "graph.hpp"

std::set<Vertex> dSeparation (
    const Digraph &D,
    const std::set<Vertex> &J,
    const std::set<Vertex> &L
);

#endif