function sendCommand(command) {
    fetch(`/send_command/${command}`);
}

function sendCoords(lat, lng) {
    fetch(`/send_coords/${lat}/${lng}`)
}
