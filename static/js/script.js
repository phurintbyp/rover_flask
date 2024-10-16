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

const wButton = document.getElementById('wButton');
let wKeyPressed = false;

// Function to handle the 'W' key press
function handleKeyDown(event) {
    if ((event.key === 'w' || event.key === 'W') && !wKeyPressed) {
        wKeyPressed = true;
        console.log("W key is held down");
        wButton.style.backgroundColor = '#0056b3';  // Optional: Change button appearance when active
    }
}

// Function to handle the 'W' key release
function handleKeyUp(event) {
    if (event.key === 'w' || event.key === 'W') {
        console.log("W key is released");
        wButton.style.backgroundColor = '';  // Optional: Revert button appearance
    }
}

// Add event listeners for keydown and keyup
document.addEventListener('keydown', handleKeyDown);
document.addEventListener('keyup', handleKeyUp);