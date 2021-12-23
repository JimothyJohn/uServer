#!$HOME/miniconda3/envs/userver/bin/python
import requests
import json

USERVER_HOST = 'http://userver.local'
API_HOST = "10.0.0.28"


def ReadInput(pin):
    payload = json.dumps({'action': 'read', 'pin': pin})
    print(f"Reading pin {pin}...", end="")
    r = requests.post(f'{USERVER_HOST}/io', data=payload)
    digitalInput = json.loads(r.text)
    assert digitalInput['pin'] == pin
    assert digitalInput['state'] == ("HIGH" or "LOW")
    print(f"Pin {digitalInput['pin']} is {digitalInput['state']}!", end="\r")
    return True


def SetOutput(pin, state):
    payload = json.dumps({'action': 'write', 'pin': pin, 'state': state})
    print(f"Setting pin {pin} to {state}...", end="")
    r = requests.post(f'{USERVER_HOST}/io', data=payload)
    digitalInput = json.loads(r.text)
    assert digitalInput['pin'] == pin
    assert digitalInput['state'] == state
    print(
        f"Pin {digitalInput['pin']} is now {digitalInput['state']}!", end="\r")
    return True


def SetVariable(number):
    payload = json.dumps({'postInt': number})
    print(f"Setting postInt to {number}...", end="")
    r = requests.post(f'{USERVER_HOST}/variables', data=payload)
    userVariable = json.loads(r.text)
    assert userVariable['code'] == 0
    assert userVariable['postInt'] == number
    print(f"Variable is now {userVariable['postInt']}!", end="\r")
    return True


def ReadDirectory(dirname):
    payload = json.dumps({'dir': dirname})
    print(f"Reading {dirname} directory...", end="")
    r = requests.post(f'{USERVER_HOST}/dir', data=payload)
    response = json.loads(r.text)
    assert len(response) > 0
    print(f"First file in directory: {response[0]['name']}", end="\r")
    return True


def ReadConfig(filename):
    payload = json.dumps({'filename': filename})
    print(f"Reading {filename} config...", end="")
    r = requests.post(f'{USERVER_HOST}/file', data=payload)
    response = json.loads(r.text)
    assert len(response) > 0
    print(f"File contents: {response}", end="\r")
    return True


class TestMQTT():

    def __init__(self, host):
        self.host = host

    def configure(self):
        payload = json.dumps({'action': 'configure', 'host': self.host})
        print(f"Configuring MQTT...", end="")
        r = requests.post(f'{USERVER_HOST}/mqtt', data=payload)
        responses = json.loads(r.text)
        for response in responses:
            print(f"Received {json.dumps(response)}")
        return True

    def connect(self):
        payload = json.dumps({'action': 'connect', 'host': self.host})
        print(f"Connecting to {self.host}...", end="")
        r = requests.post(f'{USERVER_HOST}/mqtt', data=payload)
        response = json.loads(r.text)
        assert response['code'] == 0
        print(f"Confirmation code: {response['code']}", end="\r")
        return True

    def disconnect(self):
        payload = json.dumps({'action': 'disconnect', 'host': self.host})
        print(f'Disconnecting...', end="")
        r = requests.post(f'{USERVER_HOST}/mqtt', data=payload)
        response = json.loads(r.text)
        assert response['code'] == 0
        print(f"Confirmation code: {response['code']}", end="\r")
        return True

    def publish(self, topic, payload):
        payload = json.dumps({
            'action': 'publish',
            'topic': topic,
            'payload': payload
        })
        print(f"Publishing \"{payload}\" to {topic}...", end="")
        r = requests.post(f'{USERVER_HOST}/mqtt', data=payload)
        response = json.loads(r.text)
        assert response['code'] == 0
        print(f"Confirmation code: {response['code']}", end="\r")
        return True

    def subscribe(self, topic):
        payload = json.dumps({'action': 'subscribe', 'topic': topic})
        print(f"Subscribing to {topic}...", end="")
        r = requests.post(f'{USERVER_HOST}/mqtt', data=payload)
        response = json.loads(r.text)
        assert response['code'] == 0
        print(f"Confirmation code: {response['code']}", end="\r")
        return True


def SendREST(hostname, endpoint, query):
    payload = json.dumps({
        'hostname': hostname,
        'endpoint': endpoint,
        'query': query
    })
    print(f"Sending {query} to {hostname}{endpoint}...", end="")
    r = requests.post(f'{USERVER_HOST}/cloud', data=payload)
    response = json.loads(r.text)
    assert response["code"] == 0
    print(f'{json.dumps(response["payload"])}', end="\r")
    return True


mqttTest = TestMQTT(API_HOST)


def RunTests():
    assert ReadInput(0)
    assert SetOutput(0, 1)
    assert SetVariable(32)
    assert ReadDirectory("/config")
    assert ReadConfig("/config/mqtt.json")
    assert mqttTest.configure()
    assert mqttTest.connect()
    assert mqttTest.subscribe("/topic")
    assert mqttTest.publish("/topic", "Hello world!")
    assert mqttTest.disconnect()
    assert SendREST(f"http://{API_HOST}:8000", "/config/mqtt", "?name=edge")
    return True


if RunTests():
    print('Tests passed!')
else:
    print('Tests failed...')
