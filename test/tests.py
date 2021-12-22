#!$HOME/miniconda3/envs/userver/bin/python
import requests
import json

BASE_URL = 'http://userver.local'
TEST_HOST = "10.0.0.28"


def ReadInput(pin):
    payload = json.dumps({'action': 'read', 'pin': pin})
    print(f"Reading pin {pin}...", end="")
    r = requests.post(f'{BASE_URL}/io', data=payload)
    digitalInput = json.loads(r.text)
    print(f"Pin {digitalInput['pin']} is {digitalInput['state']}!")
    assert digitalInput['pin'] == pin
    assert digitalInput['state'] == ("HIGH" or "LOW")
    return True


def SetOutput(pin, state):
    payload = json.dumps({'action': 'write', 'pin': pin, 'state': state})
    print(f"Setting pin {pin} to {state}...", end="")
    r = requests.post(f'{BASE_URL}/io', data=payload)
    digitalInput = json.loads(r.text)
    print(f"Pin {digitalInput['pin']} is now {digitalInput['state']}!")
    assert digitalInput['pin'] == pin
    assert digitalInput['state'] == state
    return True


def SetVariable(number):
    payload = json.dumps({'postInt': number})
    print(f"Setting postInt to {number}...", end="")
    r = requests.post(f'{BASE_URL}/variables', data=payload)
    userVariable = json.loads(r.text)
    print(f"Variable is now {userVariable['postInt']}!")
    assert userVariable['code'] == 0
    assert userVariable['postInt'] == number
    return True


def ReadDirectory(dirname):
    payload = json.dumps({'dir': dirname})
    print(f"Reading {dirname} directory...", end="")
    r = requests.post(f'{BASE_URL}/files', data=payload)
    response = json.loads(r.text)
    assert len(response) > 0
    print(f"First file in directory: {response[0]['name']}")
    return True


def ReadFile(filename):
    payload = json.dumps({'filename': filename})
    print(f"Reading {filename} config...", end="")
    r = requests.post(f'{BASE_URL}/file', data=payload)
    response = json.loads(r.text)
    print(f"File contents: {response}")
    return True


def ConnectMQTT(action, hostname):
    payload = json.dumps({'action': action, 'host': hostname})
    if action == 'connect':
        print(f"Connecting to {hostname}...", end="")
    else:
        print(f'Disconnecting...', end="")
    r = requests.post(f'{BASE_URL}/mqtt', data=payload)
    response = json.loads(r.text)
    print(f"Confirmation code: {response['code']}")
    assert response['code'] == 0
    return True


def PublishMQTT(topic, payload):
    payload = json.dumps({
        'action': 'publish',
        'topic': topic,
        'payload': payload
    })
    print(f"Publishing \"{payload}\" to {topic}...", end="")
    r = requests.post(f'{BASE_URL}/mqtt', data=payload)
    response = json.loads(r.text)
    print(f"Confirmation code: {response['code']}")
    assert response['code'] == 0
    return True


def SubscribeMQTT(topic):
    payload = json.dumps({'action': 'subscribe', 'topic': topic})
    print(f"Subscribing to {topic}...", end="")
    r = requests.post(f'{BASE_URL}/mqtt', data=payload)
    response = json.loads(r.text)
    print(f"Confirmation code: {response['code']}")
    assert response['code'] == 0
    return True


def SendREST(hostname, endpoint, query):
    payload = json.dumps({
        'hostname': hostname,
        'endpoint': endpoint,
        'query': query
    })
    print(f"Sending {query} to {hostname}{endpoint}...", end="")
    r = requests.post(f'{BASE_URL}/cloud', data=payload)
    response = json.loads(r.text)
    print(f"Confirmation code: {response['code']}")
    assert response['code'] == 0
    return True


def RunTests():
    assert ReadInput(0)
    assert SetOutput(0, 1)
    assert SetVariable(32)
    assert ReadDirectory("/config/mqtt")
    assert ReadFile("/config/mqtt/jetson.json")
    assert ConnectMQTT("connect", TEST_HOST)
    assert PublishMQTT("/topic", "Hello world!")
    assert SubscribeMQTT("/topic")
    assert ConnectMQTT("disconnect", TEST_HOST)
    assert SendREST(f"http://{TEST_HOST}:8000", "/mqtt/config", "jetson")
    return True


if RunTests():
    print('Tests passed!')
else:
    print('Tests failed...')
