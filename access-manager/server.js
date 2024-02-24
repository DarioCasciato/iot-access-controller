const express = require('express');
const path = require('path');
const fetch = require('node-fetch');

const app = express();
const port = 3000;

app.use(express.json());

// Import routes
const accessPointsRoutes = require('./Routes/accessPoints');
const assignmentsRoutes = require('./Routes/assignments');
const logsRoutes = require('./Routes/logs');
const usersRoutes = require('./Routes/users');

// Serve static files from the 'public' directory
app.use(express.static('public'));

// Use imported routes
app.use('/api/access-points', accessPointsRoutes);
//app.use('/api/assignments', assignmentsRoutes);
app.use('/api/logs', logsRoutes);
//app.use('/api/users', usersRoutes);

// Serve the Overview page as the landing page
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public/pages/Overview/index.html'));
});

// Serve the Access Assignment page
app.get('/access-assignment', (req, res) => {
    res.sendFile(path.join(__dirname, 'public/pages/AccessAssignment/index.html'));
});

// Serve the Access Point Config page
app.get('/access-point-config', (req, res) => {
    res.sendFile(path.join(__dirname, 'public/pages/AccessPointConfig/index.html'));
});

// Serve the Log Overview page
app.get('/log-overview', (req, res) => {
    res.sendFile(path.join(__dirname, 'public/pages/LogOverview/index.html'));
});

// Serve the User Creation page
app.get('/user-creation', (req, res) => {
    res.sendFile(path.join(__dirname, 'public/pages/UserCreation/index.html'));
});


// Helper function to enforce a timeout
function fetchWithTimeout(url, options, timeout = 450) { // Default timeout of 5000 ms
    return Promise.race([
        fetch(url, options),
        new Promise((_, reject) =>
            setTimeout(() => reject(new Error('Request timed out')), timeout)
        )
    ]);
}

// Modified endpoint with timeout
app.get('/api/device-scan/:ip', async (req, res) => {
    const ip = req.params.ip;
    const url = `http://${ip}/getdeviceid`;

    try {
        // Use the fetchWithTimeout helper with a custom timeout
        const response = await fetchWithTimeout(url, {}, 200);
        if (!response.ok) {
            throw new Error(`Request to ${url} failed with status: ${response.status}`);
        }
        const data = await response.json();
        res.json(data);
    } catch (error) {
        console.error(error);
        res.status(500).json({ message: `Failed to scan device at ${ip}: ${error.message}` });
    }
});


app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
