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
        // Read the existing CSV file
        const csvData = fs.readFileSync(accessPointsFilePath, 'utf8');
        const records = parse(csvData, { columns: true, skip_empty_lines: true });

        // Update the record with matching deviceID
        const updatedRecords = records.map(record => {
            if (record.deviceID === deviceID) {
                return { ...record, name: newName, notes: newNotes };
            }
            return record;
        });

        // Convert updated records back to CSV string
        const updatedCsvData = stringify(updatedRecords, { header: true });

        // Write the updated CSV data back to the file
        fs.writeFileSync(accessPointsFilePath, updatedCsvData, 'utf8');

        res.json({ message: 'Access point name and description updated successfully' });
    } catch (error) {
        console.error('Failed to update access point:', error);
        res.status(500).json({ message: 'Failed to update access point' });
    }
});

module.exports = router;
