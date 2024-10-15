function sendCommand(command) {
    fetch(`/send_command/${command}`);
}

function handleCoords(lat, lng) {
    fetch(`/handle_coords/${lat}/${lng}`);
}

function sendCoords() {
    fetch(`/send_coords`);
}

