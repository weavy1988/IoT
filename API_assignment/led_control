#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> // Change to WiFiClientSecure
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "NETGEAR54";         
const char* password = "largeskates598"; 

const char* statusFileUrl = "https://jamesconnects.com/ledstatus.txt"; // URL to your ledstatus.txt
const int gpioPin = 16; // GPIO pin to control

WiFiClientSecure client; // Change to WiFiClientSecure

void checkLEDStatus(); // Function declaration

void setup() {
    Serial.begin(9600);
    pinMode(gpioPin, OUTPUT); // Set GPIO 16 as output

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to Wi-Fi");

    // Initial check
    checkLEDStatus();
}

void loop() {
    static unsigned long lastCheck = 0;
    unsigned long currentMillis = millis();

    // Check the status every 2 minutes (120000 ms)
    if (currentMillis - lastCheck >= 120000) {
        lastCheck = currentMillis;
        checkLEDStatus();
    }
}

void checkLEDStatus() {
   client.setInsecure(); // Add this line to ignore SSL certificate validation

    if (client.connect("jamesconnects.com", 443)) { // Change to port 443 for HTTPS
        Serial.println("Requesting LED status...");
        client.print(String("GET ") + statusFileUrl + " HTTP/1.1\r\n" +
                     "Host: jamesconnects.com\r\n" +
                     "Connection: close\r\n\r\n");
                     
        // Wait for the response
        while (client.connected() || client.available()) {
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
