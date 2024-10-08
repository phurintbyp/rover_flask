function sendCommand(command) {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', '/send_command/' + command, true);
    xhr.send();
}

function displayWebcamFeed() {
    const webcamFeedUrl = 'http://<raspberry_pi_ip>:8080/stream';
    const webcamFeedDiv = document.getElementById('camera-feed');
    const webcamImage = document.createElement('img');
    webcamImage.src = webcamFeedUrl;
    webcamImage.alt = 'Camera Feed';
    webcamFeedDiv.appendChild(webcamImage);
}

window.onload = function() {
    displayWebcamFeed();
};
