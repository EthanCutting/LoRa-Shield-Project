// 
//    Master Of Cyber Security
//    COS60011-Technology Design Porject
//    Created by: Ethan PP Cutting - 100942775 - 21/05/2023
//	  modified last: Ethan PP Cutting - 23/05/2023

#include <SPI.h>
#include <RH_RF95.h>
#include <AES.h>
#include <CRC32.h> // Include the CRC32 library

// Define LoRa settings
#define BAND    915.0 // Frequency in MHz
#define TRANSMISSION_DELAY 2000 // Delay between packet transmissions in milliseconds

RH_RF95 rf95;
AES aes;
CRC32 crc;

// Define the key for encryption
const char* encryptionKey = "2q3rf!rT$#56yH8jPkL9@1aB";
// Define the expected size of the encrypted message
#define ENCRYPTED_MESSAGE_SIZE 32
// 'Serial.begin(9600) will initialises communication with a baud rate of 9600
void setup() {
  Serial.begin(9600);

  Serial.println("Initializing LoRa receiver...");
  if (!rf95.init()) {
    Serial.println("Initialization of LoRa receiver failed");
    while (1); // Halt if initialization failed
  }
  Serial.println("LoRa receiver initialized successfully.");
  
  rf95.setFrequency(BAND);   
  rf95.setTxPower(23, false);
  rf95.setSignalBandwidth(500000);
  rf95.setSpreadingFactor(12);

  // Set the encryption key
  Serial.println("Setting encryption key...");
  aes.set_key((byte*)encryptionKey, 16); // AES-128 encryption key size is 16 bytes
  Serial.println("Encryption key set successfully.");
}

void loop() 
{
  // if (rf95.available()) checks if there is a packet available to receive. If a packet is available, it proceeds to read it.
  if (rf95.available()) {
    Serial.println("Packet available...");
    uint8_t buf[ENCRYPTED_MESSAGE_SIZE]; // Buffer size adjusted
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      Serial.println("Received packet...");
      // Decrypt the received message
      byte decrypted[ENCRYPTED_MESSAGE_SIZE]; // Buffer size adjusted
      // The received packet is decrypted using AES decryption (aes.decrypt(buf, decrypted)).
      aes.decrypt(buf, decrypted);

      // Print decrypted message for debugging
      Serial.print("Decrypted message: ");
      for (int i = 0; i < len; i++) {
        Serial.print((char)decrypted[i]);
      }
      Serial.println();
      Serial.print("Sender ID: A");

      //  The CRC32 checksum of the decrypted message is verified using the verifyChecksum() function. If the checksum verification is successful, the decrypted message is printed.
      if (verifyChecksum(decrypted, len)) {
        Serial.println("Checksum verification successful.");
        // Print decrypted message
        Serial.print("Decrypted message: ");
        for (int i = 0; i < len; i++) {
          Serial.print((char)decrypted[i]);
        }
        Serial.println();
      } else {
        //Serial.println("Checksum verification failed. Data may be corrupted.");
      }
    } else {
      Serial.println("Failed to receive packet");
    }
  }
  delay(TRANSMISSION_DELAY); // Delay between packet transmissions
}


// This function calculates the CRC32 checksum of the message and compares it with the received CRC32 checksum to verify data integrity.
bool verifyChecksum(byte* message, size_t len) {
  // Calculate CRC32 checksum of the message
  crc.reset();
  crc.update(message, len - 4); // Exclude the last 4 bytes which are the checksum itself
  uint32_t calculatedCRC = crc.finalize();

  // Extract the received CRC32 checksum from the message
  uint32_t receivedCRC = (message[len - 4] << 24) | (message[len - 3] << 16) | (message[len - 2] << 8) | message[len - 1];

  // Compare calculated CRC with received CRC
  return calculatedCRC == receivedCRC;
}
