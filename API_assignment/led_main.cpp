#include <ESP8266WiFi.h> //enables esp8266-specific wifi fucntionality
#include <WiFiClientSecure.h> // Change to WiFiClientSecure
#include <NTPClient.h> //network transfer protocol
#include <WiFiUdp.h> //header file to enable udp over wifi

const char* ssid = "NETGEAR54";         
const char* password = "largeskates598"; 

const char* statusFileUrl = "https://jamesconnects.com/ledstatus.txt"; // URL to ledstatus.txt
const int gpioPin = 16; // GPIO pin to control LED

WiFiClientSecure client; // creates an instance of the wificlientsecure class, oject is "client"

void checkLEDStatus(); // Function declaration

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
}

void loop() {
    static unsigned long lastCheck = 0; //32-bit unsigned integer, retains its value between function calls. initializes to 0
    unsigned long currentMillis = millis(); //millis is also included in arduino library. tells #of ms since running the program 

    // Check the status every 2 minutes (120000 ms)
    if (currentMillis - lastCheck >= 120000) {
        lastCheck = currentMillis;
        checkLEDStatus();
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
