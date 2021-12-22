# API Overview

Still need CloudAPI...

## I/O
Read status of GPIO0 at http://userver.local/io example.

#### Request

```json
{
    "action": "read", // Use "write" for output
    "pin": 0
}
```

#### Response

```json
{
    "action": "read",
    "pin": 0,
    "state": "HIGH", // alternatively returns "LOW"
    "code": 0
}
```

## Variables 
Change variable value at http://userver.local/variables example.

#### Request

```json
{
    "postInt": 32 // Can be any positive integer
}
```

#### Response

```json
{
    "postInt": 32,
    "code": 0
}
```

## MQTT
Connect to new MQTT broker at http://userver.local/mqtt example:

#### Request

```json
{
    "action": "connect", // Can also "dicsonnect"
    "host": "127.0.0.1" // Address of broker
}
```

#### Response

```json
{
    "code": 0
}
```

## File system
Read files in directory at http://userver.local/files example:

#### Request

```json
{
    "dir": "/", // Choose root directory
}
```

#### Response

```json
[
    {
        "name": "index.html",
        "size": 225
    }
]
```
