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
    <div class="row justify-content-center">
      <div class="col-4 col-md-2 align-self-center text-center">
        <form onSubmit={handleSubmit}>
          <select value={count} className="form-control" onChange={handleChange}>
            <option value="0">Choose a number...</option>
            <option value="1">1</option>
            <option value="2">2</option>
            <option value="3">3</option>
            <option value="4">4</option>
          </select>
        </form>
      </div>
      <p>{count}</p>
    </div>
  )
}

ReactDOM.render(
    <Variables />,
    document.getElementById('variables')
)
