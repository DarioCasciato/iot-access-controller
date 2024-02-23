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
