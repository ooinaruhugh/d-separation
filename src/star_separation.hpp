#ifndef STAR_SEPARATION_HPP
#define STAR_SEPARATION_HPP
#include "graph.hpp"

std::set<Vertex> starSeparation (
    const Digraph &D,
    const std::set<Vertex> &J,
    const std::set<Vertex> &L
);

#endif