#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <ranges>

#include "graph.hpp"
#include "d_separation.hpp"
#include "star_separation.hpp"

using namespace emscripten;

EMSCRIPTEN_KEEPALIVE
void addToSet(VertexSet& V, Vertex v) {
    V.emplace(v);
}

EMSCRIPTEN_KEEPALIVE
void removeFromSet(VertexSet& V, Vertex v) {
    V.erase(v);
}

template <typename T> EMSCRIPTEN_KEEPALIVE
val viewFromSet(std::set<T> S) {
#ifdef __cpp_lib_ranges
    auto buffer = std::ranges::ref_view(S);
#else
    T* buffer = new T[S.size()];

    int i = 0;
    for (auto x : S) {
        buffer[i] = x;
    }
#endif
    return val(typed_memory_view(
        S.size(),
        buffer
    ));
}

EMSCRIPTEN_BINDINGS(graph) {
    class_<Digraph>("Digraph")
        .constructor<int>()
        .function("V", &Digraph::V)
        .function("addVertex", &Digraph::addVertex)
        .function("addEdge", &Digraph::addEdge)
        .function("outgoingEdges", &Digraph::outgoingEdges)
        .function("inLists", &Digraph::inLists)
        .function("isEdge", &Digraph::isEdge)
        .function("printAdjacencyList", &Digraph::printAdjacencyList)
    ;

    class_<VertexSet>("VertexSet")
        .constructor<>()
        .property("length", &VertexSet::size)
        .function("clear", &VertexSet::clear)
    ;

    function("addToSet", addToSet);
    function("removeFromSet", removeFromSet);

    function("viewFromVertexSet", viewFromSet<Vertex>);

    function("printVertexSet", printVertexSet);
    function("printEdgeSet", printEdgeSet);
}

EMSCRIPTEN_BINDINGS(algorithms) {
    function("dSeparation", dSeparation);
    function("starSeparation", starSeparation);
}