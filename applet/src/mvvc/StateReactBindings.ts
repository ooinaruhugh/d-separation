import {State} from './State';

export default class CheckedValues {
  uiState: State;

  constructor(uiState: State) {
    this.uiState = uiState;
  }

  get editMode() {
    return [this.uiState.editMode];
  }

  set editMode(checked) {
    this.uiState.editMode = checked[0];
  }

  get separationMode() {
    return [this.uiState.separationMode];
  }

  set separationMode(checked) {
    this.uiState.separationMode = checked[0];
  }
}