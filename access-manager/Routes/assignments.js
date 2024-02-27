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
        const users = readCsvFromFile(usersFilePath);
        const accessPoints = readCsvFromFile(accessPointsFilePath);

        // Corrected to match the 'name' and 'familyName' columns in your users.csv
        const user = users.find(u => `${u.name} ${u.familyName}`.trim() === userName.trim());

        // Assuming access point names are case-sensitive. Adjust the logic if necessary.
        const accessPoint = accessPoints.find(ap => ap.name.trim() === accessPointName.trim());

        if (!user || !accessPoint) {
            return res.status(400).json({ message: 'User or access point not found' });
        }

        let assignments = readCsvFromFile(assignmentsFilePath);
        assignments.push({
            cardUID: user.cardUID,
            deviceID: accessPoint.deviceID,
            expiry: expiry || "never",
            isTimeLimited: isTimeLimited ? 'true' : 'false' // Ensuring the value is a string
        });
        writeCsvToFile(assignmentsFilePath, assignments);

        res.json({ message: 'Access assignment created successfully' });
    } catch (error) {
        console.error("Failed to create access assignment:", error);
        res.status(500).json({ message: 'Failed to create access assignment' });
    }
});



router.delete('/delete/:assignmentId', (req, res) => {
    const assignmentId = req.params.assignmentId;

    try {
        let assignments = readCsvFromFile(assignmentsFilePath);
        const updatedAssignments = assignments.filter(assignment => assignment.id !== assignmentId);
        writeCsvToFile(assignmentsFilePath, updatedAssignments);
        res.json({ message: 'Access assignment deleted successfully' });
    } catch (error) {
        console.error("Failed to delete access assignment:", error);
        res.status(500).json({ message: 'Failed to delete access assignment' });
    }
});


module.exports = router;
