function sendCommand(command) {
    fetch(`/send_command/${command}`);
}

function handleCoords(index, lat, lng) {
    fetch(`/handle_coords/${index}/${lat}/${lng}`);
}

function sendCoords() {
    fetch(`/send_coords`);
}

function resetCoords(){
    fetch(`/reset_coords`);
}

