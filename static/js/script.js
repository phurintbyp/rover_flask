// Endpoints

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

// Button functions

let keyPressed = {};  // To track which keys are currently pressed
let activeKey = null; // Track the currently active key
let releaseTimeout;

// Allowed keys
const allowedKeys = ['q', 'w', 'e', 'r', 'u', 'o', 'a', 's', 'd', 'f', 'j', 'k', 'l', 'h'];

// Map spacebar to 'h'
document.addEventListener('keydown', function(event) {
    let key = event.key.toLowerCase();  // Convert key to lowercase for consistency

    if (key === ' ') {
        key = 'h';  // Map spacebar to 'h'
    }

    // Check if the key is allowed
    if (allowedKeys.includes(key)) {
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
    }
});

document.addEventListener('keyup', function(event) {
    let key = event.key.toLowerCase();

    if (key === ' ') {
        key = 'h';  // Map spacebar to 'h'
    }

    // Check if the key is allowed
    if (allowedKeys.includes(key)) {
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
    }
});

// Handle button presses with mouse events
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

function handleButtonRelease(command, buttonId) {
    if (keyPressed[command] && command === activeKey) {
        // Clear any previous timeout to avoid multiple triggers
        clearTimeout(releaseTimeout);

        // Add delay before resetting the button and sending 'stop' command
        releaseTimeout = setTimeout(function() {
            keyPressed[command] = false;
            activeKey = null;
            console.log(command + " button is released");

            // Reset button appearance
            const button = document.getElementById(buttonId);
            if (button) {
                button.style.backgroundColor = '';
            }
        }, 200);  // Delay of 200ms (adjust as needed)
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

let currentLayerType = null;
let layer = null;

let drawPath = false; // Initial value of drawPath

// Get the switch element
const drawPathSwitch = document.getElementById('drawPathSwitch');

// Add an event listener to toggle between drawPath being true or false
drawPathSwitch.addEventListener('change', function() {
    drawPath = this.checked; // Set drawPath to true if checked, false otherwise
    toggleDrawPath(); // Call function to toggle the functionality
});

// Set initial coordinates of the rover
var roverCoords = [13.7271, 100.7747 ];  // This should be updated with actual rover coordinates

var map = L.map('map').setView(roverCoords, 13); // Set initial view to rover's location

var drawnFeatures = new L.FeatureGroup();
map.addLayer(drawnFeatures);

var drawControl = new L.Control.Draw({
    edit: {
        featureGroup: drawnFeatures,
        remove: true
    }
});

// Function to toggle functionality based on drawPath value
function toggleDrawPath() {
    if (drawPath) {
        console.log("Draw path enabled");
        enableDrawControl(); // Enable draw functionality
        disableClickControl(); // Disable click functionality
        enableInputs();
        drawnFeatures.clearLayers();
        if (routeControl) {
            map.removeControl(routeControl); // Remove existing route if any
        }
    } else {
        console.log("Map click functionality enabled");
        disableDrawControl(); // Disable draw functionality
        enableClickControl(); // Enable click functionality
        disableInputs();
        drawnFeatures.clearLayers();
        if (routeControl) {
            map.removeControl(routeControl); // Remove existing route if any
        }
    }
}

function getNumberFieldValue() {
    const numberField = document.getElementById('quality');
    const quality = parseInt(numberField.value, 10);  // Convert value to an integer
    return quality;
}

function generateCircleCoords(
    lat, 
    lng, 
    radius, // Radius in meters
    quality
) {
    console.log("Generating Coords");
    const coords = []; // Array to store coordinates
    const numPoints = Math.floor(quality * (radius / 100)); // Adjust points based on radius and quality

    // Step 1: Calculate initial points around the circle
    for (let i = 0; i < numPoints; i++) {
        const angle = (2 * Math.PI / numPoints) * i; // Angle in radians

        // Calculate new latitude and longitude using basic trigonometry
        const newLat = lat + (radius / 111320) * Math.cos(angle);
        const newLng = lng + (radius / (111320 * Math.cos(lat * Math.PI / 180))) * Math.sin(angle);

        coords.push([newLat, newLng]); // Store the initial coordinate
    }

    // Step 2: Calculate the mean distance from the center
    let totalDistance = 0;
    coords.forEach(([lat2, lng2]) => {
        const dist = euclideanDistance(lat, lng, lat2, lng2);
        totalDistance += dist;
    });

    const meanDistance = totalDistance / numPoints; // Mean distance

    // Step 3: Adjust each coordinate to align with the desired radius
    const adjustedCoords = coords.map(([lat2, lng2]) => {
        const dist = euclideanDistance(lat, lng, lat2, lng2);
        const adjustment = radius / dist;

        // Adjust the latitude and longitude proportionally
        const adjustedLat = lat + (lat2 - lat) * adjustment;
        const adjustedLng = lng + (lng2 - lng) * adjustment;

        return [adjustedLat, adjustedLng];
    });

    console.log("Adjusted Coords:", adjustedCoords);
    return adjustedCoords;
}

// Helper function to calculate Euclidean distance between two points
function euclideanDistance(lat1, lng1, lat2, lng2) {
    const dLat = (lat2 - lat1) * 111320; // Convert degrees to meters
    const dLng = (lng2 - lng1) * 111320 * Math.cos(lat1 * Math.PI / 180);
    return Math.sqrt(dLat ** 2 + dLng ** 2); // Pythagorean theorem
}

function handleCoordsList(coordsList){
    let lastCoord = null;
    let length = 0;
    resetCoords();

    coordsList.forEach(function(coord, index) {
        const currentCoord = {
            i: index,
            lat: parseFloat(coord.lat.toFixed(4)),  // Limit latitude to 4 decimal places
            lng: parseFloat(coord.lng.toFixed(4))   // Limit longitude to 4 decimal places
        };

        // Check if current coordinate is different from the last sent coordinate
        if (!lastCoord || lastCoord.lat !== currentCoord.lat || lastCoord.lng !== currentCoord.lng) {
            handleCoords(currentCoord.i, currentCoord.lat, currentCoord.lng);  // Send the coordinate if it's not a duplicate
            length+=1;
            console.log(`Coords sent!\nIndex: ${index}\nlat: ${currentCoord.lat}\nlong: ${currentCoord.lng}`);
                    
            // Update the lastCoord to the current one after sending
            lastCoord = currentCoord;   
        } else {
            console.log(`Duplicate coordinate, not sent!\nIndex: ${index}\nlat: ${currentCoord.lat}\nlong: ${currentCoord.lng}`);
        }
    });
    console.log("Start sending coords to arduino");
    sendCoords(length);
}

function enableDrawControl(){
    map.on("draw:created", function (e) {
        currentLayerType = e.layerType;
        layer = e.layer;
        console.log(e);

        drawnFeatures.clearLayers();
        drawnFeatures.addLayer(layer);

        layer.bindPopup(`<p>${JSON.stringify(layer.toGeoJSON())}</p>`);
    });

    map.on("draw:edited", function(e){
        var layers = e.layers;
        var type = e.layerType;

        layers.eachLayer(function(layer){
            console.log(layer);
        })

        if(layerType=='marker'){
            //Do something
        }
    })

    map.on("draw:deleted", function (e) {
        var layers = e.layers;
        layers.eachLayer(function (layer) {
            console.log('Deleted Layer:', layer);
        });
    });
}

function disableDrawControl() {
    map.off("draw:created"); // Disable draw:created event listener
    map.off("draw:edited");
    map.off("draw:deleted");
}

// Add OpenStreetMap tile layer
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
    attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

// Marker for the rover
var roverMarker = L.marker(roverCoords).addTo(map).bindPopup("Rover is here").openPopup();

var routeControl;

// Enable map click functionality
function enableClickControl() {
    map.on('click', function (e) {
        var destination = e.latlng;

        if (routeControl) {
            map.removeControl(routeControl); // Remove existing route if any
        }

        // Create a new route
        routeControl = L.Routing.control({
            waypoints: [
                L.latLng(roverCoords), // Start at the rover's current position
                L.latLng(destination)   // End at the clicked location
            ],
            routeWhileDragging: true
        }).on('routesfound', function (e) {
            console.log("Route found:", e);
            let lastCoord = null;
            let length = 0;
            resetCoords();

            e.routes[0].coordinates.forEach(function (coord, index) {
                const currentCoord = { i: index, lat: coord.lat, lng: coord.lng };
                if (!lastCoord || lastCoord.lat !== currentCoord.lat || lastCoord.lng !== currentCoord.lng) {
                    handleCoords(currentCoord.i, currentCoord.lat, currentCoord.lng);
                    length++;
                    console.log(`Coords sent: \nIndex: ${index}, \nlat: ${currentCoord.lat}, \nlng: ${currentCoord.lng}`);

                    lastCoord = currentCoord;
                } else {
                    console.log(`Duplicate coord, not sent: \nIndex: ${index}, \nlat: ${currentCoord.lat}, \nlng: ${currentCoord.lng}`);
                }
            });
            console.log("Start sending coords to Arduino");
            sendCoords(length);
        }).addTo(map);
    });
}

// Disable map click functionality
function disableClickControl() {
    map.off('click'); // Disable the click event listener
}

// Initially enable route finding
disableInputs();

function enableInputs() {
    document.getElementById('quality').disabled = true;
    document.getElementById('getValueButton').disabled = true;
    document.getElementById('generateButton').disabled = true;
    map.addControl(drawControl);
}

// Function to disable the Quality input and Get Value button
function disableInputs() {
    document.getElementById('quality').disabled = false;
    document.getElementById('getValueButton').disabled = false;
    document.getElementById('generateButton').disabled = false;
    map.removeControl(drawControl);
}
