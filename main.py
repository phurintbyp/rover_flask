from flask import Flask, render_template
import smbus2
import time

app = Flask(__name__)

# Change to IP address of the Raspberry Pi
PI_HOST = '0.0.0.0'

bus = smbus2.SMBus(1)

ARDUINO_ADDRESS = 0x08  # I2C address of Arduino

def send_data(data):
    with smbus2.SMBus(1) as bus:
        bus.write_i2c_block_data(ARDUINO_ADDRESS, 0, [ord(c) for c in data])

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/send_command/<command>')
def handle_command(command):
    send_data(command)
    return f"Command '{command}' sent to Arduino."

if __name__ == '__main__':
    app.run(debug=True, host=PI_HOST, port=5000)
