from flask import Flask, render_template, Response
from picamera2 import Picamera2
import cv2

app = Flask(__name__)

# Initialize the camera
camera = None

def initialize_camera():
    global camera
    if camera is None:
        try:
            camera = Picamera2()
            camera.configure(camera.create_preview_configuration(main={"format":'XRGB8888', "size": (640, 480)}))
            camera.start()
        except Exception as e:
            print(f"Camera init error: {e}")

@app.route('/')
def index():
    """Video streaming home page."""
    initialize_camera()  # Ensure camera is initialized
    return render_template('index.html')

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

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=False, threaded=True)
