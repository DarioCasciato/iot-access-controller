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

function populateAccessPointsTable(accessPoints) {
    const tableBody = document.getElementById('accessPointsTable').querySelector('tbody');
    tableBody.innerHTML = ''; // Clear existing table content

    accessPoints.forEach(point => {
        const row = tableBody.insertRow();
        row.insertCell(0).textContent = point.deviceID;

        // Name cell with inline editing
        const nameCell = row.insertCell(1);
        nameCell.textContent = point.name;
        nameCell.setAttribute('contenteditable', 'true');
        nameCell.classList.add('editable');

        // Notes cell with inline editing
        const notesCell = row.insertCell(2);
        notesCell.textContent = point.notes || "";
        notesCell.setAttribute('contenteditable', 'true');
        notesCell.classList.add('editable');

        // Actions cell
        const actionsCell = row.insertCell(3); // Create an actions cell

        // Identify button for registered devices
        const identifyButton = document.createElement('button');
        identifyButton.textContent = 'Identify';
        identifyButton.addEventListener('click', () => identifyDevice(point.ip));
        actionsCell.appendChild(identifyButton);

        // Delete button
        const deleteButton = document.createElement('button');
        deleteButton.textContent = 'Delete';
        deleteButton.classList.add('delete-button');
        deleteButton.onclick = () => deleteAccessPoint(point.deviceID);
        actionsCell.appendChild(deleteButton); // Append to actionsCell, not deleteCell
    });
}


function updateAccessPoint(deviceID, newName, newNotes) {
    fetch('/api/access-points/update', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ deviceID, newName, newNotes })
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        console.log(data.message);
        // Optionally refresh the list or indicate success to the user
    })
    .catch(error => console.error('Error updating access point:', error));
}

document.getElementById('scanDevices').addEventListener('click', scanForDevices);

function scanForDevices() {
    // Initialize progress bar and found devices array
    updateProgressBar(0);
    let foundDevices = []; // Array to hold found devices

    fetch('/api/access-points')
    .then(response => response.json())
    .then(existingDevices => {
        const registeredDeviceIDs = new Set(existingDevices.map(device => device.deviceID));
        const startIp = 1; // Start of your IP range
        const endIp = 254; // End of your IP range
        let currentIp = startIp;

        const scanNextDevice = () => {
            if (currentIp > endIp) {
                console.log('Scanning complete.');
                displayFoundDevices(foundDevices); // Display all found devices after scanning is complete
                return; // Exit recursion
            }

            let ip = `192.168.1.${currentIp}`;
            fetch(`/api/device-scan/${ip}`)
                .then(response => {
                    if (!response.ok) {
                        throw new Error(`Device at ${ip} did not respond correctly or is not a target device.`);
                    }
                    return response.json();
                })
                .then(data => {
                    if (data.deviceID && !registeredDeviceIDs.has(data.deviceID)) {
                        console.log(`Found unregistered device ID ${data.deviceID} at IP ${ip}`);
                        foundDevices.push({ deviceID: data.deviceID, ip }); // Add to found devices array
                    }
                })
                .catch(error => {
                    console.log(error.message); // Log and skip over devices that don't meet criteria or cause errors
                })
                .finally(() => {
                    // Update progress bar after each IP has been scanned
                    currentIp++;
                    updateProgressBar(((currentIp - startIp) / (endIp - startIp + 1)) * 100);
                    scanNextDevice(); // Recurse to scan the next device
                });
        };

        scanNextDevice(); // Start scanning
    })
    .catch(error => console.error('Error fetching existing devices:', error));
}

function updateProgressBar(percentage) {
    const progressBar = document.getElementById('progressBar');
    progressBar.style.width = `${percentage}%`;
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
        addButton.addEventListener('click', () => addDeviceToSystem(device.deviceID));
        deviceElement.appendChild(addButton);

        // Identify button
        const identifyButton = document.createElement('button');
        identifyButton.textContent = 'Identify';
        identifyButton.addEventListener('click', () => identifyDevice(device.ip));
        deviceElement.appendChild(identifyButton);

        foundDevicesDiv.appendChild(deviceElement);
    });
}

function addDeviceToSystem(deviceID) {
    // Assuming your server expects deviceID, name, and notes for a new device
    const newName = prompt("Enter name for the new device:", "New Device");
    const newNotes = "";

    fetch('/api/access-points/update', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ deviceID, newName, newNotes })
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        console.log(data.message);
        fetchAccessPoints();
        alert('Device added to the system successfully');
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
