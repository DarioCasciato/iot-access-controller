document.addEventListener('DOMContentLoaded', () => {
    Promise.all([
        fetch('/api/logs').then(response => response.json()),
        fetch('/api/access-points').then(response => response.json())
    ])
    .then(([logs, accessPoints]) => {
        // Create a map of device IDs to names
        const deviceNameMap = new Map(accessPoints.map(point => [point.deviceID, point.name]));
        // Add device names to logs
        const logsWithDeviceName = logs.map(log => ({
            ...log,
            deviceName: deviceNameMap.get(log.deviceID) || 'Unknown Device'
        }));
        displayLogs(logsWithDeviceName);
        populateFilters(logs);
    })
    .catch(error => {
        console.error('Error:', error);
    });
});

// Fetch logs from the server
function fetchLogs() {
    fetch('/api/logs')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            displayLogs(data); // Display the logs first
            populateFilters(data); // Then populate filters based on the fetched data
        })
        .catch(error => {
            console.error('Error:', error);
        });
}

// Display logs in the table
function displayLogs(logs) {
    const tableBody = document.getElementById('logTable').querySelector('tbody');
    tableBody.innerHTML = ''; // Clear existing table rows

    logs.forEach(log => {
        const row = tableBody.insertRow();
        row.insertCell(0).textContent = log.deviceID;
        row.insertCell(1).textContent = log.deviceName; // Assuming this is already implemented
        row.insertCell(2).textContent = log.userIndex;

        const decisionCell = row.insertCell(3);
        decisionCell.textContent = log.accessDecision;
        // Add class based on the access decision
        if (log.accessDecision === 'Granted') {
            decisionCell.classList.add('granted');
        } else if (log.accessDecision === 'Denied') {
            decisionCell.classList.add('denied');
        }

        // Inside the displayLogs function, when adding class based on the access decision
        const decision = log.accessDecision.toLowerCase();
        if (decision === 'granted') {
            decisionCell.classList.add('granted');
        } else if (decision === 'denied') {
            decisionCell.classList.add('denied');
        }

        row.insertCell(4).textContent = formatTimestamp(log.timestamp);
    });
}


// Filter logs based on inputs
function filterLogs() {
    const deviceId = document.getElementById('filterDeviceId').value.toLowerCase();
    const userIndex = document.getElementById('filterUserIndex').value.toLowerCase();

    fetch('/api/logs')
        .then(response => response.json())
        .then(data => {
            // Filter data based on deviceId and userIndex
            const filteredData = data.filter(log => {
                return (log.deviceID.toLowerCase().includes(deviceId) &&
                        log.userIndex.toString().toLowerCase().includes(userIndex));
            });
            displayLogs(filteredData);
        })
        .catch(error => console.error('Error:', error));
}

// format timestamp
function formatTimestamp(isoString) {
    const date = new Date(isoString);
    const day = date.getDate().toString().padStart(2, '0');
    const month = (date.getMonth() + 1).toString().padStart(2, '0'); // Months are 0-indexed
    const year = date.getFullYear();
    const hours = date.getHours().toString().padStart(2, '0');
    const minutes = date.getMinutes().toString().padStart(2, '0');
    return `${day}-${month}-${year} ${hours}:${minutes}`;
}

function populateFilters(logs) {
    const deviceIdSet = new Set(logs.map(log => log.deviceID));
    const userIndexSet = new Set(logs.map(log => log.userIndex));

    const deviceIdDatalist = document.getElementById('deviceIds');
    deviceIdSet.forEach(id => {
        const option = document.createElement('option');
        option.value = id;
        deviceIdDatalist.appendChild(option);
    });

    const userIndexDatalist = document.getElementById('userIndices');
    userIndexSet.forEach(index => {
        const option = document.createElement('option');
        option.value = index;
        userIndexDatalist.appendChild(option);
    });
}