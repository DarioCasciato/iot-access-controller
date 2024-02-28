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

function formatDate(dateString) {
    const date = new Date(dateString);
    const day = date.getDate().toString().padStart(2, '0');
    const month = (date.getMonth() + 1).toString().padStart(2, '0'); // JavaScript months are 0-based.
    const year = date.getFullYear();
    const hours = date.getHours().toString().padStart(2, '0');
    const minutes = date.getMinutes().toString().padStart(2, '0');

    return `${day}-${month}-${year} ${hours}:${minutes}`;
}

function populateAssignmentsTable(assignments) {
    const tableBody = document.getElementById('accessAssignmentsTable').querySelector('tbody');
    tableBody.innerHTML = ''; // Clear existing assignments

    assignments.forEach((assignment, index) => {
        const row = tableBody.insertRow();
        row.insertCell(0).textContent = assignment.userName;
        row.insertCell(1).textContent = assignment.accessPointName;
        const formattedExpiry = assignment.expiry !== "never" ? formatDate(assignment.expiry) : "Never";
        row.insertCell(2).textContent = formattedExpiry;

        // Actions cell for delete button
        const actionsCell = row.insertCell(3);
        const deleteButton = document.createElement('button');
        deleteButton.textContent = 'Delete';
        deleteButton.dataset.id = assignment.id;
        deleteButton.classList.add('delete-btn');
        deleteButton.onclick = function() { // Attach the event listener directly
            deleteAssignment(assignment.id);
        };
        actionsCell.appendChild(deleteButton);
    });
}


function createAssignment() {
    const userSelect = document.getElementById('userSelect');
    const accessPointSelect = document.getElementById('accessPointSelect');
    const userName = userSelect.options[userSelect.selectedIndex].text;
    const accessPointName = accessPointSelect.options[accessPointSelect.selectedIndex].text;
    const enableExpiryDate = document.getElementById('enableExpiryDate').checked;
    const expiryDate = document.getElementById('expiryDate').value;

    if (enableExpiryDate && !expiryDate) {
        alert("Please enter a date and time.");
        return; // Exit the function early
    }

    fetch('/api/assignments')
        .then(response => response.json())
        .then(assignments => {
            // Check for duplicate assignment
            const duplicate = assignments.some(assignment => assignment.userName === userName && assignment.accessPointName === accessPointName && assignment.expiry === (expiryDate || "never"));
            if (duplicate) {
                alert("An identical access assignment already exists.");
                return; // Exit the function early if duplicate found
            }

            // Proceed with creating the assignment if no duplicate
            const assignmentData = {
                userName,
                accessPointName,
                expiry: enableExpiryDate ? expiryDate : "never",
                isTimeLimited: enableExpiryDate,
            };

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
                closeModal(); // Close the modal on success
                fetchAssignments(); // Refresh the assignments list
            })
            .catch((error) => {
                console.error('Error:', error);
            });
        })
        .catch(error => console.error('Error fetching assignments:', error));
}


function deleteAssignment(assignmentId) {
    // Use confirm dialog to ask the user for confirmation
    const isConfirmed = confirm('Are you sure you want to delete this assignment?');

    // Proceed with deletion only if the user confirms
    if (isConfirmed) {
        fetch(`/api/assignments/delete/${assignmentId}`, {
            method: 'DELETE',
        })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            fetchAssignments(); // Refresh the assignments list after deletion
        })
        .catch(error => console.error('Error:', error));
    }
    else {
        console.log('Deletion cancelled');
    }
}
