#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <DHT.h>
#include <WiFiClientSecure.h> 

#define DHTPIN 2         
#define DHTTYPE DHT11    
DHT myDHT(DHTPIN, DHTTYPE);


const char* ssid = "microUAS";         
const char* password = "masinmicro"; 

const char* server = "jamesconnects.com";
const char* node_name = "node_1"; 

WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 0, 60000); 

void setup() {
    Serial.begin(9600);
    myDHT.begin();

    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("."); 
    }
    Serial.println("Connected to Wi-Fi");

    
    timeClient.begin();
}

void sendDataToServer(float temperature, float humidity) {
    
    timeClient.update();
    
    
    unsigned long epochTime = timeClient.getEpochTime()-7 * 3600; 

    
    String url = String("https://jamesconnects.com/database_access.php?insert=true&node_name=") + node_name + 
                 "&time_received=" + String(epochTime) + 
                 "&temperature=" + String(temperature) + 
                 "&humidity=" + String(humidity);

    WiFiClientSecure client; 
    client.setInsecure(); 

    if (client.connect(server, 443)) { 
        Serial.print("Sending data to server: ");
        Serial.println(url);
        
        
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: close\r\n\r\n");

        
        String response = "";
        while (client.connected() || client.available()) {
            if (client.available()) {
                response = client.readStringUntil('\n');
                Serial.println(response); 
            }
        }
        Serial.println("Data sent successfully.");
    } else {
        Serial.println("Connection to server failed.");
    }
}

void loop() {
    float temperature = myDHT.readTemperature();
    float humidity = myDHT.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
        sendDataToServer(temperature, humidity);
    } else {
        Serial.println("Failed to read from DHT sensor!");
    }
    
    delay(20000); 
}
