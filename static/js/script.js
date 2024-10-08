function displayWebcamFeed() {
    // URL of the Raspberry Pi camera stream
    const webcamFeedUrl = 'http://192.168.0.152:8080/stream';  // Replace with your actual stream URL

    // Select the div where the camera feed will be displayed
    const webcamFeedDiv = document.getElementById('camera-feed');

    // Create a new <img> element to display the camera feed
    const webcamImage = document.createElement('img');

    // Set the source of the image to the camera stream URL
    webcamImage.src = webcamFeedUrl;
    webcamImage.alt = 'Camera Feed';

    // Append the <img> element to the div
    webcamFeedDiv.appendChild(webcamImage);
}

// Call the function when the page is loaded
window.onload = function() {
    displayWebcamFeed();
};
