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
app.config['MQTT_BROKER_URL'] = "192.168.132.204"
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = ""
app.config['MQTT_PASSWORD'] = ""
app.config['MQTT_REFRESH_TIME'] = 0.1  # refresh time in seconds
#app.config['MQTT_KEEPALIVE'] = 5  # set the time interval for sending a ping to the broker to 5 seconds

cors = CORS(app, resources={r"*": {"origins": "*"}})
mqtt = Mqtt(app)
bootstrap = Bootstrap(app)
socketio = SocketIO(app, cors_allowed_origins='*', async_mode="eventlet")

UDP_IP = "192.168.132.204"
UDP_PORT = 57345
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

#sensor_data = None

sensor_data = {}
accelero_data = {}

def listen_to_udp():
    global sensor_data, accelero_data
    while True:
        data, addr = sock.recvfrom(1024)  # Buffer size
        message = data.decode('utf-8')

        try:
            parsed_data = json.loads(message)
            '''
            sensor_id = parsed_data["sensor_id"]
            distance = parsed_data["distance"]
            sensor_data[sensor_id] = distance
            
            accel_x_g = parsed_data["accel_x_g"]
            accel_y_g = parsed_data["accel_y_g"]
            accel_z_g = parsed_data["accel_z_g"]
            
            sensor_data[0] = accel_x_g
            sensor_data[1] = accel_y_g
            sensor_data[2] = accel_z_g

            socketio.emit('update_sensor_data', {'sensor_data': sensor_data})
            '''
            #'''
            if "accel_x_g" in parsed_data and "accel_y_g" in parsed_data and "accel_z_g" in parsed_data:
                accelero_data['x'] = parsed_data["accel_x_g"]
                accelero_data['y'] = parsed_data["accel_y_g"]
                accelero_data['z'] = parsed_data["accel_z_g"]
                socketio.emit('update_accelero_data', {'accelero_data': accelero_data})

            elif "distance0" in parsed_data and "distance1" in parsed_data and "distance2" in parsed_data and "distance3" in parsed_data:
                sensor_data['0'] = parsed_data["distance0"]
                sensor_data['1'] = parsed_data["distance1"]
                sensor_data['2'] = parsed_data["distance2"]
                sensor_data['3'] = parsed_data["distance3"]
                socketio.emit('update_sensor_data', {'sensor_data': sensor_data})
            #'''
        except json.JSONDecodeError:
            print("Received malformed JSON:", message)

udp_thread = threading.Thread(target=listen_to_udp)
udp_thread.daemon = True
udp_thread.start()

@app.route('/api/acceleroData')
def get_acceleroData():
    return jsonify({'accelero_data': accelero_data})

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
    local_ip = "192.168.132.204"
    print(f" * Serving on http://{local_ip}:80/ (Press CTRL+C to quit)")
    socketio.run(app, host='0.0.0.0', port=5000, use_reloader=False, debug=True)
