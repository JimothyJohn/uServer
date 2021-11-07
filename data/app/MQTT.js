// MQTT.js

function MQTT(props) {

  // const [payload, setPayload] = React.useState('Enter message...');
  // const [topic, setTopic] = React.useState('/topic');
  const [message, setMessage] = React.useState({
    payload: 'Enter message...',
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

ReactDOM.render(
    <MQTT />,
    document.getElementById('mqtt')
)
