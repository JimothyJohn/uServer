#!$HOME/miniconda3/envs/userver/bin/python
import requests, json

BASE_URL = 'http://userver.local'


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
    print(f"First file in directory: {response[0]['name']}")
    return True


def ReadFile(filename):
    payload = json.dumps({'filename': filename})
    print(f"Reading {filename} config...", end="")
    r = requests.post(f'{BASE_URL}/file', data=payload)
    response = json.loads(r.text)
    print(f"File contents: {response}")
    return True


def RunTests():
    assert ReadInput(0)
    assert SetOutput(0, 1)
    assert SetVariable(32)
    assert ReadDirectory("/config/mqtt")
    assert ReadFile("/config/mqtt/jetson.json")
    return True


if RunTests(): print('Tests passed!')
else: print('Tests failed...')
