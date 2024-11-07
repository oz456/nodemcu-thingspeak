#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Network Credentials
const char* ssid = "";       // Wi-Fi SSID
const char* password = "";  // Wi-Fi Password

// ThingSpeak settings
unsigned long channelID = ;   // Replace with your ThingSpeak channel ID
const char * writeAPIKey = ""; // Replace with your ThingSpeak Write API Key

WiFiClient client;

void setup() {
  Serial.begin(57600);
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

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  int t = random(1010110129, 1912735479);  // Random number between 0 and 100

  // Print the random number to the Serial Monitor
  Serial.print("Random Number: ");
  Serial.println(t);

  // Send random number to ThingSpeak
  ThingSpeak.setField(1, t);  // Field 1 is for the random number

  // Write the data to ThingSpeak
  int statusCode = ThingSpeak.writeFields(channelID, writeAPIKey);

  // Check if the data was successfully written to ThingSpeak
  if (statusCode == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.println("Error sending data to ThingSpeak.");
  }

  // Wait 20 seconds before sending the next update
  delay(20000);  // Update every 20 seconds
  
}