import eventlet
eventlet.monkey_patch()
import json
import socket
import os
import ssl
import threading
import pygame
from flask import Flask, render_template, jsonify
from flask_socketio import SocketIO, emit
from flask_mqtt import Mqtt
from flask_bootstrap import Bootstrap
from flask_cors import CORS
from collections import deque
'''
app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = "192.168.137.168"
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = ""
app.config['MQTT_PASSWORD'] = ""
app.config['MQTT_REFRESH_TIME'] = 0.1  # refresh time in seconds
print(os.access('C:/Users/lasse/Desktop/Iot/IoT_Project_Group1/python/ca.crt', os.R_OK)) 
'''


app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = "94e72a782d7b4b9c8ddc2224804a18ce.s1.eu.hivemq.cloud"
app.config['MQTT_BROKER_PORT'] = 8883
app.config['MQTT_USERNAME'] = "python_server"
app.config['MQTT_PASSWORD'] = "Johannes123"
app.config['MQTT_REFRESH_TIME'] = 0.05  # refresh time in seconds
#app.config['MQTT_KEEPALIVE'] = 5  # set the time interval for sending a ping to the broker to 5 seconds

app.config['MQTT_TLS_ENABLED'] = True
app.config['MQTT_TLS_VERSION'] = ssl.PROTOCOL_TLS
app.config['MQTT_TLS_CA_CERTS'] = None
#'''
cors = CORS(app, resources={r"*": {"origins": "*"}})
mqtt = Mqtt(app)
bootstrap = Bootstrap(app)
socketio = SocketIO(app, cors_allowed_origins='*', async_mode="eventlet")

UDP_IP = "192.168.132.204"
UDP_PORT = 57345
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

sensor_data = {}
speed_data = {}
accelero_data = {}

current_state = 0

joystick_state = 0

current_state = "IDLE"  # Start in 'IDLE' state
input_locked = False  # Lock state to prevent multiple state changes
locked_button_index = -1  # Track which button caused the lock
locked_axis_index = -1  # Track which axis caused the lock
axis_locked = None  # Track if axes 4 or 5 are locked
button_2_locked = False

autonomous_status = False

power_mode_status = False
power_mode_topic = 'lowpowerstate'
power_mode_message = 'LOW POWER OFF'
new_power_mode_message = 0
check = False

# Buffer voor de laatste 10 sensorwaarden
buffer_size = 5
sensor_buffers = {
    '3': deque(maxlen=buffer_size),  # front sensor
    '0': deque(maxlen=buffer_size),  # back sensor
    '1': deque(maxlen=buffer_size),  # left sensor
    '2': deque(maxlen=buffer_size)   # right sensor
}

joystick_checker = 0


def listen_to_udp():
    global sensor_data, speed_data, accelero_data, current_state, joystick_checker

    while True:
        data, addr = sock.recvfrom(1024)  # Buffer size
        message = data.decode('utf-8')
        '''
        initialize_joystick() 
        print(joystick_checker)
        if joystick_checker != 0:
            handle_joystick()
            #print(joystick_checker)
        '''
        try:
            parsed_data = json.loads(message)

            if "accel_x_g" in parsed_data and "accel_y_g" in parsed_data and "accel_z_g" in parsed_data:
                accelero_data['x'] = parsed_data["accel_x_g"]
                accelero_data['y'] = parsed_data["accel_y_g"]
                accelero_data['z'] = parsed_data["accel_z_g"]

                socketio.emit('update_accelero_data', {'accelero_data': accelero_data})

            elif "distance0" in parsed_data and "distance1" in parsed_data and "distance2" in parsed_data and "distance3" in parsed_data:
                # Retrieve sensor data from parsed_data
                sensor_data = {
                    '0': parsed_data["distance0"],
                    '1': parsed_data["distance1"],
                    '2': parsed_data["distance2"],
                    '3': parsed_data["distance3"]
                }

                valid_sensor_data = {key: value for key, value in sensor_data.items() if value >= 0}
                selfdriving_mode(valid_sensor_data)

                socketio.emit('update_sensor_data', {'sensor_data': valid_sensor_data})


            elif "speed0" in parsed_data and "speed1" in parsed_data and "speed2" in parsed_data and "speed3" in parsed_data:
                speed_data['0'] = parsed_data["speed0"]
                speed_data['1'] = parsed_data["speed1"]
                speed_data['2'] = parsed_data["speed2"]
                speed_data['3'] = parsed_data["speed3"]
                socketio.emit('update_speed_data', {'speed_data': speed_data})
            #'''
        except json.JSONDecodeError:
            print("Received malformed JSON:", message)

def initialize_joystick():
    global joystick_checker, joystick
    """
    Checks if a joystick is connected. If not, it waits until one is connected
    and initializes it. Returns the initialized joystick object.
    """
    if pygame.init():
        pygame.joystick.init()
        joystick_count = pygame.joystick.get_count()
        if joystick_count > 0:
            # Itereer door de beschikbare joysticks en neem de eerste
            joystick_checker = 1
            for i in range(joystick_count):
                joystick = pygame.joystick.Joystick(i)
                joystick.init()
        else:
            joystick_checker = 0
            pygame.quit()

# Functie om sensorwaarden toe te voegen aan de buffer
def add_to_buffer(sensor_id, value):
    sensor_buffers[sensor_id].append(value)


# Functie om te controleren of er een kort obstakel is op basis van de buffer
def is_obstacle(sensor_id, threshold=50):
    # Als een van de laatste 'n' waarden kleiner dan 50 is, beschouwen we het als een obstakel
    return any(val < threshold for val in sensor_buffers[sensor_id])

def selfdriving_mode(valid_sensor_data):
    global autonomous_status,current_state

    if autonomous_status:
        # Voeg nieuwe sensorwaarden toe aan de buffers
        add_to_buffer('3', valid_sensor_data.get('3', 1000))  # front sensor
        add_to_buffer('0', valid_sensor_data.get('0', 1000))  # back sensor
        add_to_buffer('1', valid_sensor_data.get('1', 1000))  # left sensor
        add_to_buffer('2', valid_sensor_data.get('2', 1000))  # right sensor

        # Controleer welke richtingen vrij zijn op basis van de buffers
        front_clear = not is_obstacle('3')
        back_clear = not is_obstacle('0')
        left_clear = not is_obstacle('1')
        right_clear = not is_obstacle('2')

        # Beslis welke richting te kiezen
        if front_clear:
            next_state = "GO FORWARD"
        elif right_clear:
            next_state = "BACK WHEELS OFF RIGHT"
        elif left_clear:
            next_state = "BACK WHEELS OFF LEFT"
        elif back_clear:
            next_state = "GO BACKWARD"
        else:
            next_state = "TURN OFF"

        # Publiceer het gekozen motorstate
        if next_state != current_state:
            handle_publish(json.dumps({'topic': 'motorstate', 'message': next_state}))
            current_state = next_state

def handle_joystick():
    global current_state, input_locked, locked_axis_index, locked_button_index, axis_locked
    global autonomous_status, power_mode_status, check, button_2_locked
    global joystick_checker

    pygame.event.pump()  # Process all joystick events

    # Read buttons
    buttons = [joystick.get_button(i) for i in range(joystick.get_numbuttons())]
    filtered_buttons = [b for i, b in enumerate(buttons) if i != 2]

    # Read axes
    axes = [joystick.get_axis(i) for i in range(joystick.get_numaxes())]

    # Handle button 2 independently
    if buttons[2] == 1 and not button_2_locked:
        power_mode_status = not power_mode_status
        power_mode_message = 'LOW POWER ON' if power_mode_status else 'LOW POWER OFF'
        button_2_locked = True  # Lock button 2
        handle_publish(json.dumps({'topic': power_mode_topic, 'message': power_mode_message}))
        return  # Exit the function early to prevent state changes

    if buttons[2] == 0 and button_2_locked:
        button_2_locked = False
        return  # Exit after unlocking button 2 to avoid unintended state changes

    # Main input handling logic
    new_state = current_state  # Default: keep the current state

    if not input_locked and joystick_checker == 1:
        # Handle Axes 4 and 5 (Backward/Forward) as exceptions
        if axes[4] >= 0.500:  # Backward
            if axis_locked != 5:  # Ensure axis 5 isn't locked
                if buttons[9] == 1:
                    new_state = "GO BACKWARDLEFT"
                    input_locked = True
                    locked_button_index = 9
                elif buttons[10] == 1:
                    new_state = "GO BACKWARDRIGHT"
                    input_locked = True
                    locked_button_index = 10
                else:
                    new_state = "GO BACKWARD"  # Default Backward motion
                axis_locked = 4  # Lock Axis 4
        elif axes[5] >= 0.5:  # Forward
            if axis_locked != 4:  # Ensure axis 4 isn't locked
                if buttons[9] == 1:
                    new_state = "GO FORWARDLEFT"
                    input_locked = True
                    locked_button_index = 9
                elif buttons[10] == 1:
                    new_state = "GO FORWARDRIGHT"
                    input_locked = True
                    locked_button_index = 10
                else:
                    new_state = "GO FORWARD"  # Default Forward motion
                axis_locked = 5  # Lock Axis 5
        elif buttons[3] == 1:
            new_state = "TURN OFF"
            autonomous_status = not autonomous_status
            input_locked = True
            locked_button_index = 3
        elif buttons[9] == 1:
            new_state = "GO LEFT"
            input_locked = True
            locked_button_index = 9
        elif buttons[10] == 1:
            new_state = "GO RIGHT"
            input_locked = True
            locked_button_index = 10
        elif buttons[15] == 1:
            new_state = "TURN OFF"
            input_locked = True
            locked_button_index = 15
        elif buttons[4] == 1:
            new_state = "GO COUNTERCLOCKWISE"
            input_locked = True
            locked_button_index = 4
        elif buttons[6] == 1:
            new_state = "GO CLOCKWISE"
            input_locked = True
            locked_button_index = 6
        elif -0.600 < axes[1] < 0.600 and axes[0] >= 0.600:
            new_state = "BACK WHEELS OFF RIGHT"
            input_locked = True
            locked_axis_index = 0
        elif -0.200 < axes[1] < 0.200 and axes[0] <= -0.950:
            new_state = "BACK WHEELS OFF LEFT"
            input_locked = True
            locked_axis_index = 0
        elif -0.600 < axes[3] < 0.600 and axes[2] >= 0.600:
            new_state = "FRONT WHEELS OFF RIGHT"
            input_locked = True
            locked_axis_index = 2
        elif -0.600 < axes[3] < 0.600 and axes[2] <= -0.600:
            new_state = "FRONT WHEELS OFF LEFT"
            input_locked = True
            locked_axis_index = 2
        elif -0.600 < axes[2] < 0.600 and axes[3] <= -0.600:
            new_state = "RIGHT WHEELS OFF FORWARD"
            input_locked = True
            locked_axis_index = 3
        elif -0.600 < axes[2] < 0.600 and axes[3] >= 0.600:
            new_state = "RIGHT WHEELS OFF BACKWARD"
            input_locked = True
            locked_axis_index = 3
        elif -0.600 < axes[0] < 0.600 and axes[1] <= -0.600:
            new_state = "LEFT WHEELS OFF FORWARD"
            input_locked = True
            locked_axis_index = 1
        elif -0.600 < axes[0] < 0.600 and axes[1] >= 0.600:
            new_state = "LEFT WHEELS OFF BACKWARD"
            input_locked = True
            locked_axis_index = 1
        else:
            # Check if the locked input is released
            if locked_button_index != -1 and buttons[locked_button_index] == 0:
                input_locked = False
                locked_button_index = -1
            elif locked_axis_index != -1 and abs(axes[locked_axis_index]) < 0.500:
                input_locked = False
                locked_axis_index = -1

    # Reset axis locks when no active axis
    if axes[4] < 0.500 and axes[5] < 0.500:
        axis_locked = None  # Reset axis lock when neither axis is active

    # If nothing is being used and axes 4, 5 are inactive, set state to "TURN OFF"
    if axis_locked is None and not any(buttons) and abs(axes[0]) < 0.5 and abs(axes[1]) < 0.5 and abs(axes[2]) < 0.5 and abs(axes[3]) < 0.5:
        axis_locked = None
        input_locked = False
        if not autonomous_status:
            new_state = "TURN OFF"

    # Print the state if it changes
    if new_state != current_state:
        current_state = new_state
        handle_publish(json.dumps({'topic': 'motorstate', 'message': current_state}))

@app.route('/api/acceleroData')
def get_acceleroData():
    return jsonify({'accelero_data': accelero_data})

@app.route('/api/sensorData')
def get_sensorData():
    return jsonify({'sensor_data': sensor_data})

@app.route('/api/speedData')
def get_speedData():
    return jsonify({'speed_data': speed_data})

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
    global current_state
    data = json.loads(json_str)
    topic = data.get('topic')
    message = data.get('message')
    if topic == "lowpowerstate":
        mqtt.publish(topic, message)
    else:
        current_state = message
        mqtt.publish(topic, message)
    print(f"Published '{message}' to {topic}")

udp_thread = threading.Thread(target=listen_to_udp)
udp_thread.daemon = True
udp_thread.start()

    
@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    print(level, buf)

if __name__ == '__main__':

    local_ip = "192.168.137.168"
    print(f" * Serving on http://{local_ip}:80/ (Press CTRL+C to quit)")
    socketio.run(app, host='0.0.0.0', port=5000, use_reloader=False, debug=True)
