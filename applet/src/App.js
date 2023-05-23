import './App.css';

// import loadMarkovModule from './wasm/markov.mjs';

import {MarkovPromise, State} from './mvvc/State.ts';
import CheckedValues from './mvvc/StateReactBindings.ts';

import React from 'react';
import { 
  Toolbar,
  ToolbarButton,
  ToolbarRadioButton,
  ToolbarRadioGroup,
  ToolbarDivider,
  Menu,
  MenuTrigger,
  MenuPopover,
  MenuList,
  MenuItem,
  ToolbarProps,
  Button, 
  FluentProvider,
  webDarkTheme,
  webLightTheme
} from '@fluentui/react-components';
import {
  AddCircle24Regular,
  ArrowStepInRight24Regular,
  HandDraw24Regular,
  Cursor24Regular,
  ArrowCounterclockwise24Regular
} from '@fluentui/react-icons';

let uiState = new State();

function redraw(canvas) {
  var ctx = canvas.getContext("2d");
  ctx.clearRect(0,0, canvas.width, canvas.height);

  for (let [node, record] of uiState.nodes) {
    if (record.tag === "J") {
      ctx.strokeStyle = "blue"
    } else if (record.tag === "K") {
      ctx.strokeStyle = "red"
    } else if (record.tag === "L") {
      ctx.strokeStyle = "orange"
    } else {
      ctx.strokeStyle = "black"
    }

    ctx.strokeRect(
      record.x + uiState.originX - 25, 
      record.y + uiState.originY - 25, 
      50, 50
    );
  }
  
  for (let e of uiState.edges) {
    let [s,t] = e.map(v => uiState._coords.get(v));
    console.log(s,t);

    ctx.beginPath();
    ctx.strokeStyle = "black";

    ctx.moveTo(s.x + uiState.originX, s.y + uiState.originY);
    ctx.lineTo(t.x + uiState.originX, t.y + uiState.originY);
    ctx.stroke();  
  }
}

function collisionDetection(e, bounding, callback) {
  const x = e.x - bounding.left - uiState.originX;
  const y = e.y - bounding.top - uiState.originY;

  const nearestNeighbor = uiState.findNode(x, y);
  if (nearestNeighbor !== null) {
    return callback(nearestNeighbor);
  }

  return null;
}

function App() {
  // const [uiState, ] = React.useState(new State());
  const checkedValues = new CheckedValues(uiState);
  
  // For panning
  const [_isMousedown, _setIsMousedown] = React.useState(false);
  const isMousedown = React.useRef(_isMousedown);
  const setIsMousedown = (data) => {
    isMousedown.current = data;
    _setIsMousedown(data);
  };

  // For adding an arrow between two nodes
  const [_firstVertexForEdge, _setFirstVertexForEdge] = React.useState(null);
  const firstVertexForEdge = React.useRef(_firstVertexForEdge);
  const setFirstVertexForEdge = (data) => {
    firstVertexForEdge.current = data;
    _setFirstVertexForEdge(data);
  }

  const onChange = (
    e,
    {name, checkedItems}
  ) => {
    uiState[name] = checkedItems[0];
  };

  React.useEffect(() => {
    console.log(uiState);

    var c = document.getElementById("viewport");
    var ctx = c.getContext("2d");

    uiState.originX = c.clientWidth / 2;
    uiState.originY = c.clientHeight / 2;

    MarkovPromise.then(() => {
      // For secondary-button click
      c.addEventListener("contextmenu", (e) => {
        if (uiState.editMode === "selectSubsets") {
          let bounding = c.getBoundingClientRect();

          collisionDetection(e, bounding, v => {
            uiState.tagNode(
              v.id,
              v.tag === "L" ? "" : "L"
            );

            uiState.updateSeparationStatement();
          });

          redraw(c);
          e.preventDefault();
        }
        return uiState.editMode !== "selectSubsets"
      });

      c.addEventListener("click", (e) => {
        let bounding = c.getBoundingClientRect();
        
        if (uiState.editMode === "addNode") {
          const overlap = collisionDetection(e, bounding, () => true);

          if (overlap !== true) {
            uiState.addNode(
              e.x - bounding.left - uiState.originX, 
              e.y - bounding.top - uiState.originY
            );
              
            ctx.canvas.width = c.clientWidth;
            ctx.canvas.height = c.clientHeight;

            uiState.updateSeparationStatement();
            redraw(c);
          }
        } else if (uiState.editMode === 'addEdge') {
          if (firstVertexForEdge.current === null) {
            collisionDetection(e, bounding, v => setFirstVertexForEdge(v));
          } else {
            collisionDetection(e, bounding, v => {
              if (v.id !== firstVertexForEdge.current.id) {
                uiState.addArrow(firstVertexForEdge.current.id, v.id);

                setFirstVertexForEdge(null);

                uiState.updateSeparationStatement();
                redraw(c);
              }
            });
          }
        } else if (uiState.editMode === "selectSubsets") {
          collisionDetection(e, bounding, v => {
            uiState.tagNode(
              v.id,
              v.tag === "J" ? "" : "J"
            );
            
            uiState.updateSeparationStatement();
          });

          redraw(c);
        }
      });

      c.addEventListener('mousedown', (e) => {
        setIsMousedown(true);
      });
  
      c.addEventListener('mousemove', (e) => {
        if (isMousedown.current && (uiState.editMode === "moveCanvas")) {
          uiState.originX += e.movementX;
          uiState.originY += e.movementY;
  
          redraw(c);
        }
      });
  
      c.addEventListener('mouseup', (e) => {
        setIsMousedown(false);
      });

      document.getElementById("debugRefresh").addEventListener("click", () => {
        uiState.updateSeparationStatement();
        redraw(c);
      });
    });

    // MarkovPromise.then((Module) => {
    //   const D = new Module.Digraph(5);
    //   D.addEdge(1,4);
    //   D.addEdge(2,4);
    //   D.addEdge(2,5);
    //   D.addEdge(3,5);
  
    //   const V = D.V();
    //   console.log([...V]);
    //   Module.free(V);
    //   D.printAdjacencyList();
  
    //   const J = new Module.VertexSet();
    //   J.add(1);
  
    //   const L = new Module.VertexSet();
    //   L.add(4);
    //   L.add(5);
  
    //   const K = Module.starSeparation(D, J, L);
    //   const _J = J.toView();
    //   const _L = L.toView();
  
    //   console.log([..._J], "is star-separated from", [...K], "by", [..._L]);
  
    //   Module.free(J);
    //   // Module.free(_J);
    //   Module.free(L);
    //   Module.free(_L);
    // })
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []); // Leave empty so it only gets called once...

  return (
    <div className="App">
      <FluentProvider theme={webLightTheme} className='toolbar'>
        <Toolbar aria-label="Default" onCheckedValueChange={onChange} checkedValues={checkedValues}>
          <ToolbarRadioGroup>
            <ToolbarRadioButton
              aria-label="Move canvas"
              name="editMode"
              value="moveCanvas"
              appearance='subtle'
              icon={<Cursor24Regular />}
            />
            <ToolbarRadioButton
              aria-label="Add node"
              name="editMode"
              value="addNode"
              appearance='subtle'
              icon={<AddCircle24Regular />}
            />
            <ToolbarRadioButton
              aria-label="Add edge"
              name="editMode"
              value="addEdge"
              appearance='subtle'
              icon={<ArrowStepInRight24Regular />}
            />
            <ToolbarRadioButton
              aria-label="Select subsets"
              name="editMode"
              value="selectSubsets"
              appearance='subtle'
              icon={<HandDraw24Regular />}
            />
          </ToolbarRadioGroup>
          <ToolbarDivider/>
          <ToolbarRadioGroup>
            <ToolbarRadioButton
              aria-label="Select d-separation mode"
              name="separationMode"
              value="d"
              appearance='subtle'
              icon="d"
            />
            <ToolbarRadioButton
              aria-label="Select star-separation mode"
              name="separationMode"
              value="star"
              appearance='subtle'
              icon="&#x2731;"
            />
          </ToolbarRadioGroup>
          <ToolbarButton
            aria-label = "Refresh separation"
            id="debugRefresh"
            icon={<ArrowCounterclockwise24Regular/>}
          />
        </Toolbar>
      </FluentProvider>
      <canvas 
        id="viewport"></canvas>
    </div>
  );
}

export default App;
