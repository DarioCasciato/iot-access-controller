const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const { parse } = require('csv-parse/sync');
const { stringify } = require('csv-stringify/sync');
const app = express();
const port = 3000;

app.use(express.static('../public')); // Serve static files
app.use(bodyParser.json()); // for parsing application/json

const csvFilePath = 'access-rights.csv';

app.post('/add-access', (req, res) => {
    let { uid, accessLevel, expiresAt } = req.body;

    // Assuming UID is provided in hexadecimal format, convert it to decimal
    uid = parseInt(uid, 16).toString();

    let records = [];

    // Read existing records
    if (fs.existsSync(csvFilePath)) {
        const fileContent = fs.readFileSync(csvFilePath);
        records = parse(fileContent, { columns: true, skip_empty_lines: true });
    }

    // Add new record with UID as decimal
    records.push({ uid, accessLevel, expiresAt });

    // Write updated records to CSV
    const csvString = stringify(records, { header: true });
    fs.writeFileSync(csvFilePath, csvString);

    res.json({ message: "Access right added successfully with UID in decimal format." });
});

app.post('/check-access', (req, res) => {
    const { uid } = req.body;

    // Convert UID to lowercase for comparison
    const formattedUID = uid.toLowerCase();

    console.log(`Received access check request for UID: ${formattedUID}`);

    let accessGranted = false;
    let accessLevel = 'none';
    let message = "Access Denied";

    try {
        // Read existing records
        if (fs.existsSync(csvFilePath)) {
            const fileContent = fs.readFileSync(csvFilePath);
            const records = parse(fileContent, { columns: true, skip_empty_lines: true });

            // Check if UID has access
            const record = records.find(record => record.uid === formattedUID && (record.expiresAt === "never" || new Date(record.expiresAt) > new Date()));
            if (record) {
                accessGranted = true;
                accessLevel = record.accessLevel;
                message = "Access Granted";
            }
        }

        console.log(`Access check for UID ${formattedUID}: ${message}`);
    } catch (error) {
        console.error(`Error checking access for UID ${formattedUID}:`, error);
        message = "Error processing request";
    }

    // Respond with access status
    res.json({ accessGranted, accessLevel, message });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
