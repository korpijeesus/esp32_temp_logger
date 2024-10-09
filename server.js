const express = require('express');
const mariadb = require('mariadb');
const path = require('path');
const app = express();

// Middleware to parse incoming request bodies as JSON
app.use(express.json());

// Middleware to serve static files from the "public" directory
app.use(express.static(path.join(__dirname, 'public')));

// Create a MariaDB connection pool
const pool = mariadb.createPool({
  host: 'localhost', // Change to your MariaDB host
  user: '', // Change to your MariaDB user
  password: '', // Change to your MariaDB password
  database: '', // Change to your MariaDB database
  connectionLimit: 5
});

// Route to handle POST requests at "/submit"
app.post('/submit', async (req, res) => {
  console.log('Received data:', req.body); // Log the incoming data
  const { temperature } = req.body; // Assuming the data is sent as { "data": "some value" }

  try {
    const conn = await pool.getConnection();
    
    // Example query to insert data
    const query = 'INSERT INTO temperature (temperature) VALUES (?)';
    await conn.query(query, [temperature]);

    conn.end();
    res.status(200).send('Data inserted successfully');
  } catch (err) {
    console.error(err);
    res.status(500).send('Error inserting data');
  }
});

// Route to fetch temperature data for the graph
app.get('/temperature-data', async (req, res) => {
  try {
    const conn = await pool.getConnection();
    
    const rows = await conn.query("SELECT temperature, time FROM temperature ORDER BY time ASC");
    conn.end();
    
    // Send the fetched data to the client
    res.json(rows);
  } catch (err) {
    console.error(err);
    res.status(500).send('Error fetching data');
  }
});

// Start the server
app.listen(3000, () => {
  console.log('Server is running on http://localhost:3000');
});
