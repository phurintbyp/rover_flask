from flask import Flask, Response
import subprocess

app = Flask(__name__)

@app.route('/video_feed')
def video_feed():
    command = [
        'ffmpeg',
        '-f', 'v4l2',
        '-i', '/dev/video0',
        '-f', 'mpegts',
        'http://192.168.0.140:5000'  # Change to your desired output address
    ]
    return Response(subprocess.Popen(command, stdout=subprocess.PIPE), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    app.run(debug=True)
