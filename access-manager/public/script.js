function toggleExpiryInput() {
    const checkBox = document.getElementById("set-expiry");
    const expiryInput = document.getElementById("expires-at");
    expiryInput.disabled = !checkBox.checked;
    if (!checkBox.checked) {
        expiryInput.value = ''; // Clear value if disabled
    }
}

document.getElementById('access-form').addEventListener('submit', function(e) {
    e.preventDefault();
    const uidInput = document.getElementById('uid');
    const hexUid = uidInput.value;
    // Convert hex UID to decimal
    const decimalUid = parseInt(hexUid, 16).toString();

    const accessLevel = document.getElementById('access-level').value;
    const expiresAt = document.getElementById('set-expiry').checked ? document.getElementById('expires-at').value : "never";

    // Validate hex input
    if (isNaN(decimalUid)) {
        console.error('Invalid UID: UID must be a hexadecimal string.');
        uidInput.focus(); // Focus on the UID input field for correction
        return; // Stop the form submission
    }

    fetch('/add-access', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ uid: decimalUid, accessLevel, expiresAt }),
    })
    .then(response => response.json())
    .then(data => {
        console.log('Success:', data);
        // Refresh the access rights list or give feedback to the user
    })
    .catch((error) => {
        console.error('Error:', error);
    });
});
