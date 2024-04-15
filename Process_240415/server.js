const express = require('express');
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

const app = express();
const port = new SerialPort('COM8', { baudRate: 9600 }); // Change 'COM8' to your serial port
const parser = port.pipe(new Readline({ delimiter: '\n' }));

let latestData = "255,255,255"; // Default white background

app.use(express.static('public')); // Serve files from "public" directory

parser.on('data', data => {
  console.log('Received data:', data);
  latestData = data.trim(); // Update the latest RGB data
});

app.get('/data', (req, res) => {
  res.send(latestData); // Send the latest RGB data to the client
});

app.listen(3000, () => {
  console.log('Server running on http://localhost:3000');
});