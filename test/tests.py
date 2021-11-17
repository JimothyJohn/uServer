#!/usr/bin/env python3
import requests, json

BASE_URL = 'http://userver.local'

def ReadInput(pin):
    payload = {'action': 'read', 'pin': pin}
    r = requests.post(f'{BASE_URL}/io', data=payload)
    print(r.text)
    return

ReadInput(0)
