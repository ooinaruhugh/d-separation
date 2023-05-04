import './App.css';

import loadMarkovModule from './wasm/markov.mjs';

import State from './mvvc/State.ts';
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
  Cursor24Regular
} from '@fluentui/react-icons';

const MarkovPromise = loadMarkovModule({
	noInitialRun: true,
	noExitRuntime: true
});

function redraw(canvas, ctx, state) {
  ctx.clearRect(0,0, canvas.width, canvas.height)

  // for (let node in state.nodes) {
  state.nodes.forEach((node) => {
    if (node.tag === "") {
      ctx.strokeStyle = "black"
    } else if (node.tag === "J") {
      ctx.strokeStyle = "red"
    }

    ctx.strokeRect(
      node.x + state.originX, 
      node.y + state.originY, 
      50, 50
    );
  });
  // }
}

function App() {
  const [uiState, ] = React.useState(new State());
  const checkedValues =  new CheckedValues(uiState);
  
  console.log(uiState);

  const [_isMousedown, _setIsMousedown] = React.useState(false);
  const isMousedown = React.useRef(_isMousedown);
  const setIsMousedown = (data) => {
    isMousedown.current = data;
    _setIsMousedown(data);
  }

  const onChange = (
    e,
    {name, checkedItems}
  ) => {
    uiState[name] = checkedItems[0];
  };

  React.useEffect(() => {
    var c = document.getElementById("viewport");
    var ctx = c.getContext("2d");

    uiState.originX = c.clientWidth / 2;
    uiState.originY = c.clientHeight / 2;

    c.addEventListener("contextmenu", (e) => {
      if (uiState.editMode === "selectSubsets") e.preventDefault()
      return uiState.editMode !== "selectSubsets"
    });

    c.addEventListener("click", (e) => {

    })
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []); // Leave empty so it only gets called once...

  MarkovPromise.then((Markov) => {
    const D = new Markov.Digraph(0);

    uiState.theGraph = D;

    uiState.addNode(3);
    uiState.addNode(5);

    let V = uiState.nodes
    console.log(V);

    console.log(Markov);
  });

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
              aria-label="Move canvas"
              name="separationMode"
              value="d"
              appearance='subtle'
              icon="d"
            />
            <ToolbarRadioButton
              aria-label="Add node"
              name="separationMode"
              value="star"
              appearance='subtle'
              icon="&#x2731;"
            />
          </ToolbarRadioGroup>
        </Toolbar>
      </FluentProvider>
      <canvas 
        id="viewport"></canvas>
    </div>
  );
}

export default App;
