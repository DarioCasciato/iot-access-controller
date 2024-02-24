const express = require('express');
const fs = require('fs');
const { parse } = require('csv-parse/sync'); // Corrected import
const { stringify } = require('csv-stringify/sync');
const path = require('path');
const router = express.Router();

const accessPointsFilePath = path.join(__dirname, '..', 'data', 'access-points.csv');

// Example route to get all access points
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

router.post('/update', (req, res) => {
    const { deviceID, newName, newNotes } = req.body;

    try {
        const csvData = fs.readFileSync(accessPointsFilePath, 'utf8');
        let records = parse(csvData, { columns: true, skip_empty_lines: true });

        // Check if device already exists
        const index = records.findIndex(record => record.deviceID === deviceID);
        if (index === -1) {
            // Add new device if it doesn't exist
            records.push({ deviceID, name: newName, notes: newNotes });
        } else {
            // Update existing device's name and notes
            records[index] = { ...records[index], name: newName, notes: newNotes };
        }

        const updatedCsvData = stringify(records, { header: true });
        fs.writeFileSync(accessPointsFilePath, updatedCsvData, 'utf8');

        res.json({ message: 'Device added or updated successfully' });
    } catch (error) {
        console.error('Failed to add or update device:', error);
        res.status(500).json({ message: 'Failed to add or update device' });
    }
});

module.exports = router;
