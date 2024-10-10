function sendCommand(command) {
    fetch(`/send_command/${command}`);
}
