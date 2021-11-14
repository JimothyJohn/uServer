// Template.js
const { BrowserRouter, Switch, Route, Link } = ReactRouterDOM;

function NavBar(props) {
  // Classes for each module
  const navClass = "navbar navbar-dark bg-dark shadow px-5";
  const linkClass = "navbar-brand px-3";

  return (
    <header>
      <div className={navClass}>
        <Link to="/" className={linkClass}>About</Link>
        <Link to="/io" className={linkClass}>Digital I/O</Link>
        <Link to="/variables" className={linkClass}>Variables</Link>
        <Link to="/mqtt" className={linkClass}>MQTT</Link>
      </div>
    </header>
  );
}

function Footer(props) {
    return(
      <footer>
        <div className="fixed-bottom container-fluid">
            <div className="row">
                <div className="col text-center">
                <a href="http://advin.io/"><img src="https://drive.google.com/uc?id=19gtVzQTrjAqPSP7G1lUsRx8mjidmgyCA" width="150px" /></a>
                <p>Copyright © 2020 advin.io. All rights reserved.</p>
                </div>
            </div>
        </div>
      </footer>
    )
}

function About(props) {
  return (
    <main class="container-fluid">
        <div class="row p-5 justify-content-center">
            <div class="description col col-md-6 bg-light rounded-3 align-self-center text-center shadow">
                <h1 class="p-4 headline"><strong>µServer</strong></h1>
                <p class="lead text-muted pb-3">µServer is a template for rapid networking of MCU's.<br />
                Use the navbar to see what you can do!</p>
            </div>
        </div>
    </main>
  )
}

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
    <div className="container-fluid">
      <div class="row p-5 justify-content-center">
      <div class="description col col-md-6 bg-light rounded-3 align-self-center text-center shadow">
          <h1 class="p-4 headline"><strong>Digital I/O</strong></h1>
          <p class="lead text-muted pb-3">
            Use buttons to set outputs and text to
            read inputs all via web browser.
          </p>
      </div>
      </div>
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
    <div class="container-fluid">
      <div class="row p-5 justify-content-center">
        <div class="description col col-md-6 bg-light rounded-3 align-self-center text-center shadow">
          <h1 class="p-4 headline"><strong>Variables</strong></h1>
          <p class="lead text-muted pb-3">
            Use forms and inputs to set variables and 
            parameters all via web browser.
          </p>
        </div>
      </div>
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

function Publish(props) {

  const [message, setMessage] = React.useState({
    payload: 'Hello world!',
    topic: '/topic',
  });

  function handleSubmit(event){
    event.preventDefault();
    axios.post('/mqtt/pub', {
      payload: message.payload,
      topic: message.topic,
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
      <div className="row justify-content-center">
        <div className="col-4 col-md-2 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <label for="payload">Enter message:</label>
            <input
              type="text"
              value={message.payload}
              onChange={e => setMessage({payload: e.target.value})}
              name="payload"
            />
            <label for="topic">Enter topic:</label>
            <input
              type="text"
              value={message.topic}
              onChange={e => setMessage({topic: e.target.value})}
              name="topic"
            />
            <input type="submit" value="Publish" />
          </form>
        </div>
      </div>
    </div>
  )
}

function Connect(props) {

  const [hostname, setHost] = React.useState("10.0.0.28");

  function handleSubmit(event){
    event.preventDefault();
    axios.post('/mqtt/connect', {
      host: hostname,
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
      <div className="row justify-content-center">
        <div className="col-4 col-md-2 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <label for="hostname">Enter hostname:</label>
            <input
              type="text"
              value={hostname}
              onChange={e => setHost(e.target.value)}
              name="hostname"
            />
            <input type="submit" value="Connect" />
          </form>
        </div>
      </div>
    </div>
  )
}

function MQTT(props) {
  return (
    <div class="container-fluid">
      <div class="row p-5 justify-content-center">
        <div class="description col col-md-6 bg-light rounded-3 align-self-center text-center shadow">
          <h1 class="p-4 headline"><strong>MQTT</strong></h1>
          <p class="lead text-muted pb-3">Publish messages and subscribe to topics!</p>
        </div>
      </div>
      <Publish />
      <Connect />
    </div>
  )
}

function App(props) {
  return (
    <div className="App">
      <NavBar />
      <Switch>
        <Route exact path="/"><About /></Route>
        <Route path="/io"><IO /></Route>
        <Route path="/variables"><Variables /></Route>
        <Route path="/mqtt"><MQTT /></Route>
      </Switch>
      <Footer />
    </div>
  )
}

ReactDOM.render(
  <BrowserRouter>
    <App />
  </BrowserRouter>,
  document.getElementById('root')
)
