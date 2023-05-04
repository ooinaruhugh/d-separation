#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <ranges>

#include "graph.hpp"
#include "d_separation.hpp"
#include "star_separation.hpp"

using namespace emscripten;

EMSCRIPTEN_KEEPALIVE
int testMe() {
    return 1;
}

template <typename T> EMSCRIPTEN_KEEPALIVE
void addToSet(std::set<T>& V, T v) {
    V.emplace(v);
}

template <typename T> EMSCRIPTEN_KEEPALIVE
void removeFromSet(std::set<T>& V, T v) {
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
        buffer[i++] = x;
    }
#endif
    return val(typed_memory_view(
        S.size(),
        buffer
    ));
}

template <typename T> EMSCRIPTEN_KEEPALIVE
val viewFromSetRef(const std::set<T>& S) {
#ifdef __cpp_lib_ranges
    auto buffer = std::ranges::ref_view(S);
#else
    T* buffer = new T[S.size()];

    int i = 0;
    for (auto x : S) {
        buffer[i++] = x;
    }
#endif
    return val(typed_memory_view(
        S.size(),
        buffer
    ));
}

EMSCRIPTEN_KEEPALIVE
size_t Vcount(const Digraph& D) {
    return D.E.size();
}

EMSCRIPTEN_KEEPALIVE
val _V(const Digraph& D) {
    return viewFromSetRef(D.V());
}

EMSCRIPTEN_BINDINGS(graph) {
    class_<Digraph>("Digraph")
        .constructor<int>()
        .function("V", _V)
        .property("E", &Digraph::E)
        .function("Vcount", Vcount)
        .function("addVertex", &Digraph::addVertex)
        .function("addEdge", &Digraph::addEdge)
        .function("removeVertex", &Digraph::removeVertex)
        .function("removeEdge", &Digraph::removeEdge)
        .function("outgoingEdges", &Digraph::outgoingEdges)
        .function("inLists", &Digraph::inLists)
        .function("isEdge", &Digraph::isEdge)
        .function("printAdjacencyList", &Digraph::printAdjacencyList)
    ;

    value_array<Edge>("Edge")
        .element(&Edge::first)
        .element(&Edge::second)
    ;

    class_<VertexSet>("VertexSet")
        .constructor<>()
        .property("length", &VertexSet::size)
        .function("clear", &VertexSet::clear)
        .function("add", addToSet<Vertex>)
        .function("remove", removeFromSet<Vertex>)
        .function("toView", viewFromSetRef<Vertex>);
    ;

    class_<EdgeSet>("EdgeSet")
        .constructor<>()
        .property("length", &EdgeSet::size)
        .function("clear", &EdgeSet::clear)
    ;
    
    register_vector<Vertex>("VertexVector");
    register_vector<Edge>("EdgeVector");
    register_map<Vertex, VertexSet>("AdjacencyList");

    function("viewFromVertexSet", viewFromSet<Vertex>);

    function("printVertexSet", printVertexSet);
    function("printEdgeSet", printEdgeSet);

    function("testMe", testMe);
}

val _dSeparation (
    const Digraph &D,
    const VertexSet &J,
    const VertexSet &L
) {
    return viewFromSet(dSeparation(D,J,L));
}

val _starSeparation (
    const Digraph &D,
    const VertexSet &J,
    const VertexSet &L
) {
    return viewFromSet(starSeparation(D,J,L));
}

EMSCRIPTEN_BINDINGS(algorithms) {
    function("dSeparation", _dSeparation);
    function("starSeparation", _starSeparation);
}