from flask import Flask, render_template, Response, jsonify  # Import jsonify
from picamera2 import Picamera2
import cv2
import smbus2
import time

app = Flask(__name__)

ARDUINO_ADDRESS = 0x08
bus = smbus2.SMBus(1)  # Initialize the bus here

def send_data(data):
    with bus:  # Use the existing bus instance
        bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(c) for c in data])

# Initialize the camera
camera = None

def initialize_camera():
    global camera
    if camera is None:
        try:
            camera = Picamera2()
            camera.configure(camera.create_preview_configuration(main={"format": 'XRGB8888', "size": (320, 240)}))
            camera.start()
        except Exception as e:
            print(f"Camera init error: {e}")

@app.route('/')
def index():
    """Video streaming home page."""
    initialize_camera()  # Ensure camera is initialized
    return render_template('index.html')

@app.route('/send_command/<command>')
def handle_command(command):
    send_data(command)
    return f"Command '{command}' sent to Arduino."

def generate_frames():
    """Video streaming generator function."""
    while True:
        frame = camera.capture_array()  # Capture the frame
        if frame is None or frame.size == 0:
            print("Error: Captured frame is empty.")
            continue  # Skip this iteration if frame capture failed
        
        ret, jpeg = cv2.imencode('.jpg', frame)  # Encode the frame
        if not ret:
            print("Error: Frame could not be encoded.")
            continue  # Skip this iteration if encoding fails
        
        frame = jpeg.tobytes()  # Convert to bytes for streaming
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

@app.route('/video_feed')
def video_feed():
    """Video streaming route. Put this in the src attribute of an img tag."""
    initialize_camera()  # Ensure camera is initialized before streaming
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/request_data')
def request_data():
    bus.write_byte(ARDUINO_ADDRESS, 0)  # Sending a dummy byte to request data
    time.sleep(0.1)  # Allow some time for Arduino to prepare the data

    # Assuming you have a function to read the data from Arduino
    data = bus.read_i2c_block_data(ARDUINO_ADDRESS, 0, 32)  # Adjust size as needed
    return jsonify({"data": ''.join(chr(i) for i in data if i > 0)})  # Convert byte data to string

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=False, threaded=True)
