const express = require('express');
const fs = require('fs');
const { parse } = require('csv-parse/sync');
const { stringify } = require('csv-stringify/sync');
const path = require('path');
const router = express.Router();

const assignmentsFilePath = path.join(__dirname, '..', 'data', 'access-assignments.csv');
const logsFilePath = path.join(__dirname, '..', 'data', 'access-log.csv');

// Helper function to read CSV files
function readCsvFromFile(filePath) {
    const csvData = fs.readFileSync(filePath, 'utf8');
    return parse(csvData, { columns: true, skip_empty_lines: true });
}

// Helper function to write to CSV files
function writeCsvToFile(filePath, records) {
    const updatedCsvData = stringify(records, { header: true });
    fs.writeFileSync(filePath, updatedCsvData, 'utf8');
}

// Route to handle access requests
router.post('/access-request', (req, res) => {
    const { deviceId, uid } = req.body;
    const assignments = readCsvFromFile(assignmentsFilePath);
    const currentTime = new Date();
    const hexUid = parseInt(uid, 10).toString(16);

    // Check if access right exists and has not expired
    const accessRight = assignments.find(assignment => assignment.deviceID === deviceId && assignment.cardUID === hexUid &&
        (assignment.expiry === "never" || new Date(assignment.expiry) > currentTime));

    const accessDecision = accessRight ? "granted" : "denied";

    // Log the access request
    const logs = readCsvFromFile(logsFilePath);
    const newLogEntry = {
        deviceID: deviceId,
        cardUID: hexUid,
        accessDecision: accessDecision,
        timestamp: currentTime.toISOString()
    };
    logs.push(newLogEntry);
    writeCsvToFile(logsFilePath, logs);

    // Respond with granted or denied
    res.json({ accessDecision: accessDecision });
});

module.exports = router;
