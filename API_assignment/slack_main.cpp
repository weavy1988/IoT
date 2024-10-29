#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <DHT.h>
#include <WiFiClientSecure.h> 

#define DHTPIN 2        
#define DHTTYPE DHT11   
#define BUTTON_PIN 16   

DHT myDHT(DHTPIN, DHTTYPE);

const char* ssid = "NETGEAR54";         
const char* password = "largeskates598"; 

const char* iftttWebhookUrl = "/trigger/button_pressed/json/with/key/bE10pbsmig1dADG8h_RypE";
const char* iftttHost = "maker.ifttt.com";

void setup() {
    Serial.begin(9600);
    myDHT.begin();
    
    pinMode(BUTTON_PIN, INPUT);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected to Wi-Fi");
}
void sendDataToIFTTT(float temperature, float humidity) {
    WiFiClientSecure client;
    client.setInsecure();

    if (client.connect(iftttHost, 443)) {
        String jsonPayload = String("{\"value1\":\"") + temperature + "\",\"value2\":\"" + humidity + "\"}";
        
        client.print(String("POST ") + iftttWebhookUrl + " HTTP/1.1\r\n" +
                     "Host: " + iftttHost + "\r\n" +
                     "Content-Type: application/json\r\n" +
                     "Content-Length: " + jsonPayload.length() + "\r\n" +
                     "Connection: close\r\n\r\n" +
                     jsonPayload);

        Serial.println("Data sent to IFTTT.");


        // Read the response from the server
        String response = "";
        while (client.connected() || client.available()) {
            if (client.available()) {
                response = client.readStringUntil('\n');
                Serial.println(response); // Print response for debugging
            }
        }
    } else {
        Serial.println("Connection to IFTTT failed.");
    }
}

void loop() {
    if (digitalRead(BUTTON_PIN) == HIGH) {
        float temperature = myDHT.readTemperature();
        float humidity = myDHT.readHumidity();

        if (!isnan(temperature) && !isnan(humidity)) {
            sendDataToIFTTT(temperature, humidity);
        } else {
            Serial.println("Failed to read from DHT sensor!");
        }

        delay(2000); // Debounce delay
    }

    delay(100); // Small delay to avoid excessive looping
}
