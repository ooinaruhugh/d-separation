import "emscripten";

export type Vertex = number;
export type Edge = [Vertex, Vertex];

declare namespace Module {
    export class VertexSet {
        length: number;
        clear(): void;

        add(v: Vertex): void;
        remove(v: Vertex): void;
        toView(): Array<number>;
    }

    export function viewFromVertexSet(S: VertexSet): Array<number>;
    export function printVertexSet(S: VertexSet): void;
    export function printEdgeSet(S: EdgeSet): void;

    export class EdgeSet {
        length: number;
        clear(): void;
    }

    export class VertexVector {
        size(): number;
        resize(size: number, v: Vertex): void;
        get(index: number): Vertex;
        set(index: number, v: Vertex): boolean;
    }

    export class EdgeVector {
        size(): number;
        resize(size: number, v: Edge): void;
        get(index: number): Edge;
        set(index: number, v: Edge): boolean;
    }

    export class AdjacencyList {
        size(): number;
        get(index: number): VertexSet;
        set(index: number, v: VertexSet): boolean;
        keys(): VertexVector;
    }

    export class Digraph {
        V() : Array<number>;
        Vcount() : number;

        E : AdjacencyList;

        addVertex(v: Vertex): void;
        addEdge(s: Vertex, t: Vertex): void;

        removeVertex(v: Vertex): void;
        removeEdge(s: Vertex, t: Vertex): void;

        outgoingEdges(v: Vertex): Emscripten.CPointerType;
        inLists(): Emscripten.CPointerType;

        isEdge(s: Vertex, t: Vertex): boolean;
        printAdjacencyList(): void;
    }

    export function dSeparation(D: Digraph, J: VertexSet, L: VertexSet): Array<number>;
    export function starSeparation(D: Digraph, J: VertexSet, L: VertexSet): Array<number>;
}