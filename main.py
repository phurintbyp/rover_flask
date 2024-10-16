from flask import Flask, render_template, Response, jsonify  # Import jsonify
from picamera2 import Picamera2
import cv2
import smbus2
import time
import os

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
        
@app.route('/handle_coords/<index>/<lat>/<long>')
def handle_coords(index, lat, long):
    data = (int(index), float(lat), float(long))  # Store as tuple with correct types
    coords_list.append(data)
    return f"Coords '{data}' received and stored."


@app.route('/send_coords/<max>')
def send_coords(max):
    if not coords_list:
        return "No coordinates to send!", 400
    
    # Send max index
    array_max = f"max: {max}"
    send_string(array_max)
    # Sort coordinates by index
    sorted_coords = sorted(coords_list, key=lambda x: x[0])

    print(f"\nSorted Coords List:\n{sorted_coords}")

    for coord in sorted_coords:
        # Format as string before sending
        data_str = f"{coord[0]} {coord[1]} {coord[2]}"
        print(f"Sending: {data_str}")
        send_string(data_str)

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

def get_temperature():
    # Retrieve the Raspberry Pi temperature using the `vcgencmd` command
    temp_output = os.popen("vcgencmd measure_temp").readline()
    return temp_output.replace("temp=", "").replace("'C\n", "")

@app.route('/temperature', methods=['GET'])
def temperature():
    temp = get_temperature()
    return jsonify({'temperature': temp})

if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', port=5000, debug=False)
    except KeyboardInterrupt:
        print("Server stopping...")