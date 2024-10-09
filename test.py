from picamera2 import Picamera2
import time

picam2 = Picamera2()

# Configure the camera
picam2.configure(picam2.create_preview_configuration())

picam2.start_preview()
picam2.start()

# Give the camera some time to initialize
time.sleep(2)

# Capture a single frame
frame = picam2.capture_array()
print("Captured frame successfully!")

picam2.stop()
