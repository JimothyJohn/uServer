// App.js
const { BrowserRouter, Switch, Route, Link } = ReactRouterDOM;
const {
  Container,
  Row,
  Col,
  Navbar,
  Nav,
  ListGroup
} = ReactBootstrap;
const { useState } = React;

const error_codes = {
  0: 'Success',
  1: 'Failure',
  2: 'No response',
};

function NavBar(props) {

  return (
    <header>
      <Navbar collapseOnSelect bg="dark" variant="dark" expand="lg" className="shadow">
        <Container>
          <Navbar.Brand as={Link} to="/">µServer</Navbar.Brand>
          <Navbar.Toggle aria-controls="muh-toggle" />
          <Navbar.Collapse id="muh-toggle">
            <Nav className="me-auto">
              <Nav.Link as={Link} to="/io">Digital I/O</Nav.Link>
              <Nav.Link as={Link} to="/variables">Variables</Nav.Link>
              <Nav.Link as={Link} to="/mqtt">MQTT</Nav.Link>
              <Nav.Link as={Link} to="/files">File System</Nav.Link>
            </Nav>
          </Navbar.Collapse>
        </Container>
      </Navbar>
    </header>
  );
}

function Footer(props) {

  return (
    <footer>
      <Container fluid="sm" className="fixed-bottom">
        <Row>
          <Col className="text-center">
            <a href="http://advin.io/"><img src="https://drive.google.com/uc?id=19gtVzQTrjAqPSP7G1lUsRx8mjidmgyCA" width="150px" /></a>
            <p>Copyright © 2020 advin.io. All rights reserved.</p>
          </Col>
        </Row>
      </Container>
    </footer>
  )
}

function About(props) {

  return (
    <Container fluid="sm">
      <Row className="p-5 justify-content-center">
        <Col className="description rounded-3 align-self-center text-center shadow">
          <h1 className="p-4 headline"><strong>µServer</strong></h1>
          <p className="lead text-muted pb-3">µServer is a template for rapid networking of MCU's.<br />
          Use the navbar to see what you can do!</p>
        </Col>
      </Row>
    </Container>
  )
}

function IO(props) {

  const [output, setOutput] = useState({
    pin: 0,
    state: 'LOW',
  });

  const [input, setInput] = useState({
    pin: 0,
    state: 'LOW',
  });

  function readInput(event) {
    event.preventDefault();
    axios.post('/io', {
      action: 'read',
      pin: event.target.value,
    })
      .then(function (response) {
        setInput({
          pin: response.data.pin,
          state: response.data.state,
        })
      })
      .catch(function (error) {
        console.log(error);
      });
  }

  function writeOutput(event) {
    event.preventDefault();
    axios.post('/io', {
      action: 'write',
      pin: output.pin,
      state: event.target.value,
    })
      .then(function (response) {
        setOutput(prevState => {
          return {
            ...prevState,
            state: response.data.state,
          };
        });
      })
      .catch(function (error) {
        console.log(error);
      });
  };

  function chooseOutput(event) {
    event.preventDefault();
    setOutput(prevState => {
      // Object.assign would also work
      return {
        ...prevState,
        pin: event.target.value
      };
    });
  }

  return (
    <Container fluid="sm">
      <Row className="p-5 justify-content-center">
        <Col className="description rounded-3 align-self-center text-center shadow">
          <h1 className="p-4 headline"><strong>Digital I/O</strong></h1>
          <p className="lead text-muted pb-3">
            Use buttons to set outputs and text to
            read inputs all via web browser.
            </p>
        </Col>
      </Row>
      <Row className="p-3 justify-content-center">
        <Col className="align-self-center text-center">
          <h2>Input {input.pin} state: {input.state}</h2>
        </Col>
        <Col className="align-self-center text-center">
          <h2>Output {output.pin} state: {output.state}</h2>
        </Col>
      </Row>
      <Row className="justify-content-center">
        <Col className="col-4 align-self-center text-center">
          <form>
            <select
              value={input.pin}
              className="form-control"
              onChange={readInput}
            >
              <option>Choose an input...</option>
              <option value={0}>0</option>
              <option value={1}>1</option>
              <option value={2}>2</option>
              <option value={3}>3</option>
            </select>
          </form>
        </Col>
        <Col className="col-4 align-self-center text-center">
          <form>
            <select
              value={output.pin}
              className="form-control"
              onChange={chooseOutput}>
              <option>Choose an output...</option>
              <option value={0}>0</option>
              <option value={1}>1</option>
              <option value={2}>2</option>
              <option value={3}>3</option>
            </select>
            <select value={output.state} className="form-control" onChange={writeOutput}>
              <option>Choose a state...</option>
              <option value={0}>LO</option>
              <option value={1}>HI</option>
            </select>
          </form>
        </Col>
      </Row>
    </Container>
  )
}

function Variables(props) {

  const [count, setCount] = useState(0);

  function sendCount(event) {
    event.preventDefault();
    setCount(event.target.value);
    axios.post('/variables', {
      postInt: event.target.value,
    })
      .then(function (response) {
        console.log(response);
      })
      .catch(function (error) {
        console.log(error);
      });
  }

  return (
    <Container fluid="sm">
      <Row className="p-5 justify-content-center">
        <Col className="description rounded-3 align-self-center text-center shadow">
          <h1 className="p-4 headline"><strong>Variables</strong></h1>
          <p className="lead text-muted pb-3">
            Use forms and inputs to set variables and
            parameters all via web browser.
          </p>
        </Col>
      </Row>
      <Row className="p-3 justify-content-center">
        <Col className="col-4 align-self-center text-center">
          <h2>Number selected: {count}</h2>
        </Col>
      </Row>
      <Row className="justify-content-center">
        <Col className="col-4 align-self-center text-center">
          <form>
            <select value={count} className="form-control" onChange={sendCount}>
              <option value="0">Choose a number...</option>
              <option value="1">1</option>
              <option value="2">2</option>
            </select>
          </form>
        </Col>
      </Row>
    </Container>
  )
}

function MQTT(props) {

  const [connected, setConnected] = useState(false)

  function Connect() {

    const [hostname, setHost] = useState("10.0.0.28");

    function handleSubmit(event) {
      event.preventDefault();
      var action = 'connect'
      if (connected) { action = 'disconnect' }
      axios.post('/mqtt', {
        action: action,
        host: hostname,
      })
        .then(response => {
          if (response.data.code == 0) { setConnected(!connected); }
          else { setConnected(false); }
        })
        .catch(error => { console.log(error); });
    };

    return (
      <Row className="p-3 justify-content-center">
        <Col className="col-4 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <label for="hostname">Enter hostname:</label>
            <input
              type="text"
              value={hostname}
              onChange={event => setHost(event.target.value)}
              name="hostname"
            />
            <input type="submit" value={connected ? 'Disconnect' : 'Connect'} />
          </form>
          <p>{connected ? 'Connected to ' + hostname : 'Not connected'}</p>
        </Col>
      </Row>
    )
  }

  function Publish() {

    const [message, setMessage] = useState({
      payload: 'Hello world!',
      topic: '/topic',
    });
    const [status, setStatus] = useState("")

    const handleChange = (event) => {
      setMessage({ ...message, [event.target.name]: event.target.value });
    }

    function handleSubmit(event) {
      event.preventDefault();

      if (!connected) {
        setStatus("Not connected to broker!")
        return
      }

      axios.post('/mqtt', {
        action: 'publish',
        payload: message.payload,
        topic: message.topic,
      })
        .then(response => {
          if (response.data.code > 0) { setStatus("Failed to publish!") }
          else { setStatus("Sent \"" + response.data.payload + "\" to " + response.data.topic) }
        })
        .catch(function (error) { console.log(error); });
    };

    return (
      <Row className="p-3 justify-content-center">
        <Col className="col-4 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <label for="payload">Enter message:</label>
            <input
              type="text"
              name="payload"
              value={message.payload}
              onChange={handleChange}
            />
            <label for="topic">Enter topic:</label>
            <input
              type="text"
              name="topic"
              value={message.topic}
              onChange={handleChange}
            />
            <input type="submit" value="Publish" />
            <p>{status}</p>
          </form>
        </Col>
      </Row>
    )
  }

  function Subscribe() {

    const source = new EventSource('/events');
    const [message, setMessage] = useState({
      payload: '',
      topic: '',
    });

    source.addEventListener('subscription', function (event) {
      const response = JSON.parse(event.data);
      setMessage({
        payload: response.payload,
        topic: response.topic,
      })
      setStatus("");
    }, false);

    const [topic, setTopic] = useState('/topic');
    const [subs, setSubs] = useState([])
    const [status, setStatus] = useState("")

    function handleSubmit(event) {
      event.preventDefault();
      if (!connected) {
        setStatus("Not connected to broker!")
        return
      }
      setTopic(event.target.value)
      axios.post('/mqtt', {
        action: 'subscribe',
        topic: topic,
      })
        .then(function (response) {
          if (response.data.code > 0) { setStatus("Failed to subscribe!") }
          else {
            setSubs(subs.concat(topic));
            setStatus("Subscribed to \"" + topic);
          }
        })
        .catch(function (error) {
          console.log(error);
        });
    };

    function renderSub(sub) {

      return (
        <Row>
          <ListGroup.Item active={message.topic == sub}>{sub}</ListGroup.Item>
          <p className="message">{message.topic == sub ? message.payload : ""}</p>
        </Row>
      );
    }

    function ListSubs() {
      const subList = [];
      for (var i = 0; i < subs.length; i++) {
        subList.push(renderSub(subs[i]));
      };
      return (
        <Col className="col-4 align-self-center text-center">
          <h>Active subscriptions:</h>
          <ListGroup>{subList}</ListGroup>
        </Col>
      );
    }

    return (
      <Row className="p-3 justify-content-center">
        <Col className="col-4 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <label for="topic">Subscribe to topic:</label>
            <input
              type="text"
              name="topic"
              value={topic}
              onChange={event => setTopic(event.target.value)}
            />
            <input type="submit" value="Subscribe" />
          </form>
          <p>{status}</p>
          <p>Received {message.payload} from {message.topic}</p>
        </Col>
        <ListSubs />
      </Row>
    )
  }


  return (
    <Container fluid="sm">
      <Row className="p-5 justify-content-center">
        <Col className="description rounded-3 align-self-center text-center shadow">
          <h1 className="p-4 headline"><strong>MQTT</strong></h1>
          <p className="lead text-muted pb-3">Publish messages and subscribe to topics!</p>
        </Col>
      </Row>
      <Connect />
      <Publish />
      <Subscribe />
    </Container>
  )
}

function ReadFiles(props) {

  const [config, setConfig] = useState({
    filename: "",
    contents: {},
  });

  const [directory, setDirectory] = useState({
    files: [],
    dirs: [],
    dir: "/config/mqtt",
  });

  function ChooseDirectory() {

    function handleSubmit(event) {
      event.preventDefault();
      axios.post('/files', { dir: directory.dir, })
      .then(response => {
        setDirectory(prevState => {
          return {
            ...prevState,
            files: response.data
          }
        });
      })
      .catch(error => console.log(error));
    }

    const handleChange = (event) => {
      setDirectory(prevState => {
        return {
          ...prevState,
          dir: event.target.value,
        };
      });
    }

    function ListFiles() {
      if (directory.files.length>0) {
        return ( directory.files.map(file => (
          <li>{file.name.split("/")[3]}</li>
        )));
      } else { return ( <li>Empty</li> ); }
    }

    return (
      <Row className="p-3 justify-content-center">
        <Col className="col-4 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <label for="dir">Enter directory...</label>
            <select
              value={directory.dir}
              className="form-control"
              onChange={handleChange}>
              <option value={"/config/mqtt"}>MQTT</option>
              <option value={"/config/ws"}>WebSocket</option>
              <option value={"/config/Modbus"}>Modbus</option>
            </select>
            <input type="submit" value="Read Directory" />
          </form>
          <ul><ListFiles /></ul>
        </Col>
      </Row >
    );
  }

  function ChooseFile() {

    function handleSubmit(event) {
      event.preventDefault();
      axios.post('/file', { filename: event.target.value, })
        .then(response => {
          setConfig(prevState => {
            return {
              ...prevState,
              contents: response.data,
            }
          });
        })
        .catch(error => console.log(error));
    };

    const handleChange = (event) => {
      setConfig(prevState => {
        return {
          ...prevState,
          filename: event.target.value,
        };
      });
    }

    function FileOptions() {
      if (directory.files.length>0) {
        return ( directory.files.map(file => (
          <option value={file.name}>{file.name.split("/")[3]}</option>
        )));
      } else { return ( <option>Empty</option> ); }
    };

    return (
      <Row className="p-3 justify-content-center">
        <Col className="col-4 align-self-center text-center">
          <form onSubmit={handleSubmit}>
            <label for="file">Choose a file...</label>
            <select
              value={config.filename}
              className="form-control"
              onChange={handleChange}>
              <FileOptions />
            </select>
            <input type="submit" value="Read File" />
          </form>
        </Col>
      </Row >
    );
  }

  return (
    <div className="App">
      <Container fluid="sm">
        <Row className="p-5 justify-content-center">
          <Col className="description rounded-3 align-self-center text-center shadow">
            <h1 className="p-4 headline"><strong>File Browser</strong></h1>
            <p className="lead text-muted pb-3">Browse the file system and read JSON configs!</p>
          </Col>
        </Row>
        <ChooseDirectory />
      </Container>
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
        <Route path="/files"><ReadFiles /></Route>
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
