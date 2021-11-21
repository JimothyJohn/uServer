#!/usr/bin/env python3
#!$HOME/miniconda3/envs/userver/bin/python
import requests, json

BASE_URL = 'http://userver.local'

def ReadInput(pin):
    payload = json.dumps({'action': 'read', 'pin': pin})
    print(payload)
    r = requests.post(f'{BASE_URL}/io', data=payload)
    # r = requests.get(f'{BASE_URL}/')
    print(r.text)

ReadInput(0)
