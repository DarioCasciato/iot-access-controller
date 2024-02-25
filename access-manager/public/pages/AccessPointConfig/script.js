document.addEventListener('DOMContentLoaded', () => {
    fetchAccessPoints();
});

function fetchAccessPoints() {
    fetch('/api/access-points')
        .then(response => response.json())
        .then(data => {
            populateAccessPointsTable(data);
        })
        .catch(error => console.error('Error fetching access points:', error));
}


function makeCellEditable(cell, deviceID, field, originalValue) {
    cell.onclick = function() {
        const input = document.createElement('input');
        input.type = 'text';
        input.value = originalValue;
        input.onblur = function() {
            const newValue = input.value.trim();
            if (newValue !== originalValue) { // Only update if value has changed
                updateAccessPoint(deviceID, field, newValue);
            }
            cell.textContent = newValue; // Update cell text regardless of change
            cell.onclick = null; // Re-enable the onclick event
            makeCellEditable(cell, deviceID, field, newValue); // Reattach editable functionality
        };
        cell.innerHTML = ''; // Clear cell content
        cell.appendChild(input);
        input.focus(); // Focus on the new input
        cell.onclick = null; // Disable onclick event to avoid recursion
    };
}

function updateAccessPoint(deviceID, field, newValue) {
    const body = { deviceID };
    // Use 'newName' or 'newNotes' depending on the field being updated
    const updateField = field === 'name' ? 'newName' : 'newNotes';
    body[updateField] = newValue;

    fetch('/api/access-points/update', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(body)
    })
    .then(response => response.json())
    .then(data => {
        console.log(data.message); // Log success message
        // Optionally refresh or display a message
    })
    .catch(error => {
        console.error('Error updating access point:', error);
        // Optionally display error message
    });
}


function appendActionButtons(actionsCell, point) {
    // Append Identify button
    const identifyButton = document.createElement('button');
    identifyButton.textContent = 'Identify';
    identifyButton.onclick = () => identifyDevice(point.ip);
    actionsCell.appendChild(identifyButton);

    // Append Delete button
    const deleteButton = document.createElement('button');
    deleteButton.textContent = 'Delete';
    deleteButton.onclick = () => deleteAccessPoint(point.deviceID);
    actionsCell.appendChild(deleteButton);
}

document.getElementById('scanDevices').addEventListener('click', scanForDevices);

function scanForDevices() {
    // Initialize progress bar and found devices array
    updateProgressBar(0);
    let foundDevices = []; // Array to hold found devices

    fetch('/api/access-points')
    .then(response => response.json())
    .then(existingDevices => {
        const deviceMap = new Map(existingDevices.map(device => [device.deviceID, device.ip]));
        const startIp = 1; // Start of your IP range
        const endIp = 254; // End of your IP range
        let currentIp = startIp;

        const scanNextDevice = () => {
            if (currentIp > endIp) {
                console.log('Scanning complete.');
                if (foundDevices.length === 0) {
                    alert('No new or changed devices found.');
                } else {
                    displayFoundDevices(foundDevices); // Display all found devices after scanning is complete
                }
                return;
            }

            let ip = `192.168.1.${currentIp}`;
            fetch(`/api/device-scan/${ip}`)
                .then(response => response.json())
                .then(data => {
                    // Check if deviceID is known and if IP has changed
                    if (data.deviceID && (!deviceMap.has(data.deviceID) || deviceMap.get(data.deviceID) !== data.ip)) {
                        console.log(`Found new or changed device ID ${data.deviceID} at IP ${ip}`);
                        foundDevices.push({ deviceID: data.deviceID, ip });
                    }
                })
                .catch(error => console.log(error.message))
                .finally(() => {
                    currentIp++;
                    updateProgressBar(((currentIp - startIp) / (endIp - startIp + 1)) * 100);
                    scanNextDevice();
                });
        };

        scanNextDevice();
    })
    .catch(error => console.error('Error fetching existing devices:', error));
}


function updateProgressBar(percentage) {
    const progressBar = document.getElementById('progressBar');
    progressBar.style.width = `${percentage}%`;
}


function populateAccessPointsTable(accessPoints) {
    const tableBody = document.getElementById('accessPointsTable').querySelector('tbody');
    tableBody.innerHTML = ''; // Clear existing table content

    accessPoints.forEach(point => {
        const row = tableBody.insertRow();
        row.insertCell(0).textContent = point.deviceID;

        // Editable Name Cell
        const nameCell = row.insertCell(1);
        nameCell.textContent = point.name;
        makeCellEditable(nameCell, point.deviceID, 'name', point.name);

        // Editable Notes Cell
        const notesCell = row.insertCell(2);
        notesCell.textContent = point.notes || "";
        makeCellEditable(notesCell, point.deviceID, 'notes', point.notes || "");

        // Create the "Actions" cell
        const actionsCell = row.insertCell(3);
        actionsCell.classList.add('actions-cell'); // Add a class for styling if needed

        // Add "Identify" button
        const identifyButton = document.createElement('button');
        identifyButton.textContent = 'Identify';
        identifyButton.onclick = () => identifyDevice(point.ip); // Assuming identifyDevice function accepts ip address
        actionsCell.appendChild(identifyButton);

        // Add "Delete" button and add 'delete-button' class to it
        const deleteButton = document.createElement('button');
        deleteButton.textContent = 'Delete';
        deleteButton.classList.add('delete-button'); // Add this line
        deleteButton.onclick = () => deleteAccessPoint(point.deviceID); // Assuming deleteAccessPoint function accepts deviceID
        actionsCell.appendChild(deleteButton);
    });
}



function addDeviceToSystem(deviceID, ip) {  // Make sure to pass the IP here
    const newName = prompt("Enter name for the new device:", "New Device");
    const newNotes = "";

    fetch('/api/access-points/update', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ deviceID, ip, newName, newNotes })  // Include the IP address in the request body
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        console.log(data.message);
        alert('Device added to the system successfully');
        fetchAccessPoints(); // Refresh the list of access points
    })
    .catch(error => console.error('Error adding device to system:', error));
}

function deleteAccessPoint(deviceID) {
    if (!confirm(`Are you sure you want to delete the device with ID ${deviceID}?`)) {
        return; // Exit if the user cancels the operation
    }

    // Send a request to your server's endpoint for deleting an access point
    fetch(`/api/access-points/delete`, {
        method: 'POST', // or 'DELETE', depending on how your API is designed
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ deviceID })
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        console.log(data.message);
        alert('Device deleted successfully');
        fetchAccessPoints(); // Refresh the list of access points
    })
    .catch(error => console.error('Error deleting device:', error));
}

function identifyDevice(ip) {
    fetch(`http://${ip}/ping`)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not OK');
            }
            console.log(`Identify signal sent to device at IP ${ip}`);
        })
        .catch(error => {
            console.error('Error sending identify signal:', error);
        });
}

function displayFoundDevices(foundDevices) {
    const foundDevicesDiv = document.getElementById('foundDevices');
    foundDevicesDiv.innerHTML = ''; // Clear previous results

    foundDevices.forEach(device => {
        const deviceElement = document.createElement('div');
        deviceElement.textContent = `Device ID: ${device.deviceID} (IP: ${device.ip}) `;

        // Add to System button
        const addButton = document.createElement('button');
        addButton.textContent = 'Add to System';
        addButton.addEventListener('click', () => addDeviceToSystem(device.deviceID, device.ip));
        deviceElement.appendChild(addButton);

        // Identify button
        const identifyButton = document.createElement('button');
        identifyButton.textContent = 'Identify';
        identifyButton.addEventListener('click', () => identifyDevice(device.ip));
        deviceElement.appendChild(identifyButton);

        foundDevicesDiv.appendChild(deviceElement);
    });
}