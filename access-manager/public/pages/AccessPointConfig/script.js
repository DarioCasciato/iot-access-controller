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
    tableBody.innerHTML = '';

    accessPoints.forEach(point => {
        const row = tableBody.insertRow();
        row.insertCell(0).textContent = point.deviceID;

        // Name cell with inline editing
        const nameCell = row.insertCell(1);
        nameCell.textContent = point.name;
        nameCell.setAttribute('contenteditable', true);
        nameCell.addEventListener('blur', () => updateAccessPoint(point.deviceID, nameCell.textContent, notesCell.textContent));

        // Notes cell with inline editing
        const notesCell = row.insertCell(2);
        notesCell.textContent = point.notes || ""; // Display existing notes or an empty string
        notesCell.setAttribute('contenteditable', true);
        notesCell.addEventListener('blur', () => updateAccessPoint(point.deviceID, nameCell.textContent, notesCell.textContent));
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
    // Initialize progress bar
    updateProgressBar(0);

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
                            displayFoundDevice(data.deviceID, ip);
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


function displayFoundDevice(deviceID, ip) {
    const foundDevicesDiv = document.getElementById('foundDevices');
    const deviceElement = document.createElement('div');
    deviceElement.textContent = `Device ID: ${deviceID} (IP: ${ip}) `;
    const addButton = document.createElement('button');
    addButton.textContent = 'Add to System';
    addButton.addEventListener('click', () => addDeviceToSystem(deviceID));
    deviceElement.appendChild(addButton);
    foundDevicesDiv.appendChild(deviceElement);
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
        alert('Device added to the system successfully');
        // Optionally, refresh the access points list
        fetchAccessPoints();
    })
    .catch(error => console.error('Error adding device to system:', error));
}
