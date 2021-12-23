#!$HOME/miniconda3/envs/userver/bin/python

from typing import Optional
import uvicorn
from fastapi import FastAPI
import json
# from pydantic import BaseModel

app = FastAPI()


def ReadConfig(filename):

    with open(filename, 'r') as f:
        configJSON = json.load(f)
        f.close()

    return configJSON


protocols = {
    "mqtt": ReadConfig("config/mqtt.json"),
    "modbus": ReadConfig("config/modbus.json"),
    "ws": ReadConfig("config/ws.json"),
}


@app.get("/")
def read_root():
    return {"payload": protocols, "code": 0}


@app.get("/config/{protocol}")
def read_config(protocol: str, name: Optional[str] = None):
    if name is None:
        # Send list of possible config files for specific protocol
        return {
            "payload": [device for device, _ in protocols[protocol].items()],
            "code": 0,
        }
    else:
        # Send contents of configuration file
        return {"payload": protocols[protocol][name], "code": 0}


@ app.get("/ping")
def read_help():
    return {
        "payload":
        "Choose an endpoint: /mqtt/config/{file}, /modbus, /ws, /help",
        "code": 0
    }


if __name__ == "__main__":
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=True)
