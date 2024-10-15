from flask import Flask, render_template, Response, jsonify  # Import jsonify
from picamera2 import Picamera2
import cv2
import smbus2
import time

app = Flask(__name__)

ARDUINO_ADDRESS = 0x08
bus = smbus2.SMBus(1)  # Initialize the bus here

coords_list = []

def send_data(data):
    bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(data)])  # Send the single character as its ASCII value

def send_string(data):
    ascii_values = [ord(char) for char in data]
    bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, ascii_values)


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
        
@app.route('/handle_coords/<lat>/<long>')
def handle_coords(lat, long):
    data = f"lat: {lat}, lng: {long}"  # Format the string
    coords_list.append(data)
    return f"Coords '{data}' sent to the PI."

@app.route('/send_coords')
def send_coords ():
    if not coords_list:  # If coords_list is empty
        return "No coordinates to send!", 400  # HTTP 400 Bad Request
    
    print(f"\nCoords_list:\n{coords_list}")

    for coord in coords_list :
        print(f"\nCoordinates: \n{coord}")
        send_string(coord)
        # if wait_for_ack():
        #     print(f"Reached: {coord}, sending next...")

    return "All coordinates sent!"

@app.route('/reset_coords')
def reset_coords():
    coords_list.clear()
    return "Coordinates list cleared"

def wait_for_ack():
    while True:
        try:
            # Read a byte from Arduino, assuming it sends '1' when target is reached
            data = bus.read_byte(ARDUINO_ADDRESS)
            if data == ord('1'):
                return True  # Acknowledgement received
        except:
            pass

        time.sleep(0.1)


@app.route('/video_feed')
def video_feed():
    """Video streaming route. Put this in the src attribute of an img tag."""
    initialize_camera()  # Ensure camera is initialized before streaming
    return Response(generate_frames(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=False, threaded=True)
