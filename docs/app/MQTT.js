// MQTT.js

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
    <div className="App">
      <Publish />
      <Connect />
    </div>
  )
}

ReactDOM.render(
    <MQTT />,
    document.getElementById('mqtt')
)
