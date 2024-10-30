#include <ESP8266WiFi.h> //enables esp8266-specific wifi functionality
#include <WiFiClientSecure.h> // Change to WiFiClientSecure
#include <NTPClient.h> //network transfer protocol
#include <WiFiUdp.h> //header file to enable udp over wifi

const char* ssid = "iPhone (4)"; // SSID of the Wi-Fi network
const char* password = "p27v4kxs9abqn"; // Password for the Wi-Fi network

const char* statusFileUrl = "/ledstatus.txt"; // URL to ledstatus.txt (changed to relative URL)
const int gpioPin = 16; // GPIO pin to control regular LED
const int gpioRed = 0;       // GPIO 0 for Red LED
const int gpioBlue = 4; 

WiFiClientSecure client; // creates an instance of the wificlientsecure class, object is "client"

void checkLEDStatus(); // Function declaration
void checkLEDSliders();

void setup() { //this block of code initializes wifi connection 
    Serial.begin(9600); //serial is an object of the hardwareSerial class, included in arduino library. this sets baud rate 
    //to 9600 which allows esp8266 to communicate with my laptop
    pinMode(gpioPin, OUTPUT); // Set GPIO 16 as output. pinMode is a pre-defined function within arduino library

    WiFi.begin(ssid, password); //wifi.begin is a function declared in the esp8266wifi header file.
    while (WiFi.status() != WL_CONNECTED) { //while not connected to wifi, display ".........."
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to Wi-Fi");

    // Initial check
    checkLEDStatus();
    checkLEDSliders(); // Added to check slider values on startup
}

void loop() {
    static unsigned long lastCheck = 0; //32-bit unsigned integer, retains its value between function calls. initializes to 0
    unsigned long currentMillis = millis(); //millis is also included in arduino library. tells #of ms since running the program 

    // Check the status every 2 minutes (30000 ms)
    if (currentMillis - lastCheck >= 30000) { // Adjusted to 30 seconds
        lastCheck = currentMillis;
        checkLEDStatus();
        checkLEDSliders(); // Check sliders every 30 SEC
    }
}

void checkLEDStatus() {
    client.setInsecure(); // ignores SSL certificate validation

    if (client.connect("jamesconnects.com", 443)) { // domain name
        Serial.println("Requesting LED status..."); //print this to serial monitor
        client.print(String("GET ") + statusFileUrl + " HTTP/1.1\r\n" + //client.print is what sends a string over the internet to the server
                     "Host: jamesconnects.com\r\n" +                 //http version 1.1
                     "Connection: close\r\n\r\n"); //end of http header
                     
        // Wait for the response
        while (client.connected() || client.available()) { //while connected to the server and while ledstatus.txt has data to be read
            if (client.available()) {
                String line = client.readStringUntil('\n');
                Serial.println(line); // Print the response line by line
                
                // Look for the LED Status line
                if (line.startsWith("LED Status:")) {
                    String ledStatus = line.substring(12); // Get the status after "LED Status: "
                    if (ledStatus.equals("on")) {
                        digitalWrite(gpioPin, HIGH); // Set GPIO 16 high
                        Serial.println("LED is ON - Setting GPIO HIGH");
                    } else {
                        digitalWrite(gpioPin, LOW); // Set GPIO 16 low
                        Serial.println("LED is OFF - Setting GPIO LOW");
                    }
                }
            }
        }
        client.stop(); // Disconnect
    } else {
        Serial.println("Failed to connect to the server");
    }
}

void checkLEDSliders() {
    client.setInsecure(); // Ignore SSL certificate validation
    if (client.connect("jamesconnects.com", 443)) { // Connect to server on port 443 for HTTPS
        Serial.println("Requesting slider values..."); // Print status to serial monitor
        client.print(String("GET ") + statusFileUrl + " HTTP/1.1\r\n" +
                     "Host: jamesconnects.com\r\n" +
                     "Connection: close\r\n\r\n"); // Send HTTP GET request

        int redValue = 0;   // Default to 0 if slider values not found
        int blueValue = 0;

        while (client.connected() || client.available()) { // Wait for server response
            if (client.available()) {
                String line = client.readStringUntil('\n'); // Read line by line
                // Check for slider values in the response
                if (line.startsWith("Slider 1 Value:")) {
                    redValue = line.substring(16).toInt();   // Parse Red LED slider value after "Slider 1 RED:"
                } else if (line.startsWith("Slider 2 Value:")) {
                    blueValue = line.substring(16).toInt();  // Parse Blue LED slider value after "Slider 2 BLUE:"
                }
            }
        }

        // Apply parsed values to Red and Blue LEDs using PWM
        analogWrite(gpioRed, redValue);      // Set PWM for Red LED
        analogWrite(gpioBlue, blueValue);    // Set PWM for Blue LED

        Serial.print("Red LED set to PWM: ");
        Serial.println(redValue);
        Serial.print("Blue LED set to PWM: ");
        Serial.println(blueValue);

        client.stop(); // Disconnect from server
    } else {
        Serial.println("Failed to connect to the server for slider values");
    }
}
