// 
//    Master Of Cyber Security
//    COS60011-Technology Design Porject
//    Created by: Ethan PP Cutting - 100942775 - 19/05/2023
//	  modified last: Ethan PP Cutting - 27/05/2023
//    Group Members Part
//    Sachin: Send ID
//    Karan: Hard Code password
//    Sabdar: Messages are to be encrypted using a stream cipher
#include <SPI.h>
#include <RH_RF95.h>
#include <AES.h> // Include the AES library

// Define LoRa settings
#define BAND    915.0 // Frequency in MHz
#define SENDER_ID 'A' // Sender ID for Relay

// Define the key for encryption
const char* encryptionKey = "2q3rf!rT$#56yH8jPkL9@1aB"; 

RH_RF95 rf95;
AES aes;

// Declaration of the sendMessage function
void sendMessage(byte* message, size_t len);

void setup() {
  Serial.begin(9600);
  
  if (!rf95.init()) {
    Serial.println("Initialization of LoRa transmitter failed");
    while (1); // Halt if initialization failed
  }
  
  rf95.setFrequency(BAND);   
  rf95.setTxPower(23, false);
  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(12);
  
  // Set the encryption key
  aes.set_key((byte*)encryptionKey, 16); // AES-128 encryption key size is 16 bytes
}

void loop() {
  // Hardcoded password (It defines a hardcoded password ("hi") and an input buffers and It waits for the user to input a password via the serial monitor, It reads the input from the serial monitor and compares it with the hardcoded password.)
  char password[] = "hi";

  // Input buffer
  char input[100];
  memset(input, 0, sizeof(input)); // Initialise buffer to all zeroes

  Serial.print("Enter password: ");
  while (Serial.available() == 0); // Wait for input
  Serial.readBytesUntil('\n', input, sizeof(input) - 1); // Read input from serial monitor
  input[strlen(input)] = '\0'; // Ensure null-termination

  // Debugging output
  Serial.println("Entered password: " + String(input));

  // Check if the entered password matches the hardcoded one
  if (strcmp(input, password) == 0) {
    Serial.println("Access granted!");
    
    // Prepare the message with sender's identity and content
    String message = "Hello from Relay";

    // Encrypt the message
    byte encrypted[16]; // AES-128 encryption produces 16-byte output
    aes.encrypt((byte*)message.c_str(), encrypted);

    // Send the encrypted message over LoRa
    sendMessage(encrypted, 16);
    
    // Add your code for granting access here
  } else {
    Serial.println("Access denied!");
    
    // Prepare the message with sender's identity (If the password doesn't match, it denies access by preparing a denial message, encrypting it, and sending it over LoRa.)
    String message = "Denied";

    // Encrypt the message 
    byte encrypted[16]; // AES-128 encryption produces 16-byte output
    aes.encrypt((byte*)message.c_str(), encrypted);

    // Send the encrypted message over LoRa ()
    sendMessage(encrypted, 16);
    
  }

  delay(1000); // It adds a delay of 1000 milliseconds for stability before repeating the loop.

}

// Definition of the sendMessage function  (If the password matches, it grants access by preparing a message, encrypting it using AES, and sending it over LoRa using the sendMessage() function)
void sendMessage(byte* message, size_t len) {
  // Print the sender ID and message being sent
  Serial.print("Sending from Sender ");
  Serial.print(SENDER_ID);
  Serial.print(": ");
  for (int i = 0; i < len; i++) {
    Serial.print(message[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  // Send message along with sender's identity
  rf95.send(message, len);
  rf95.waitPacketSent();
  Serial.println("Message sent.");
}
