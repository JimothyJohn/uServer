// IO.js

function IO(props) {

  const [state, setState] = React.useState(0);
  const stateArray = {"0": "LOW", "1": "HIGH"}

  function handleChange(e){
    setState(e.target.value);
    axios.post('/io', {
      output: state,
    })
    .then(function (response) {
      console.log(response);
    })
    .catch(function (error) {
      console.log(error);
    });
  };

  function handleSubmit(e){
    e.preventDefault();
    console.log(framework);
  };

  return (
    <div className="App">
      <div className="row p-3 justify-content-center">
        <div className="col-6 col-md-4 align-self-center text-center">
          <h2>Output state: {stateArray[state]}</h2>
        </div>
      </div>
      <div className="row justify-content-center">
        <div className="col-4 col-md-2 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <select value={state} className="form-control" onChange={handleChange}>
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
