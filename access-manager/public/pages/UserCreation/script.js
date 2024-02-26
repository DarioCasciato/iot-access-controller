document.addEventListener('DOMContentLoaded', () => {
    fetchUsers();
    fetchAccessPointsForSelect();
    setupEventListeners();
});

function fetchUsers() {
    fetch('/api/users')
        .then(response => response.json())
        .then(users => {
            populateUsersTable(users);
        })
        .catch(error => console.error('Error fetching users:', error));
}

function populateUsersTable(users) {
    const tableBody = document.getElementById('usersTable').querySelector('tbody');
    tableBody.innerHTML = ''; // Clear existing users

    users.forEach(user => {
        const row = tableBody.insertRow();
        row.insertCell(0).textContent = user.name;
        row.insertCell(1).textContent = user.familyName;
        row.insertCell(2).textContent = user.cardUID;

        // Actions cell for delete button
        const actionsCell = row.insertCell(3);
        const deleteButton = document.createElement('button');
        deleteButton.textContent = 'Delete';
        deleteButton.classList.add('delete-button'); // Apply SCSS styles
        deleteButton.onclick = () => deleteUser(user.cardUID);
        actionsCell.appendChild(deleteButton);
    });
}

function fetchAccessPointsForSelect() {
    fetch('/api/access-points')
        .then(response => response.json())
        .then(accessPoints => {
            const select = document.getElementById('accessPointSelect');
            accessPoints.forEach(ap => {
                const option = document.createElement('option');
                option.value = ap.ip;
                option.textContent = ap.name || ap.ip;
                select.appendChild(option);
            });
        })
        .catch(error => console.error('Error fetching access points:', error));
}

function setupEventListeners() {
    document.getElementById('createUserButton').addEventListener('click', () => {
        document.getElementById('createUserModal').style.display = 'block';
    });

    document.getElementById('readCardUIDButton').addEventListener('click', readCardUID);
    document.getElementById('submitUserButton').addEventListener('click', createUser);
}

function readCardUID() {
    const selectedAP = document.getElementById('accessPointSelect').value;
    fetch(`http://${selectedAP}/getcarduid`)
        .then(response => response.json())
        .then(data => {
            if (data.uid) {
                document.getElementById('cardUID').value = parseInt(data.uid, 10).toString(16); // Convert to HEX
            } else if (data === 'timeout') {
                alert('Timeout: No card presented');
            }
        })
        .catch(error => console.error('Error reading card UID:', error));
}

function createUser() {
    const name = document.getElementById('userName').value;
    const familyName = document.getElementById('userFamilyName').value;
    const cardUID = document.getElementById('cardUID').value;

    // Fetch existing users first
    fetch('/api/users')
        .then(response => response.json())
        .then(users => {
            if (users.some(user => user.cardUID === cardUID)) {
                alert('A user with this Card UID already exists.');
                return; // Exit the function if UID already exists
            }

            // Proceed to create user if UID is unique
            fetch('/api/users', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ name, familyName, cardUID })
            })
            .then(response => {
                if (!response.ok) throw new Error('Failed to create user');
                return response.json();
            })
            .then(() => {
                fetchUsers(); // Refresh the user list
                document.getElementById('createUserModal').style.display = 'none'; // Hide modal
            })
            .catch(error => console.error('Error creating user:', error));
        });
}


function deleteUser(cardUID) {
    if (!confirm(`Are you sure you want to delete this user with Card UID: ${cardUID}?`)) return;

    fetch(`/api/users/${cardUID}`, { method: 'DELETE' })
        .then(response => {
            if (!response.ok) throw new Error('Failed to delete user');
            fetchUsers(); // Refresh the user list
        })
        .catch(error => console.error('Error deleting user:', error));
}
