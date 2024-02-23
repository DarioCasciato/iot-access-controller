const express = require('express');
const path = require('path');

const app = express();
const port = 3000;


// Import routes
const accessPointsRoutes = require('./Routes/accessPoints');
const assignmentsRoutes = require('./Routes/assignments');
const logsRoutes = require('./Routes/logs');
const usersRoutes = require('./Routes/users');

// Serve static files from the 'public' directory
app.use(express.static('public'));

// Use imported routes
//app.use('/api/access-points', accessPointsRoutes);
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

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
