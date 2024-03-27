let allLogs = []; // This will store all logs for filtering

document.addEventListener('DOMContentLoaded', () => {
    Promise.all([
        fetch('/api/logs').then(response => response.json()),
        fetch('/api/access-points').then(response => response.json()),
        fetch('/api/users').then(response => response.json())
    ])
    .then(([logs, accessPoints, users]) => {
        // Store all logs after enriching them
        allLogs = enrichLogs(logs, accessPoints, users);
        displayLogs(allLogs); // Display all logs initially
        populateDeviceNameFilter(accessPoints);
        populateUserNameFilter(users);
    })
    .catch(error => {
        console.error('Error:', error);
    });
});

function enrichLogs(logs, accessPoints, users) {
    const deviceNameMap = new Map(accessPoints.map(point => [point.deviceID, point.name]));
    const userNameMap = new Map(users.map(user => [user.cardUID, `${user.name} ${user.familyName}`]));
    return logs.map(log => ({
        ...log,
        deviceName: deviceNameMap.get(log.deviceID) || 'Unknown Device',
        userName: userNameMap.get(log.cardUID) || 'Unknown User'
    }));
}

function populateDeviceNameFilter(accessPoints) {
    const deviceNameDatalist = document.getElementById('deviceNames');
    accessPoints.forEach(point => {
        const option = document.createElement('option');
        option.value = point.name;
        deviceNameDatalist.appendChild(option);
    });
}

function populateUserNameFilter(users) {
    const userNameDatalist = document.getElementById('userNames');
    users.forEach(user => {
        const option = document.createElement('option');
        option.value = `${user.name} ${user.familyName}`;
        userNameDatalist.appendChild(option);
    });

    // Add "Unknown User" option
    const unknownOption = document.createElement('option');
    unknownOption.value = 'Unknown User';
    userNameDatalist.appendChild(unknownOption);
}

function displayLogs(logs) {
    const tableBody = document.getElementById('logTable').querySelector('tbody');
    tableBody.innerHTML = '';

    // Reverse the logs to display the newest first
    const reversedLogs = logs.slice(); // Create a copy and reverse it

    reversedLogs.forEach(log => {
        const row = tableBody.insertRow();
        row.insertCell(0).textContent = log.deviceID;
        row.insertCell(1).textContent = log.deviceName;
        row.insertCell(2).textContent = log.userName;
        row.insertCell(3).textContent = log.cardUID;
        const decisionCell = row.insertCell(4);
        decisionCell.textContent = log.accessDecision;
        decisionCell.classList.add(log.accessDecision.toLowerCase());
        row.insertCell(5).textContent = formatTimestamp(log.timestamp);
    });
}

function filterLogs() {
    const deviceName = document.getElementById('filterDeviceName').value.toLowerCase();
    const userName = document.getElementById('filterUserName').value.toLowerCase();
    const filteredLogs = allLogs.filter(log => {
        const deviceMatch = !deviceName || log.deviceName.toLowerCase().includes(deviceName);
        const userMatch = !userName || log.userName.toLowerCase().includes(userName);
        return deviceMatch && userMatch;
    });
    displayLogs(filteredLogs); // Display filtered logs
}

function resetFilters() {
    document.getElementById('filterDeviceName').value = '';
    document.getElementById('filterUserName').value = '';
    displayLogs(allLogs); // Display all logs again
}

function formatTimestamp(isoString) {
    const date = new Date(isoString);
    return `${date.getDate().toString().padStart(2, '0')}-${(date.getMonth() + 1).toString().padStart(2, '0')}-${date.getFullYear()} ${date.getHours().toString().padStart(2, '0')}:${date.getMinutes().toString().padStart(2, '0')}`;
}
