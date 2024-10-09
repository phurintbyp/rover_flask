# camera.py
from picamera2 import Picamera2
import io
import cv2

class Camera:
    def __init__(self):
        self.camera = Picamera2()
        self.camera.configure(self.camera.create_preview_configuration(main={"format": 'XRGB8888', "size": (640, 480)}))
        self.camera.start()

    def generate_frames(self):
        while True:
            try:
                frame = self.camera.capture_array()
                ret, buffer = cv2.imencode('.jpg', frame)
                frame = buffer.tobytes()
                yield (b'--frame\r\n'
                       b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')
            except Exception as e:
                print(f"Error capturing frame: {e}")
