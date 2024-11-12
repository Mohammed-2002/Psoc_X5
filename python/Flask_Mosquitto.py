import eventlet
eventlet.monkey_patch()
import json
import socket
import threading
from flask import Flask, render_template, jsonify
from flask_socketio import SocketIO, emit
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

UDP_IP = "192.168.57.204"
UDP_PORT = 57345
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

#sensor_data = None

sensor_data = {} 

def listen_to_udp():
    global sensor_data
    while True:
        data, addr = sock.recvfrom(1024)  # Buffer size
        message = data.decode('utf-8')
        '''
        socketio.emit('update_sensor_data', {'sensor_data': sensor_data})
        '''

        try:
            parsed_data = json.loads(message)
            sensor_id = parsed_data["sensor_id"]
            distance = parsed_data["distance"]

            sensor_data[sensor_id] = distance

            socketio.emit('update_sensor_data', {'sensor_data': sensor_data})

        except json.JSONDecodeError:
            print("Received malformed JSON:", message)

udp_thread = threading.Thread(target=listen_to_udp)
udp_thread.daemon = True
udp_thread.start()

@app.route('/api/ledData')
def get_ledData():
    return jsonify({'sensor_data': sensor_data})

@app.route('/api/sensorData')
def get_sensorData():
    return jsonify({'sensor_data': sensor_data})

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
