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

// Fetch sensor data from the sensor_info table
$query = "SELECT timestamp, value FROM sensor_info ORDER BY timestamp ASC";
$result = $conn->query($query);

// Prepare data for Google Charts
$dataArray = [['Timestamp', 'Value']];

if ($result->num_rows > 0) {
    // Fetch data and prepare for chart
    while ($row = $result->fetch_assoc()) {
        // Convert timestamp to a format Google Charts can use (epoch timestamp)
        $timestamp = strtotime($row['timestamp'])*1000; // Google Charts expects timestamps in milliseconds
        $dataArray[] = [$timestamp, (float)$row['value']];
    }
} else {
    echo "No data found.";
}

// Convert data to JSON for use in Google Charts
$dataJson = json_encode($dataArray);

// Close the database connection
$conn->close();
?>

<!-- Line Chart Section -->
<h2>Sensor Data Line Graph</h2>
<?php if (!empty($dataArray)) { ?>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
        google.charts.load('current', {'packages':['corechart', 'line']});
        google.charts.setOnLoadCallback(drawChart);

        function drawChart() {
            var data = google.visualization.arrayToDataTable(<?php echo $dataJson; ?>);

            var options = {
                title: 'Sensor Values Over Time',
                hAxis: {
                    title: 'Time',
                    format: 'HH:mm:ss',  // Time format on the x-axis
                    slantedText: true,
                    slantedTextAngle: 45
                },
                vAxis: {
                    title: 'Sensor Value'
                },
                series: {
                    0: { curveType: 'function' }
                },
                legend: { position: 'none' }
            };

            var chart = new google.visualization.LineChart(document.getElementById('chart_div'));
            chart.draw(data, options);
        }
    </script>

    <div id="chart_div" style="width: 800px; height: 400px;"></div>

<?php } else { ?>
    <p>No data available to display the chart.</p>
<?php } ?>

