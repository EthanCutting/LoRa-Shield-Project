#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <SPI.h>
#include <RH_RF95.h>

// Define LoRa settings
#define BAND    915.0 // Frequency in MHz
#define SENDER_ID 'A' // Sender ID for Relay

const char* ssid = "TelstraA156F1_EXT";     // Your WiFi SSID
const char* password = "r4yrsnrb54"; // Your WiFi password

RH_RF95 rf95;

void setup() {
  Serial.begin(9600);
  
  // Initialize WiFi connection
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  // Wait for WiFi connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  // Print WiFi connection status
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connection failed. Please check your credentials and try again.");
    while (1); // Halt if WiFi connection failed
  }
  
  // Initialize LoRa module
  if (!rf95.init()) {
    Serial.println("Initialization of LoRa module failed.");
    while (1); // Halt if initialization failed
  }
  
  // Configure LoRa settings
  rf95.setFrequency(BAND);   
  rf95.setTxPower(23, false);
  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(12);
}


void loop() {
  // Hardcoded password
  char password[] = "hi";

  // Debugging output
  Serial.println("Entered password: " + String(password));

  // Check if the entered password matches the hardcoded one
  if (strcmp(password, "hi") == 0) {
    Serial.println("Access granted!");
    
    // Prepare the message with sender's identity and content
    String message = "Sending from Sender ";
    message += SENDER_ID;
    message += ": Hello from Relay";

    // Print the message
    Serial.println(message);

    // Send the message over LoRa
    sendMessage(message);
    
    // Add your code for granting access here
  } else {
    Serial.println("Access denied!");
    
    // Prepare the message with sender's identity
    String message = "Access denied by Relay";

    // Send the message over LoRa
    sendMessage(message);
    
    // Add your code for denying access here
  }

  delay(1000); // Delay for stability
}

void sendMessage(String message) {
  // Print the sender ID and message being sent
  Serial.print("Sending from Sender ");
  Serial.print(SENDER_ID);
  Serial.print(": ");
  Serial.println(message);
  
  // Send message along with sender's identity
  char buf[100];
  sprintf(buf, "%c: %s", SENDER_ID, message.c_str());
  
  rf95.send((uint8_t*)buf, strlen(buf) + 1); // Include null-termination
  rf95.waitPacketSent();
  Serial.println("Message sent.");
}
