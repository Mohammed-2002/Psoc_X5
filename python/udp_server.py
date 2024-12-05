'''
import socket
import optparse
import time
import sys

#DEFAULT_IP   = socket.gethostbyname(socket.gethostname())   # IP address of the UDP server
DEFAULT_IP   = "192.168.57.204"   # IP address of the UDP server
DEFAULT_PORT = 57345             # Port of the UDP server

LED_ON = '1'
LED_OFF = '0'

sensor_state = "LED OFF"

def enter_command(sock, addr):
    print("============================================================")
    cmd = int(input('Enter 0 to turn off or 1 to turn on the LED on Client\nInput Command: '))
    if cmd == 1:
        sock.sendto(bytes(LED_ON, "utf-8"), addr)
    elif cmd == 0:
        sock.sendto(bytes(LED_OFF, "utf-8"), addr)
    else:
        sock.sendto(bytes(str(cmd), "utf-8"), addr)

def receive_messages(sock):
    while True:
        try:
            data, addr = sock.recvfrom(4096)
            message = data.decode("utf-8")  # Decode the incoming byte data

            if message == 'A':
                print("Client connected and ready.")
            
            elif message == 'LED ON ACK':
                print("Acknowledgment received: LED turned ON.")
            elif message == 'LED OFF ACK':
                print("Acknowledgment received: LED turned OFF.")
            elif message == 'INVALID CMD RECEIVED':
                print("Client received an invalid command.")
            
            else:
                print(f"Message from Client: {message}")

            sensor_state = message

        except Exception as e:
            print("An error occurred while receiving:", e)
            break  # Exit the loop if a fatal error occurs

def echo_server(host, port):
    print("============================================================")
    print("UDP Server")
    print("============================================================")
    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #Bind UDP Server
    sock.bind((host,port))
    print('UDP Server on IP Address: {} port {}'.format(host, port))
    print('waiting to receive message from UDP Client')
    
    receive_messages(sock)

if __name__ == '__main__':
    parser = optparse.OptionParser()
    parser.add_option("-p", "--port", dest="port", type="int", default=DEFAULT_PORT, help="Port to listen on [default: %default].")
    parser.add_option("--hostname", dest="hostname", default=DEFAULT_IP, help="Hostname to listen on.")

    (options, args) = parser.parse_args()

    echo_server(options.hostname, options.port)
'''

import socket
import json

# Define the UDP server's address and port
UDP_IP = "192.168.137.1"  # Replace with the server's IP address
UDP_PORT = 57345           # Replace with the server's port

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket if required (or skip if server sends unicast directly to you)
sock.bind((UDP_IP, UDP_PORT))

print(f"Listening for UDP data on {UDP_IP}:{UDP_PORT}...")

while True:
    try:
        # Receive data from the UDP server
        data, addr = sock.recvfrom(1024)  # Buffer size
        message = data.decode('utf-8')
        print(f"Received message from {addr}: {message}")
        
        # Try parsing JSON if the data is in JSON format
        try:
            parsed_data = json.loads(message)
            print("Parsed JSON Data:", parsed_data)
        except json.JSONDecodeError:
            print("Received malformed JSON data:", message)

    except KeyboardInterrupt:
        print("UDP Client stopped.")
        break
    except Exception as e:
        print(f"Error: {e}")
