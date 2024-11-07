#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Network Credentials
const char* ssid = "";       //////////////////////
const char* password = "";  ///////////

// ThingSpeak settings
unsigned long channelID = ;   ////////////////////
const char * writeAPIKey = "";///////////////////

// RFID configuration
#define RST_PIN D1
#define SS_PIN D2
MFRC522 rfid(SS_PIN, RST_PIN);

// LED configuration
#define LED_PIN1 D3 // LED pin 1
#define LED_PIN2 D8 // LED pin 2

WiFiClient client;

void setup() {
  Serial.begin(115200);
   delay(10);

  // Connecting to Wi-Fi
  Serial.println();
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  // Wait for the Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(10000);
    Serial.print(".");
  }
  // Successfully connected
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize SPI and RFID
  SPI.begin();
  rfid.PCD_Init();
  ThingSpeak.begin(client);

  // Set up LED pins
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  digitalWrite(LED_PIN1, LOW); // Turn off LEDs initially
  digitalWrite(LED_PIN2, LOW);
}

void loop() {
  // Check for new RFID card
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Read the RFID UID and store it as a string
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""); // Add leading zero if needed
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  int t = strtol(uid.c_str(), NULL, 16);


  Serial.print("UID: ");
  Serial.println(uid);


  // Halt PICC to allow a new reading
  rfid.PICC_HaltA();
  delay(1000); // Wait before the next read


  ThingSpeak.setField(1, t);  // Field 1 is for the random number

  int statusCode = ThingSpeak.writeFields(channelID, writeAPIKey);

  // Check if the data was successfully written to ThingSpeak
  if (statusCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.println("Error sending data to ThingSpeak.");
  }


}