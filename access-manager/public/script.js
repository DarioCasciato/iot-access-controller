function toggleExpiryInput()
{
    const checkBox = document.getElementById("set-expiry");
    const expiryInput = document.getElementById("expires-at");
    expiryInput.disabled = !checkBox.checked;
    if (!checkBox.checked)
    {
        expiryInput.value = '';
    }
}

document.getElementById('access-form').addEventListener('submit', function(e)
{
    e.preventDefault();
    const uidInput = document.getElementById('uid');
    const hexUid = uidInput.value.trim();

    if (!/^[0-9a-fA-F]+$/.test(hexUid))
    {
        console.error('Invalid UID: UID must be a hexadecimal string.');
        alert('Invalid UID: UID must be a hexadecimal string.');
        uidInput.focus();
        return;
    }

    const accessLevel = document.getElementById('access-level').value;
    const expiresAt = document.getElementById('set-expiry').checked ? document.getElementById('expires-at').value : "never";

    fetch('/add-access',
    {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ uid: hexUid, accessLevel, expiresAt })
    })
    .then(response =>
    {
        if (!response.ok) throw new Error('Network response was not ok');
        return response.json();
    })
    .then(data =>
    {
        console.log('Success:', data);
        alert('Access right added successfully.');
        uidInput.value = '';
        document.getElementById('expires-at').value = '';
        document.getElementById('set-expiry').checked = false;
    })
    .catch(error =>
    {
        console.error('Error:', error);
        alert('Error adding access right.');
    });
});
