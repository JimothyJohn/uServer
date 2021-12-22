#!$HOME/miniconda3/envs/userver/bin/python

from typing import Optional
import uvicorn
from fastapi import FastAPI
import json

app = FastAPI()


def ReadConfig(filename):

    with open(filename, 'r') as f:
        configJSON = json.load(f)
        f.close()

    return configJSON


endpoints = {
    "/mqtt": {
        "devices": {
            "jetson": ReadConfig("config/mqtt/jetson.json"),
            "cloud": ReadConfig("config/mqtt/cloud.json")
        },
    },
    "/modbus": {
        "devices": {
            "moxa": ReadConfig("config/modbus/moxa.json"),
            "ur": ReadConfig("config/modbus/ur.json")
        },
    },
    "/ws": {
        "devices": ["ur", "mecademic"],
    }
}


def ListConfigs(protocol):
    deviceList = [
        f"{name}, " for name, _ in endpoints["/mqtt"]["devices"].items()
    ]
    return {"payload": deviceList, "code": 0}


@app.get("/")
def read_root():
    return {"payload": endpoints, "code": 0}


@app.get("/mqtt/config/{config}")
def read_mqtt_config(config: str):
    return {"payload": endpoints["/mqtt"]["devices"][config], "code": 0}


@app.get("/modbus/config/{config}")
def read_modbus_devices(config: str):
    return {"payload": endpoints["/modbus"]["devices"][config], "code": 0}


@app.get("/ws/config/{config}")
def read_websocket_devices(config: str):
    return {"payload": endpoints["/ws"]["devices"][config], "code": 0}


@app.get("/ping")
def read_help():
    return {
        "payload":
        "Choose an endpoint: /mqtt/config/{file}, /modbus, /ws, /help",
        "code": 0
    }


if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)
