function sendCommand(command) {
    fetch(`/send_command/${command}`);
}

function handleCoords(index, lat, lng) {
    fetch(`/handle_coords/${index}/${lat}/${lng}`);
}

function sendCoords(max) {
    fetch(`/send_coords/${max}`);
}

function resetCoords(){
    fetch(`/reset_coords`);
}

let keyPressed = {};  // To track which keys are currently pressed

document.addEventListener('keydown', function(event) {
    let key = event.key.toLowerCase();  // Convert key to lowercase for consistency

    // Handle Spacebar separately
    if (key === ' ') {
        key = 'h';  // Map Spacebar to 'h'
    }

    // Check if the key is already pressed
    if (!keyPressed[key]) {
        keyPressed[key] = true;  // Mark key as pressed
        console.log(key + " key is held down");

        // Change the appearance of the button (optional)
        const button = document.querySelector(`#${key}Button`);
        if (button) {
            button.style.backgroundColor = '#0056b3';
        }

        // Send command
        const xhr = new XMLHttpRequest();
        xhr.open('GET', '/send_command/' + key, true);
        xhr.send();
    }
});

document.addEventListener('keyup', function(event) {
    let key = event.key.toLowerCase();

    if (key === ' ') {
        key = 'h';  // Map Spacebar to 'h'
    }

    if (keyPressed[key]) {
        keyPressed[key] = false;  // Mark key as released
        console.log(key + " key is released");

        // Revert the appearance of the button (optional)
        const button = document.querySelector(`#${key}Button`);
        if (button) {
            button.style.backgroundColor = '';  // Reset button color
        }

        // Send stop command or any other release-based action
        const xhr = new XMLHttpRequest();
        xhr.open('GET', '/send_command/stop', true);  // Change 'stop' to the appropriate command
        xhr.send();
    }
});
