const express = require('express');
const fs = require('fs');
const { parse } = require('csv-parse/sync');
const { stringify } = require('csv-stringify/sync');
const path = require('path');
const router = express.Router();

const accessPointsFilePath = path.join(__dirname, '..', 'data', 'access-points.csv');

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
        if (index === -1) {
            // Add new device if it doesn't exist
            records.push({ deviceID, ip, name: newName, notes: newNotes });
        } else {
            // Update existing device's name, notes, and IP
            records[index] = { ...records[index], ip, name: newName, notes: newNotes };
        }

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
        const csvData = fs.readFileSync(accessPointsFilePath, 'utf8');
        let records = parse(csvData, { columns: true, skip_empty_lines: true });

        const updatedRecords = records.filter(record => record.deviceID !== deviceID);
        const updatedCsvData = stringify(updatedRecords, { header: true });
        fs.writeFileSync(accessPointsFilePath, updatedCsvData, 'utf8');

        res.json({ message: 'Device deleted successfully' });
    } catch (error) {
        console.error('Failed to delete device:', error);
        res.status(500).json({ message: 'Failed to delete device' });
    }
});

module.exports = router;
