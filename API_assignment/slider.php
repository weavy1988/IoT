<?php
// Check if the request is POST (form submission)
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Retrieve slider values from POST request
    $slider1 = isset($_POST['slider1']) ? $_POST['slider1'] : 0;
    $slider2 = isset($_POST['slider2']) ? $_POST['slider2'] : 0;

    // Define the path to the status file
    $filePath = 'ledstatus.txt';

    // Read the current LED status to preserve it in the update
    $currentContent = file_get_contents($filePath);
    preg_match('/LED Status: (on|off)/', $currentContent, $matches);
    $ledStatus = isset($matches[1]) ? $matches[1] : 'off';

    // Prepare the content to write to the file
    $content = "LED Status: " . $ledStatus . "\n";
    $content .= "Slider 1 Value: " . $slider1 . "\n";
    $content .= "Slider 2 Value: " . $slider2 . "\n";

    // Write to the file
    $result = file_put_contents($filePath, $content);

    // Check if the write operation was successful
    if ($result === false) {
        echo "Failed to update slider values.";
    } else {
        echo "Slider values updated successfully.";
    }
} else {
    // Display the HTML form if the request is not POST
    ?>
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Slider Control</title>
        <style>
            h1 { color: blue; }
        </style>
    </head>
    <body>
        <h1>Slider Control</h1>

        <h4>Adjust the Sliders and Submit the Values</h4>
        <form method="post" action="slider.php">
            <label for="slider1">Slider 1 (0-255): </label>
            <input type="range" id="slider1" name="slider1" min="0" max="255" value="0" oninput="this.nextElementSibling.value = this.value">
            <output>0</output>
            <p></p>

            <label for="slider2">Slider 2 (0-255): </label>
            <input type="range" id="slider2" name="slider2" min="0" max="255" value="0" oninput="this.nextElementSibling.value = this.value">
            <output>0</output>
            <p></p>

            <!-- Submit button for sliders -->
            <input type="submit" value="Submit Slider Values">
        </form>
    </body>
    </html>
    <?php
}
?>
