// Variables.js

function Variables(props) {

  const [count, setCount] = React.useState(0);

  function sendCount(value) {
    axios.post('/variables', {
      postInt: value,
    })
    .then(function (response) {
      console.log(response);
    })
    .catch(function (error) {
      console.log(error);
    });
  }

  function handleChange(e){
    setCount(e.target.value);
    sendCount(e.target.value);
 };

  function handleSubmit(e){
    e.preventDefault();
    console.log(framework);
  };

  return (
    <div className="App">
      <div className="row p-3 justify-content-center">
        <div className="col-6 col-md-4 align-self-center text-center">
          <h2>Number selected: {count}</h2>
        </div>
      </div>
      <div className="row justify-content-center">
        <div className="col-4 col-md-2 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <select value={count} className="form-control" onChange={handleChange}>
              <option value="0">Choose a number...</option>
              <option value="1">1</option>
              <option value="2">2</option>
            </select>
          </form>
        </div>
      </div>
    </div>
  )
}

ReactDOM.render(
    <Variables />,
    document.getElementById('variables')
)
