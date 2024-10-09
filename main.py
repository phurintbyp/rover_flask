# main.py
from flask import Flask, render_template, Response
import smbus2
from camera import Camera  # Import the Camera class from camera.py

app = Flask(__name__)

# Change to IP address of the Raspberry Pi
PI_HOST = '0.0.0.0'

bus = smbus2.SMBus(1)
ARDUINO_ADDRESS = 0x08  # I2C address of Arduino
camera = Camera()  # Create a Camera object

def send_data(data):
    with smbus2.SMBus(1) as bus:
        bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(c) for c in data])

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/send_command/<command>')
def handle_command(command):
    send_data(command)
    return f"Command '{command}' sent to Arduino."

@app.route('/video_feed')
def video_feed():
    return Response(camera.generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(debug=True, host=PI_HOST, port=5000)
