<?php

// Database connection credentials
$servername = "localhost";
$username = "u204282949_db_JamesWeaver"; // Database username
$password = "zulu6!ARclite51!"; // Database password
$dbname = "u204282949_JamesWeaver"; // Database name

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Check if data is being inserted
if (isset($_GET['insert']) && $_GET['insert'] === 'true') {
    $node_name = $_GET['node_name'];
    $time_received = $_GET['time_received']; // UNIX timestamp
    $temperature = $_GET['temperature'];
    $humidity = $_GET['humidity'];

    // Convert UNIX timestamp to datetime format
    $formatted_time_received = date('Y-m-d H:i:s', $time_received);

    // Check if the node exists in the sensor_register table
    $checkNodeQuery = "SELECT COUNT(*) as count FROM sensor_register WHERE node_name = '$node_name'";
    $result = $conn->query($checkNodeQuery);
    $row = $result->fetch_assoc();

    if ($row['count'] > 0) {
        // Node exists, proceed with insertion
        $insertQuery = "INSERT INTO sensor_data (node_name, time_received, temperature, humidity) 
                        VALUES ('$node_name', '$formatted_time_received', $temperature, $humidity)";

        if ($conn->query($insertQuery) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $insertQuery . "<br>" . $conn->error;
        }
    } else {
        // Node not recognized
        echo "Node not recognized. Request denied.";
    }
}

// Fetch sensor data from data.php
function fetchSensorData($url) {
    $jsonData = file_get_contents($url);
    
    if ($jsonData === false) {
        return null;
    }

    $dataArray = json_decode($jsonData, true);

    if ($dataArray === null) {
        return null;
    }

    return $dataArray;
}

// URL to fetch the data from data.php
$dataUrl = 'https://jamesconnects.com/Chartjs/data.php';
$dataArray = fetchSensorData($dataUrl);

if ($dataArray !== null) {
    // Display sensor_data
    if (isset($dataArray['sensor_data'])) {
        echo '<h2>Sensor Data</h2>';
        echo '<table>';
        echo '<tr><th>Node Name</th><th>Time Received</th><th>Temperature</th><th>Humidity</th></tr>';

        foreach ($dataArray['sensor_data'] as $row) {
            echo '<tr>';
            echo '<td>' . htmlspecialchars($row['node_name']) . '</td>';
            echo '<td>' . htmlspecialchars($row['time_received']) . '</td>';
            echo '<td>' . htmlspecialchars($row['temperature']) . '</td>';
            echo '<td>' . htmlspecialchars($row['humidity']) . '</td>';
            echo '</tr>';
        }

        echo '</table>';
    } else {
        echo 'No sensor data available.';
    }

    // Display sensor_register
    if (isset($dataArray['sensor_register'])) {
        echo '<h2>Sensor Register</h2>';
        echo '<table>';
        echo '<tr><th>Node Name</th><th>Manufacturer</th><th>Longitude</th><th>Latitude</th></tr>';

        foreach ($dataArray['sensor_register'] as $row) {
            echo '<tr>';
            echo '<td>' . htmlspecialchars($row['node_name']) . '</td>';
            echo '<td>' . htmlspecialchars($row['manufacturer']) . '</td>';
            echo '<td>' . htmlspecialchars($row['longitude']) . '</td>';
            echo '<td>' . htmlspecialchars($row['latitude']) . '</td>';
            echo '</tr>';
        }

        echo '</table>';
    }
} else {
    echo 'Failed to retrieve data from the server.';
}

// HTML and CSS styles for tables
echo '<style>
    table {
        width: 50%;
        border-collapse: collapse;
        margin: 20px 0;
    }
    th, td {
        padding: 5px;
        text-align: left;
        border: 1px solid #ddd;
    }
    th {
        background-color: #4CAF50;
        color: white;
    }
    tr:nth-child(even) {
        background-color: #f2f2f2;
    }
    tr:hover {
        background-color: #ddd;
    }
</style>';

// Calculate average temperature and humidity for node_1
$avgQuery = "SELECT AVG(temperature) AS avg_temp, AVG(humidity) AS avg_hum FROM sensor_data WHERE node_name = 'node_1'";
$avgResult = $conn->query($avgQuery);

// Display average temperature and humidity
if ($avgResult && $avgResult->num_rows > 0) {
    $avgRow = $avgResult->fetch_assoc();
    $avgTemperature = round($avgRow['avg_temp'], 2); // Rounding to 2 decimal places
    $avgHumidity = round($avgRow['avg_hum'], 2);     // Rounding to 2 decimal places

    echo "<h2>Average Temperature and Humidity for Node 1</h2>";
    echo "<p>Average Temperature: " . htmlspecialchars($avgTemperature) . " °C</p>";
    echo "<p>Average Humidity: " . htmlspecialchars($avgHumidity) . " %</p>";
} else {
    echo "<p>No data found for Node 1 to calculate averages.</p>";
}

?>

<!-- Bar Chart Section -->
<h2>Temperature Chart for Node 1</h2>
<?php
// Create a new PDO connection for the chart data
try {
    // Create a PDO database connection
    $db = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION); // Enable exceptions for error handling

    // SQL query to retrieve data for node_1
    $query = "SELECT time_received, temperature FROM sensor_data WHERE node_name = 'node_1'";

    // Prepare and execute the query
    $stmt = $db->prepare($query);
    $stmt->execute();

    // Fetch data into an array
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);

    // Prepare data for Google Charts
    $dataArray = [['Time Received', 'Temperature']];
    foreach ($data as $row) {
        $dataArray[] = [$row['time_received'], (float)$row['temperature']];
    }

    // Convert data to JSON
    $dataJson = json_encode($dataArray);

    // Close the database connection
    $db = null; // Close the PDO connection

} catch (PDOException $e) {
    die("Error: " . $e->getMessage());
}
?>

<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
    google.charts.load('current', {'packages':['corechart']});
    google.charts.setOnLoadCallback(drawChart);

    function drawChart() {
        var data = google.visualization.arrayToDataTable(<?php echo $dataJson; ?>);

        var options = {
            title: 'Temperature vs. Time for Node 1',
            vAxis: { title: 'Temperature (°C)' },
            hAxis: { title: 'Time', slantedText: true },
            series: {0: { pointSize: 6 }},
            legend: { position: 'none' }
        };

        var chart = new google.visualization.ColumnChart(document.getElementById('chart_div'));
        chart.draw(data, options);
    }
</script>

<div id="chart_div" style="width: 800px; height: 400px;"></div>
