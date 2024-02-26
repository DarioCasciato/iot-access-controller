const express = require('express');
const fs = require('fs');
const { parse } = require('csv-parse/sync');
const { stringify } = require('csv-stringify/sync');
const path = require('path');
const router = express.Router();

const usersFilePath = path.join(__dirname, '..', 'data', 'users.csv');

// Helper function to read users from CSV
function readUsersFromFile(filePath) {
    const csvData = fs.readFileSync(filePath, 'utf8');
    return parse(csvData, { columns: true });
}

// Helper function to write users to CSV
function writeUsersToFile(filePath, records) {
    const updatedCsvData = stringify(records, { header: true });
    fs.writeFileSync(filePath, updatedCsvData, 'utf8');
}

// Route to get all users
router.get('/', (req, res) => {
    try {
        const records = readUsersFromFile(usersFilePath);
        res.json(records);
    } catch (error) {
        console.error("Failed to read users file:", error);
        res.status(500).json({ message: 'Error reading users file' });
    }
});

// Route to create a new user
router.post('/', (req, res) => {
    const { name, familyName, cardUID } = req.body;

    try {
        let records = readUsersFromFile(usersFilePath);

        // Check if cardUID already exists
        if (records.some(record => record.cardUID === cardUID)) {
            return res.status(400).json({ message: 'User with this Card UID already exists.' });
        }

        records.push({ name, familyName, cardUID });
        writeUsersToFile(usersFilePath, records);
        res.json({ message: 'User created successfully' });
    } catch (error) {
        console.error("Failed to create user:", error);
        res.status(500).json({ message: 'Failed to create user' });
    }
});


// Route to delete a user
router.delete('/:cardUID', (req, res) => {
    const { cardUID } = req.params;

    try {
        let records = readUsersFromFile(usersFilePath);
        const updatedRecords = records.filter(record => record.cardUID !== cardUID);
        writeUsersToFile(usersFilePath, updatedRecords);
        res.json({ message: 'User deleted successfully' });
    } catch (error) {
        console.error("Failed to delete user:", error);
        res.status(500).json({ message: 'Failed to delete user' });
    }
});

module.exports = router;
