from flask import Flask, render_template, Response
import smbus2
import time
import io
import picamera

app = Flask(__name__)

# Change to IP address of the Raspberry Pi
PI_HOST = '0.0.0.0'

bus = smbus2.SMBus(1)

ARDUINO_ADDRESS = 0x08  # I2C address of Arduino

def send_data(data):
    with smbus2.SMBus(1) as bus:
        bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(c) for c in data])

def generate_frames():
    with picamera.PiCamera() as camera:
        camera.resolution = (640, 480)
        camera.framerate = 24
        stream = io.BytesIO()

        for _ in camera.capture_continuous(stream, 'jpeg', use_video_port=True):
            stream.seek(0)
            yield b'--frame\r\nContent-Type: image/jpeg\r\n\r\n' + stream.read() + b'\r\n'
            stream.seek(0)
            stream.truncate()

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
