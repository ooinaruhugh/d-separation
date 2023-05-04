import { Module } from "../wasm/MarkovTypes";

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

export default class State {
    _editMode: EditMode = "addNode";
    _separationMode: SeparationMode = "d";
    
    _nodes: Array<Node> = [];

    private _theGraph : Module.Digraph;
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

    constructor(originX, originY) {
        this._originX = originX;
        this._originY = originY;
    }

    // addNode (x, y) {
    //     this._nodes.push(new Node(this._nodes.length, x, y))
    // }

    // addArrow(s: Node, t: Node) {
    //     s.addNeighbor(t);
    // }

    get nodes() {
        return this._nodes;
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

    updateSeparationStatement() {
        
    }
}