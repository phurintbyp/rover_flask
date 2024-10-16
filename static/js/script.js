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
let activeKey = null; // Track the currently active key

document.addEventListener('keydown', function(event) {
    let key = event.key.toLowerCase();  // Convert key to lowercase for consistency

    // Handle Spacebar separately
    if (key === ' ') {
        key = 'h';  // Map Spacebar to 'h'
    }

    // Check if any key is already active
    if (!activeKey) {
        activeKey = key; // Set active key
        keyPressed[key] = true;  // Mark key as pressed
        console.log(key + " key is held down");

        // Change the appearance of the button (optional)
        const button = document.querySelector(`#${key}Button`);
        if (button) {
            button.style.backgroundColor = '#0056b3';
        }
    }
});

document.addEventListener('keyup', function(event) {
    let key = event.key.toLowerCase();

    if (key === ' ') {
        key = 'h';  // Map Spacebar to 'h'
    }

    // Check if the released key is the active key
    if (keyPressed[key] && key === activeKey) {
        keyPressed[key] = false;  // Mark key as released
        activeKey = null; // Reset active key
        console.log(key + " key is released");

        // Revert the appearance of the button (optional)
        const button = document.querySelector(`#${key}Button`);
        if (button) {
            button.style.backgroundColor = '';  // Reset button color
        }
    }
});
