#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>  // ThingSpeak library

#define DHTPIN 2       // Pin connected to DHT11 data pin
#define DHTTYPE DHT11  // Type of DHT sensor

char ssid[] = "Galaxy M314C7C";     // WiFi SSID
char pass[] = "12345678"; // WiFi Password

unsigned long channelID = 2249572;         // Your ThingSpeak Channel ID
const char *apiKey = "628GCXMFNFWL0USV";   // Your ThingSpeak API Key

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  dht.begin();         // Initialize the DHT sensor
  connectWiFi();        // Connect to WiFi
  ThingSpeak.begin(client);  // Initialize ThingSpeak communication
}

void loop() {
  delay(2000);  // Wait for a few seconds between readings

  float temperature = dht.readTemperature();     // Read temperature in Celsius
  float humidity = dht.readHumidity();           // Read humidity

  if (!isnan(temperature) && !isnan(humidity)) {  // Check if sensor readings are valid
    sendDataToThingSpeak(temperature, humidity);  // Send data to ThingSpeak
  } else {
    Serial.println("Failed to read from DHT sensor!");  // Print error message if readings are invalid
  }
  
  delay(30000); // Wait for 30 seconds before the next reading
}

void connectWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, pass);   // Attempt to connect to WiFi using the provided SSID and password
    delay(1000);
  }
  Serial.println("Connected to WiFi");
}

void sendDataToThingSpeak(float temp, float humidity) {
  ThingSpeak.setField(1, temp);       // Set field 1 with temperature data
  ThingSpeak.setField(2, humidity);   // Set field 2 with humidity data
  
  int httpCode = ThingSpeak.writeFields(channelID, apiKey);  // Send data to ThingSpeak

  if (httpCode == 200) {
    Serial.println("Data sent to ThingSpeak");  // Print success message if data is sent successfully
  } else {
    Serial.println("Error sending data to ThingSpeak");  // Print error message if data transmission fails
  }
}
