// IO.js

function IOPoints(points) {

  function RenderOption(idx) {
    return (
      <option value={idx}>{idx}</option>
    )
  }

  const options = [];
  for (var i=0; i<points; i++) {
    options.push(RenderOption(i));
  }

  return options;
}

function IO(props) {

  const [state, setState] = React.useState({
    point: "0",
    state: "0"
  });
  const stateArray = {"0": "LOW", "1": "HIGH"}

  function handleChange(e){
    if (e.target.name==="point") {
      setState({point: e.target.value})
    } else {
      setState({state: e.target.value})
    }
    axios.post('/io', {
      point: state.point,
      state: state.state,
    })
    .then(function (response) {
      console.log(response);
    })
    .catch(function (error) {
      console.log(error);
    });
  };

  return (
    <div className="App">
      <div className="row p-3 justify-content-center">
        <div className="col-6 col-md-4 align-self-center text-center">
          <h2>Output {state.point} state: {stateArray[state.state]}</h2>
        </div>
      </div>
      <div className="row justify-content-center">
        <div className="col-4 col-md-2 align-self-center text-center">
          <form>
            <select
              value={state.point}
              className="form-control"
              onChange={handleChange}
              name="point"
            >
              <option>Choose an output...</option>
              {IOPoints(10)}
            </select>
            <select
              value={state.state}
              className="form-control"
              onChange={handleChange}
              name="state"
            >
              <option>Choose a state...</option>
              <option value="0">Low</option>
              <option value="1">High</option>
            </select>
          </form>
        </div>
      </div>
    </div>
  )
}

ReactDOM.render(
    <IO />,
    document.getElementById('io')
)
