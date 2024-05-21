#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include <RH_RF95.h>
#include <WiFi.h> // Include the WiFi library

// Define LoRa settings
#define BAND    915.0 // Frequency in MHz

const char* ssid = "TelstraA156F1_EXT";     // Your WiFi SSID
const char* password = "r4yrsnrb54"; // Your WiFi password

RH_RF95 rf95;

void setup() {
  Serial.begin(9600);
  
  // Initialize WiFi connection
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) { // Try for a limited number of attempts
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println("");
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection failed. Please check your credentials and try again.");
    Serial.println(WiFi.status());
    while (1); // Halt if WiFi connection failed
  } else {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  
  if (!rf95.init()) {
    Serial.println("Initialization of LoRa receiver failed");
    while (1); // Halt if initialization failed
  }
  
  rf95.setFrequency(BAND);   
  rf95.setTxPower(23, false);
  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(12);
}

void loop() {
  // Check for incoming messages
  if (rf95.available()) {
    uint8_t buf[100];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len)) {
      buf[len] = '\0'; // Ensure null-termination
      
      // Extract sender ID and message
      char senderID = (char)buf[0];
      char* message = (char*)(buf + 2); // Skip the sender ID and colon
      
      // Print received message
      Serial.print("Received from Sender ");
      Serial.print(senderID);
      Serial.print(": ");
      Serial.println(message);
    } else {
      Serial.println("Receive failed");
    }
  }

  delay(100); // Short delay to avoid excessive CPU usage
}

