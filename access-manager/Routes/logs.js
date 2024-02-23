const express = require('express');
const fs = require('fs');
const { parse } = require('csv-parse');
const path = require('path');
const router = express.Router();

const logFilePath = path.join(__dirname, '..', 'data', 'access-log.csv');

router.get('/', (req, res) => {
    const records = [];
    fs.createReadStream(logFilePath)
        .pipe(parse({ columns: true }))
        .on('data', (data) => records.push(data))
        .on('end', () => {
            res.json(records);
        })
        .on('error', (error) => {
            console.error("Failed to read and parse log file:", error);
            res.status(500).json({ message: 'Error processing log file' });
        });
});

module.exports = router;
