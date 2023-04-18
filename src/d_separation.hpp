#ifndef D_SEPARATION_HPP
#define D_SEPARATION_HPP
#include "graph.hpp"

std::set<Vertex> restrictedBFS(
    Digraph D,
    std::set<std::pair<Edge, Edge>> illegal_edges,
    std::set<Vertex> J
);

std::set<Vertex> dSeparation (
    Digraph D,
    std::set<Vertex> J,
    std::set<Vertex> L
);

#endif