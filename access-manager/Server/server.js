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
const logFilePath = 'access-log.csv'; // Path for the access log CSV file


// Function to append log entry to the access log CSV
function appendLogEntry(uid, accessDecision, accessLevel)
{
    const timestamp = new Date().toISOString();
    // Use "accessDecision" as the property name to match the CSV column name
    const logEntry = [{ uid, accessDecision: accessDecision ? "Granted" : "Denied", accessLevel, timestamp }];

    // Check if log file exists to determine if headers are needed
    const includeHeaders = !fs.existsSync(logFilePath);

    const csvString = stringify(logEntry,
    {
        header: includeHeaders,
        columns: ['uid', 'accessDecision', 'accessLevel', 'timestamp']
    });
    fs.appendFileSync(logFilePath, csvString);
}

app.post('/add-access', (req, res) =>
{
    const { uid, accessLevel, expiresAt } = req.body; // Use uid directly as hex
    console.log(`Received access right addition request for UID (hex): ${uid}`);

    stringUid = uid.toString(16);

    let records = [];

    if (fs.existsSync(csvFilePath))
    {
        const fileContent = fs.readFileSync(csvFilePath);
        records = parse(fileContent, { columns: true, skip_empty_lines: true });
    }

    records.push({ uid, accessLevel, expiresAt }); // Add new record using hex UID

    const csvString = stringify(records, { header: true });
    fs.writeFileSync(csvFilePath, csvString);

    res.json({ message: "Access right added successfully." });
});

app.post('/check-access', (req, res) =>
{
    const { uid } = req.body; // Assume UID is sent in decimal from the MCU
    // Convert decimal UID to hexadecimal for comparison
    let hexUid = parseInt(uid, 10).toString(16);

    console.log(`Received access check request for UID (converted to hex): ${hexUid}`);

    let accessGranted = false;
    let accessLevel = 'none';
    let message = "Access Denied";

    try
    {
        if (fs.existsSync(csvFilePath))
        {
            const fileContent = fs.readFileSync(csvFilePath);
            const records = parse(fileContent, { columns: true, skip_empty_lines: true });

            // Use the converted hexUid for comparison, ensuring it's compared in lowercase to match case-insensitivity
            const record = records.find(record => record.uid.toLowerCase() === hexUid.toLowerCase() &&
                                                   (record.expiresAt === "never" || new Date(record.expiresAt) > new Date()));
            if (record)
            {
                accessGranted = true;
                accessLevel = record.accessLevel;
                message = "Access Granted";
            }
        }
    }
    catch (error)
    {
        console.error(`Error checking access for UID ${hexUid}:`, error);
        message = "Error processing request";
    }

    // Log the access check, using the UID in hexadecimal format
    appendLogEntry(hexUid, accessGranted ? "Granted" : "Denied", accessLevel);

    res.json({ accessGranted, accessLevel, message });
});




app.listen(port, () =>
{
    console.log(`Server running at http://localhost:${port}`);
});
