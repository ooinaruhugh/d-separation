import { Module, Vertex } from "../wasm/MarkovTypes";
import loadMarkovModule from '../wasm/markov.mjs';
import { kdTree } from "kd-tree-javascript";

export const MarkovPromise = loadMarkovModule({
	noInitialRun: true,
	noExitRuntime: true
});

type SubsetTag = "" | "J" | "K" | "L";
type TaggedVertex = {
    x: number,
    y: number,
    id: number,
    tag: SubsetTag
}

const editModeNames = [
    "moveCanvas",
    "addNode",
    "addEdge",
    "selectSubsets"
] as const;
type EditMode = typeof editModeNames[number];

const separationModeNames = [
    "d",
    "star"
] as const;
type SeparationMode = typeof separationModeNames[number];

function* filter(iterable, predicate) {
    var i = 0;
    for (var item of iterable)
        if (predicate(item, i++))
             yield item;
}

export class State {
    _editMode: EditMode = "addNode";
    _separationMode: SeparationMode = "d";
    
    _coords: Map<Vertex,TaggedVertex>;
    _kdtree: kdTree<TaggedVertex>;

    _edges: Set<[Vertex, Vertex]>;

    private _theGraph : Module.Digraph | null;
    public get theGraph() : Module.Digraph {
        return this._theGraph;
    }
    public set theGraph(D : Module.Digraph) {
        this._theGraph = D;
    }
    
    private _originX: Number;
    public get originX(): Number {
        return this._originX;
    }
    public set originX(value: Number) {
        this._originX = value;
    }
    private _originY: Number;
    public get originY(): Number {
        return this._originY;
    }
    public set originY(value: Number) {
        this._originY = value;
    }

    constructor(originX: number, originY: number) {
        this._originX = originX;
        this._originY = originY;

        this._coords = new Map<Vertex, TaggedVertex>();
        this._kdtree = new kdTree<TaggedVertex>(
            [],
            (a, b) => Math.sqrt(
                Math.pow(a.x - b.x, 2) 
                + Math.pow(a.y - b.y, 2)),
            // (a, b) => Math.max(Math.abs(a.x - b.x), Math.abs(a.y - b.y)),
            // (a, b) => Math.abs(a.x - b.x) + Math.abs(a.y - b.y),
            ["x", "y"]
        );

        this._theGraph = null;
        MarkovPromise.then(Markov => this._theGraph = new Markov.Digraph(0));

        this._edges = new Set();
    }

    addNode (x:number, y:number) {
        if (this._theGraph !== null) {
            const id = this._theGraph.Vcount() + 1;

            this._theGraph.addVertex(id);
            let tagRef: TaggedVertex = {
                x: x,
                y: y,
                id: id,
                tag: ""
            };
            this._kdtree.insert(tagRef);
            this._coords.set(id, tagRef);

            // console.log(this._coords);
            // console.log(this._kdtree);
        }
    }

    removeNode(id: number) {
        this._theGraph.removeVertex(id);
        let record = this._coords.get(id);

        this._kdtree.remove(record);
        this._coords.delete(id);
    }

    addArrow(s: number, t: number) {
        this._theGraph.addEdge(s, t);
        this._edges.add([s,t]);
    }

    get nodes() {
        return this._coords;
    }

    get edges() {
        return this._edges;
    }

    get editMode() {
        return this._editMode;
    }

    set editMode(name) {
        if (editModeNames.includes(name)) {
            this._editMode = name
        }
    }

    get separationMode() {
        return this._separationMode;
    }

    set separationMode(name) {
        if (separationModeNames.includes(name)) {
            this._separationMode = name
        }
    }

    findNode(x: number, y: number): TaggedVertex | null {
        const result = this._kdtree.nearest({
            x: x,
            y: y,
            id: 0,
            tag: ""
        }, 1, 25);

        if (result.length > 0) {
            const [[nearestNeighbor, dist]] = result;
            return nearestNeighbor;
        }

        return null;
    }

    tagNode(id: number, tag: SubsetTag) {
        if (this._coords.has(id)) this._coords.get(id).tag = tag;
    }

    updateSeparationStatement() {
        MarkovPromise.then(Markov => {
            const J: Module.VertexSet = new Markov.VertexSet();
            const L: Module.VertexSet = new Markov.VertexSet();

            for (let v of filter(this._coords.values(), (v: TaggedVertex) => v.tag === "J")) {
                J.add(v.id);
            }
            for (let v of filter(this._coords.values(), (v: TaggedVertex) => v.tag === "L")) {
                L.add(v.id);
            }

            const K: Array<number> = 
                this.separationMode === "d" 
                    ? Markov.dSeparation(this._theGraph, J, L) 
                    : Markov.starSeparation(this._theGraph, J, L);

            for (let v of filter(this._coords.values(), (v: TaggedVertex) => v.tag === "K")) {
                this.tagNode(v.id, "");
            }
            if (J.length > 0) {
                for (let k of K) {
                    this.tagNode(k, "K");
                }    
            }
            
            try {
                Markov._free(K);
            } catch (error) {
                console.log("Trying to free", K);
            }
        });
    }
}