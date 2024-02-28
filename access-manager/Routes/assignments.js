const express = require('express');
const fs = require('fs');
const { parse } = require('csv-parse/sync');
const { stringify } = require('csv-stringify/sync');
const path = require('path');
const router = express.Router();

const assignmentsFilePath = path.join(__dirname, '..', 'data', 'access-assignments.csv');
const usersFilePath = path.join(__dirname, '..', 'data', 'users.csv');
const accessPointsFilePath = path.join(__dirname, '..', 'data', 'access-points.csv');

// Read CSV file and return parsed data
function readCsvFromFile(filePath) {
    const csvData = fs.readFileSync(filePath, 'utf8');
    return parse(csvData, { columns: true, skip_empty_lines: true });
}

// Write data to CSV file
function writeCsvToFile(filePath, records) {
    const updatedCsvData = stringify(records, { header: true });
    fs.writeFileSync(filePath, updatedCsvData, 'utf8');
}

// Fetch assignments handler
router.get('/', (req, res) => {
    try {
        let assignments = readCsvFromFile(assignmentsFilePath);
        const users = readCsvFromFile(usersFilePath);
        const accessPoints = readCsvFromFile(accessPointsFilePath);

        // Enrich assignments with user names and access point names
        const enrichedAssignments = assignments.map(assignment => {
            const user = users.find(user => user.cardUID === assignment.cardUID);
            const accessPoint = accessPoints.find(ap => ap.deviceID === assignment.deviceID);

            return {
                ...assignment,
                userName: user ? `${user.name} ${user.familyName}` : 'Unknown User',
                accessPointName: accessPoint ? accessPoint.name : 'Unknown Access Point'
            };
        });

        res.json(enrichedAssignments);
    } catch (error) {
        console.error("Failed to fetch assignments:", error);
        res.status(500).json({ message: 'Failed to fetch assignments' });
    }
});


// Create assignment handler
router.post('/create', (req, res) => {
    const { userName, accessPointName, expiry, isTimeLimited } = req.body;

    try {
        // Read existing data
        const users = readCsvFromFile(usersFilePath);
        const accessPoints = readCsvFromFile(accessPointsFilePath);
        let assignments = readCsvFromFile(assignmentsFilePath);

        // Find user and access point based on provided names
        const user = users.find(u => `${u.name} ${u.familyName}`.trim() === userName.trim());
        const accessPoint = accessPoints.find(ap => ap.name.trim() === accessPointName.trim());

        // Validate user and access point existence
        if (!user) {
            return res.status(400).json({ message: 'User not found' });
        }
        if (!accessPoint) {
            return res.status(400).json({ message: 'Access point not found' });
        }

        // Generate a new ID for the assignment
        let maxId = assignments.reduce((max, assignment) => Math.max(max, parseInt(assignment.id || 0, 10)), 0);
        const newId = maxId + 1;

        // Create new assignment object
        const newAssignment = {
            id: newId.toString(),
            cardUID: user.cardUID,
            deviceID: accessPoint.deviceID,
            expiry: expiry || "never",
            isTimeLimited: isTimeLimited ? 'true' : 'false'
        };

        // Add the new assignment to the list and write back to the CSV
        assignments.push(newAssignment);
        writeCsvToFile(assignmentsFilePath, assignments);

        // Respond with success message
        res.json({ message: 'Access assignment created successfully', id: newId });
    } catch (error) {
        console.error("Failed to create access assignment:", error);
        res.status(500).json({ message: 'Failed to create access assignment', error: error.message });
    }
});


router.delete('/delete/:id', (req, res) => {
    const { id } = req.params; // Get the id from URL parameters

    try {
        let assignments = readCsvFromFile(assignmentsFilePath);
        assignments = assignments.filter(assignment => assignment.id !== id);
        writeCsvToFile(assignmentsFilePath, assignments);

        res.json({ message: 'Access assignment deleted successfully' });
    } catch (error) {
        console.error("Failed to delete access assignment:", error);
        res.status(500).json({ message: 'Failed to delete access assignment' });
    }
});


module.exports = router;
