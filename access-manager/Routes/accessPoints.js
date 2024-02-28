const express = require('express');
const fs = require('fs');
const { parse } = require('csv-parse/sync');
const { stringify } = require('csv-stringify/sync');
const path = require('path');
const router = express.Router();

const accessPointsFilePath = path.join(__dirname, '..', 'data', 'access-points.csv');
const assignmentsFilePath = path.join(__dirname, '..', 'data', 'access-assignments.csv');

function readCsvFromFile(filePath) {
    const csvData = fs.readFileSync(filePath, 'utf8');
    return parse(csvData, { columns: true, skip_empty_lines: true });
}

function writeCsvToFile(filePath, records) {
    const updatedCsvData = stringify(records, { header: true });
    fs.writeFileSync(filePath, updatedCsvData, 'utf8');
}

// Route to get all access points, including their IPs
router.get('/', (req, res) => {
    fs.readFile(accessPointsFilePath, (err, fileData) => {
        if (err) {
            console.error("Failed to read access points file:", err);
            return res.status(500).json({ message: 'Error reading access points file' });
        }
        const records = parse(fileData, { columns: true });
        res.json(records);
    });
});

// Route to update an existing access point or add a new one
router.post('/update', (req, res) => {
    const { deviceID, ip, newName, newNotes } = req.body;

    try {
        const csvData = fs.readFileSync(accessPointsFilePath, 'utf8');
        let records = parse(csvData, { columns: true, skip_empty_lines: true });

        const index = records.findIndex(record => record.deviceID === deviceID);
        if (index !== -1) {
            // Existing device, update only provided fields
            const existingRecord = records[index];
            const updatedRecord = {
                ...existingRecord,
                ip: ip ?? existingRecord.ip, // Use existing value if ip is not provided
                name: newName ?? existingRecord.name, // Use existing value if newName is not provided
                notes: newNotes ?? existingRecord.notes // Use existing value if newNotes is not provided
            };
            records[index] = updatedRecord;
        } else {
            // New device, add it
            records.push({ deviceID, ip, name: newName, notes: newNotes });
        }

        // Save the updated records to the CSV
        const updatedCsvData = stringify(records, { header: true });
        fs.writeFileSync(accessPointsFilePath, updatedCsvData, 'utf8');

        res.json({ message: 'Access point updated successfully' });
    } catch (error) {
        console.error('Failed to update access point:', error);
        res.status(500).json({ message: 'Failed to update access point' });
    }
});


// Route to delete an access point
router.post('/delete', (req, res) => {
    const { deviceID } = req.body;

    try {
        let accessPoints = parse(fs.readFileSync(accessPointsFilePath, 'utf8'), { columns: true, skip_empty_lines: true });
        let assignments = readCsvFromFile(assignmentsFilePath);

        // Filter out the access point to be deleted
        const updatedAccessPoints = accessPoints.filter(ap => ap.deviceID !== deviceID);

        // Filter out any assignments related to the deleted access point
        const updatedAssignments = assignments.filter(assignment => assignment.deviceID !== deviceID);

        // Write the updated access points and assignments back to their respective CSV files
        fs.writeFileSync(accessPointsFilePath, stringify(updatedAccessPoints, { header: true }), 'utf8');
        writeCsvToFile(assignmentsFilePath, updatedAssignments);

        res.json({ message: 'Access point and related assignments deleted successfully' });
    } catch (error) {
        console.error('Failed to delete access point and related assignments:', error);
        res.status(500).json({ message: 'Failed to delete access point and related assignments' });
    }
});

module.exports = router;
