import eventlet
eventlet.monkey_patch()
import json
from flask import Flask, render_template
from flask_socketio import SocketIO
from flask_mqtt import Mqtt
from flask_bootstrap import Bootstrap
from flask_cors import CORS

app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = "192.168.57.204"
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = ""
app.config['MQTT_PASSWORD'] = ""
app.config['MQTT_REFRESH_TIME'] = 0.1  # refresh time in seconds
#app.config['MQTT_KEEPALIVE'] = 5  # set the time interval for sending a ping to the broker to 5 seconds

cors = CORS(app, resources={r"*": {"origins": "*"}})
mqtt = Mqtt(app)
bootstrap = Bootstrap(app)
socketio = SocketIO(app, cors_allowed_origins='*', async_mode="eventlet")

led_state = 'OFF'

@app.route('/')
def index():
    return render_template('button.html')

#@mqtt.on_connect()
@socketio.on('subscribe')
def handle_subscribe(json_str):
    data = json.loads(json_str)
    topic = data.get('topic')
    mqtt.subscribe(topic)
    print(f"subscribed to {topic}")

@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    data = {
        'topic':message.topic,
        'payload':message.payload.decode()
    }
    print(f"received data on {data['topic']}: {data['payload']}")
    socketio.emit('mqtt_message', data)

@socketio.on('publish')
def handle_publish(json_str):
    data = json.loads(json_str)
    topic = data.get('topic')
    message = data.get('message')
    mqtt.publish(topic, message)
    print(f"Published '{message}' to {topic}")
    
@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    print(level, buf)

if __name__ == '__main__':
    local_ip = "192.168.57.204"
    print(f" * Serving on http://{local_ip}:80/ (Press CTRL+C to quit)")
    socketio.run(app, host='0.0.0.0', port=5000, use_reloader=False, debug=True)
