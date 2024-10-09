from flask import Flask, render_template, Response
import smbus2
import time
import io
from picamera2 import Picamera2
from PIL import Image
import cv2

app = Flask(__name__)

# Change to IP address of the Raspberry Pi
PI_HOST = '0.0.0.0'

bus = smbus2.SMBus(1)
ARDUINO_ADDRESS = 0x08  # I2C address of Arduino

camera = Picamera2()
# camera.configure(camera.create_preview_configuration(main={"format": 'XRGB8888', "size": (640, 480)}))
camera_config = camera.create_preview_configuration()
camera.configure(camera_config)
camera.start()

def send_data(data):
    with smbus2.SMBus(1) as bus:
        bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(c) for c in data])

def generate_frames():
    while True:
        frame = camera.capture_array()
        ret, buffer = cv2.imencode('.jpg', frame)
        frame = buffer.tobytes()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/send_command/<command>')
def handle_command(command):
    send_data(command)
    return f"Command '{command}' sent to Arduino."

@app.route('/video_feed')
def video_feed():
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(debug=True, host=PI_HOST, port=8080, threaded=True)
