document.addEventListener('DOMContentLoaded', () => {
    fetchAssignments();
    fetchUsers(); // Fetch and populate users
    fetchAccessPoints(); // Fetch and populate access points
    setupEventListeners();
    setupCheckboxListener();
});

function setupCheckboxListener() {
    var checkbox = document.getElementById('enableExpiryDate');
    var expiryDateInput = document.getElementById('expiryDate');

    checkbox.addEventListener('change', function() {
        if (checkbox.checked) {
            expiryDateInput.disabled = false; // Enable the expiry date input
        } else {
            expiryDateInput.disabled = true; // Disable the expiry date input
        }
    });
}

function setupEventListeners() {
    document.getElementById('assignAccessButton').addEventListener('click', openModal);
    document.getElementById('closeModalButton').addEventListener('click', closeModal);
    document.getElementById('submitAccessButton').addEventListener('click', createAssignment);
}

function openModal() {
    document.getElementById('modalOverlay').style.display = 'block';
    document.getElementById('modalContainer').style.display = 'block';
}

function closeModal() {
    document.getElementById('modalOverlay').style.display = 'none';
    document.getElementById('modalContainer').style.display = 'none';
}


// Fetch and populate the users dropdown
function fetchUsers() {
    fetch('/api/users')
        .then(response => response.json())
        .then(users => {
            const userSelect = document.getElementById('userSelect');
            users.forEach(user => {
                const option = document.createElement('option');
                option.value = user.name; // Use user's name as value
                option.textContent = user.name + " " + user.familyName; // Display full name
                userSelect.appendChild(option);
            });
        })
        .catch(error => console.error('Error fetching users:', error));
}

// Fetch and populate the access points dropdown
function fetchAccessPoints() {
    fetch('/api/access-points')
        .then(response => response.json())
        .then(accessPoints => {
            const accessPointSelect = document.getElementById('accessPointSelect');
            accessPoints.forEach(accessPoint => {
                const option = document.createElement('option');
                option.value = accessPoint.name; // Use access point's name as value
                option.textContent = accessPoint.name; // Display access point name
                accessPointSelect.appendChild(option);
            });
        })
        .catch(error => console.error('Error fetching access points:', error));
}

function fetchAssignments() {
    fetch('/api/assignments')
        .then(response => response.json())
        .then(assignments => {
            populateAssignmentsTable(assignments);
        })
        .catch(error => console.error('Error fetching assignments:', error));
}

function populateAssignmentsTable(assignments) {
    const tableBody = document.getElementById('accessAssignmentsTable').querySelector('tbody');
    tableBody.innerHTML = ''; // Clear existing assignments

    assignments.forEach((assignment, index) => {
        const row = tableBody.insertRow();
        row.insertCell(0).textContent = assignment.userName; // Adjusted to use userName
        row.insertCell(1).textContent = assignment.accessPointName; // Adjusted to use accessPointName
        row.insertCell(2).textContent = assignment.expiry;

        // Actions cell for delete button
        const actionsCell = row.insertCell(3);
        const deleteButton = document.createElement('button');
        deleteButton.textContent = 'Delete';
        deleteButton.dataset.id = assignment.id; // Use data attributes to store the assignment ID
        deleteButton.classList.add('delete-btn'); // Add class for easier selection
        actionsCell.appendChild(deleteButton);
    });

    // Attach the event listener to the table body for event delegation
    tableBody.addEventListener('click', function(event) {
        if (event.target.classList.contains('delete-btn')) {
            const assignmentId = event.target.dataset.id;
            deleteAssignment(assignmentId);
        }
    });
}

function createAssignment() {
    const userSelect = document.getElementById('userSelect');
    const accessPointSelect = document.getElementById('accessPointSelect');
    const userName = userSelect.options[userSelect.selectedIndex].text;
    const accessPointName = accessPointSelect.options[accessPointSelect.selectedIndex].text;
    const enableExpiryDate = document.getElementById('enableExpiryDate').checked;
    const expiryDate = document.getElementById('expiryDate').value;

    const assignmentData = {
        userName,
        accessPointName,
        expiry: enableExpiryDate ? expiryDate : "never",
        isTimeLimited: enableExpiryDate,
    };

    console.log(JSON.stringify(assignmentData));

    fetch('/api/assignments/create', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(assignmentData),
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        console.log('Success:', data);
        closeModal(); // Close the modal on success
        fetchAssignments(); // Refresh the assignments list
    })
    .catch((error) => {
        console.error('Error:', error);
    });
}


function deleteAssignment(assignmentId) {
    fetch(`/api/assignments/delete/${assignmentId}`, {
        method: 'DELETE', // Assuming your API uses the DELETE method for deletion
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        fetchAssignments(); // Refresh the assignments list after deletion
    })
    .catch(error => console.error('Error:', error));
}