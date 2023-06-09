import './App.css';

// import loadMarkovModule from './wasm/markov.mjs';

import {MarkovPromise, State} from './mvvc/State.ts';
import CheckedValues from './mvvc/StateReactBindings.ts';

import React, { useEffect } from 'react';
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
import { render } from 'react-dom';

function redraw(canvas, uiState) {
  var ctx = canvas.getContext("2d");
  ctx.clearRect(0,0, canvas.width, canvas.height);

  for (let [node, record] of uiState.nodes) {
    ctx.beginPath();

    if (record.tag === "J") {
      ctx.strokeStyle = "blue"
    } else if (record.tag === "K") {
      ctx.strokeStyle = "red"
    } else if (record.tag === "L") {
      ctx.strokeStyle = "orange"
    } else {
      ctx.strokeStyle = "black"
    }

    // ctx.strokeRect(
    //   record.x + uiState.originX - 25, 
    //   record.y + uiState.originY - 25, 
    //   50, 50
    // );
    
    ctx.ellipse(
      record.x + uiState.originX, 
      record.y + uiState.originY, 
      25, 25,
      0, 
      0, 2 * Math.PI
    );
    ctx.stroke();

  }
  
  for (let e of uiState.edges) {
    let [s,t] = e.map(v => uiState._coords.get(v));

    ctx.beginPath();
    ctx.strokeStyle = "black";

    let dist = Math.sqrt(
      Math.pow(s.x - t.x, 2) + Math.pow(s.y - t.y, 2)
    );

    const lineCoords = (a) => [
      (1 - a) * s.x + a * t.x + uiState.originX,
      (1 - a) * s.y + a * t.y + uiState.originY
    ];

    ctx.moveTo(...lineCoords(25/dist));
    ctx.lineTo(...lineCoords(1 - (25/dist)));
    ctx.stroke();

    // Arrow heads
    const normal = (() => {
      const x = t.x - s.x;
      const y = t.y - s.y;
      return [5 * y/dist, -5 * x/dist]; 
    })();
    const arrowHeadEnd = lineCoords(1 - (45/dist));

    ctx.beginPath();
    ctx.moveTo(...lineCoords(1 - (25/dist)));
    ctx.lineTo(normal[0] + arrowHeadEnd[0], normal[1] + arrowHeadEnd[1]);
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(...lineCoords(1 - (25/dist)));
    ctx.lineTo(-normal[0] + arrowHeadEnd[0], -normal[1] + arrowHeadEnd[1]);
    ctx.stroke();
  }
}

function collisionDetection(e, bounding, uiState, callback) {
  const x = e.clientX - bounding.left - uiState.originX;
  const y = e.clientY - bounding.top - uiState.originY;

  const nearestNeighbor = uiState.findNode(x, y);
  if (nearestNeighbor !== null) {
    return callback(nearestNeighbor);
  }

  return null;
}

function App() {
  const [uiState, updateUiState] = React.useState(new State());
  const checkedValues = new CheckedValues(uiState);
  const canvas = React.useRef();
  
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
    updateUiState(currentState => {
      let state = Object.assign(Object.create(currentState), currentState);
      state[name] = checkedItems[0];

      return state;
    });
  };

  const canvasClickHandler = e => {
    MarkovPromise.then(() => {
      var ctx = canvas.current.getContext("2d");
      let bounding = canvas.current.getBoundingClientRect();
          
      if (uiState.editMode === "addNode") {
        const overlap = collisionDetection(e, bounding, uiState, () => true);

        if (overlap !== true) {
          updateUiState(currentState => {
            let state = Object.assign(Object.create(currentState), currentState);

            state.addNode(
              e.clientX - bounding.left - uiState.originX, 
              e.clientY - bounding.top - uiState.originY
            );

            state.updateSeparationStatement();
            return state;
          })
            
          ctx.canvas.width = canvas.current.clientWidth;
          ctx.canvas.height = canvas.current.clientHeight;
        }
      } else if (uiState.editMode === 'addEdge') {
        if (firstVertexForEdge.current === null) {
          collisionDetection(e, bounding, uiState, v => setFirstVertexForEdge(v));
        } else {
          collisionDetection(e, bounding, uiState, v => {
            if (v.id !== firstVertexForEdge.current.id) {
              updateUiState(currentState => {
                let state = Object.assign(Object.create(currentState), currentState);
      
                state.addArrow(firstVertexForEdge.current.id, v.id);
                state.updateSeparationStatement();
                
                setFirstVertexForEdge(null);
                
                return state;
              });
            }
          });
        }
      } else if (uiState.editMode === "selectSubsets") {
        collisionDetection(e, bounding, uiState, v => {
          updateUiState(currentState => {
            let state = Object.assign(Object.create(currentState), currentState);
            state.tagNode(
              v.id,
              v.tag === "J" ? "" : "J"
            );
          
            state.updateSeparationStatement();
            return state;
          });
        });
      }
    });
  };

  const panCanvas = e => {
    if (isMousedown.current && (uiState.editMode === "moveCanvas")) {
      updateUiState(currentState => {
        let state = Object.assign(Object.create(currentState), currentState);
        state.originX += e.movementX;
        state.originY += e.movementY;

        return state;
      })
    }
  }

  const canvasSecondaryClick = e => {
    if (uiState.editMode === "selectSubsets") {
      let bounding = canvas.current.getBoundingClientRect();

      collisionDetection(e, bounding, uiState, v => {
        updateUiState(currentState => {
          let state = Object.assign(Object.create(currentState), currentState);

          state.tagNode(
            v.id,
            v.tag === "L" ? "" : "L"
          );

          state.updateSeparationStatement();
          return state;
        });
      });

      e.preventDefault();
    }
    return uiState.editMode !== "selectSubsets"
  };

  const debugRefresh = e => {
    console.log("Used the debug refresh", uiState);
    updateUiState(currentState => {
      let state = Object.assign(Object.create(currentState), currentState);
      state.updateSeparationStatement();

      return state;
    })
  }

  useEffect(() => {
    redraw(canvas.current, uiState);
    console.log("Triggered redraw", uiState);
  }, [uiState]);

  React.useEffect(() => {
    if (uiState.originX === undefined) 
      uiState.originX = canvas.current.clientWidth / 2;
    if (uiState.originY === undefined) 
      uiState.originY = canvas.current.clientHeight / 2;

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
            onClick={debugRefresh}
            icon={<ArrowCounterclockwise24Regular/>}
          />
        </Toolbar>
      </FluentProvider>
      <canvas 
        id="viewport"
        ref={canvas}
        onClick={canvasClickHandler}
        onContextMenu={canvasSecondaryClick}
        onMouseUp={e => setIsMousedown(false)}
        onMouseMove={panCanvas}
        onMouseDown={e => setIsMousedown(true)}></canvas>
    </div>
  );
}

export default App;
