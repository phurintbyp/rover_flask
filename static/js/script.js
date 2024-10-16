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

function handleButtonPress(command, buttonId) {
    // Check if no key is currently active
    if (!activeKey) {
        activeKey = command;  // Set the command as the active key
        keyPressed[command] = true;  // Mark the command as pressed
        console.log(command + " button is pressed");

        // Change the appearance of the button
        const button = document.getElementById(buttonId);
        if (button) {
            button.style.backgroundColor = '#0056b3';  // Optional: Change button appearance
        }
    }
}

// Function to handle button release (similar to keyup)
function handleButtonRelease(command, buttonId) {
    // Check if the released button corresponds to the active key
    if (keyPressed[command] && command === activeKey) {
        keyPressed[command] = false;  // Mark the command as released
        activeKey = null;  // Reset the active key
        console.log(command + " button is released");

        // Revert the appearance of the button
        const button = document.getElementById(buttonId);
        if (button) {
            button.style.backgroundColor = '';  // Reset button appearance
        }
    }
}

const buttons = ['qButton', 'wButton', 'eButton', 'rButton', 'uButton', 'oButton', 'aButton', 'sButton', 'dButton', 'fButton', 'jButton', 'kButton', 'lButton', 'hButton'];

buttons.forEach(buttonId => {
    // Get the command from the buttonId (remove 'Button' and convert to lowercase)
    const command = buttonId.charAt(0).toLowerCase();

    // Add mousedown listener (for pressing the button)
    document.getElementById(buttonId).addEventListener('mousedown', function() {
        handleButtonPress(command, buttonId);
    });

    // Add mouseup listener (for releasing the button)
    document.getElementById(buttonId).addEventListener('mouseup', function() {
        handleButtonRelease(command, buttonId);
    });
});