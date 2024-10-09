from flask import Flask, render_template, Response
import smbus2
import time
import io
from picamera2 import Picamera2
from PIL import Image

app = Flask(__name__)

# Change to IP address of the Raspberry Pi
PI_HOST = '0.0.0.0'

bus = smbus2.SMBus(1)
ARDUINO_ADDRESS = 0x08  # I2C address of Arduino

def send_data(data):
    with smbus2.SMBus(1) as bus:
        bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(c) for c in data])

def generate_frames():
    picam2 = Picamera2()  # Initialize Picamera2
    config = picam2.create_video_configuration(main={"size": (640, 480)})
    picam2.configure(config)
    picam2.start()

    while True:
        frame = picam2.capture_array()
        img = Image.fromarray(frame)
        stream = io.BytesIO()
        img.save(stream, 'JPEG')
        stream.seek(0)
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + stream.read() + b'\r\n')

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
    app.run(debug=True, host=PI_HOST, port=5000, threaded=True)
