# MCU_Server (Dashboard-Only MQTT Web Client)

This service is a lightweight Django app that serves a realtime dashboard.
The browser connects directly to the MQTT broker over WebSockets and updates the UI instantly.

## What This Server Does

- Serves the dashboard page at `/`
- Provides broker/topic config to frontend JavaScript
- Hosts static assets (CSS/JS)

## Data Flow

1. ESP32 Camera or MQTTX publishes payload to MQTTx Web Client
2. Browser dashboard subscribes to that topic over WebSockets
3. Dashboard classifies payload as BIO or N-BIO and updates animation

## Prerequisites

- Python 3.10+
- pip
- Internet access to MQTTx Web Client broker (default `broker.emqx.io`)

## Install and Run

From `MCU_Server` directory:

```powershell
py -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r requirements.txt
python manage.py runserver
```

Open:

- http://127.0.0.1:8000/

## MQTT/Broker Configuration

Defaults are in `config/settings.py`:

- `MQTT_BROKER_HOST = broker.emqx.io`
- `MQTT_BROKER_PORT = 1883` (ESP32 TCP MQTT)
- `MQTT_WS_PORT = 8084` (browser WebSocket)
- `MQTT_WS_PATH = /mqtt`
- `MQTT_TOPIC = waste-management-system/command`
- `MQTT_DATA_TOPIC = waste-management-system/data`

Optional override (PowerShell example before running server):

```powershell
$env:MQTT_BROKER_HOST = "broker.emqx.io"
$env:MQTT_WS_PORT = "8084"
$env:MQTT_WS_PATH = "/mqtt"
$env:MQTT_TOPIC = "waste-management-system/command"
python manage.py runserver
```

## MQTTX Web Client Setup

In MQTTX Web Client:

1. Create connection using WebSocket/WSS
2. Host: `broker.emqx.io`
3. Port: `8084`
4. Path: `/mqtt`
5. TLS/SSL: enabled
6. Connect

Publish to topic:

- `waste-management-system/command`

## Payload Format Accepted by Dashboard

The dashboard accepts both plain text and JSON payloads.

### Plain Text Payload

Supported values:

- BIO aliases: `BIO`, `B`, `BIODEGRADABLE`
- Non-bio aliases: `N-BIO`, `NON-BIO`, `NON_BIO`, `NB`, `NONBIO`, `NONBIODEGRADABLE`

Examples:

```text
BIO
```

```text
N-BIO
```

### JSON Payload

Supported keys (first one found is used):

- `label`
- `class`
- `type`
- `category`
- `prediction`
- `cmd`

Value must still be one of the aliases above.

Examples:

```json
{ "label": "BIO" }
```

```json
{ "cmd": "N-BIO" }
```

```json
{ "type": "B" }
```

## Which Data Format Is Received From MCU_Server?

MCU_Server itself does not receive waste-classification payloads.
Payloads are received directly by the browser from MQTT.

MCU_Server sends frontend config as JSON (embedded in page) with this shape:

```json
{
  "host": "broker.emqx.io",
  "tcpPort": 1883,
  "cameraTcpPort": 1883,
  "servoTcpPort": 1883,
  "wsPort": 8084,
  "wsPath": "/mqtt",
  "wsUseSSL": true,
  "commandTopic": "waste-management-system/command",
  "dataTopic": "waste-management-system/data"
}
```

## Troubleshooting

- If dashboard does not update:

1. Check browser connection pill shows connected
2. Verify MQTTX topic matches `MQTT_TOPIC`
3. Verify payload value is one of supported aliases
4. Hard refresh with Ctrl+F5 to clear stale frontend cache

- If stale tabs previously requested removed APIs and spammed logs:

1. Close old tabs
2. Reopen dashboard page
