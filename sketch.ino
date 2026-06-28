#include <WiFi.h>
#include <DHT.h>

// --- Configuration ---
const char* WIFI_SSID = "Wokwi-GUEST"; // Wokwi's default virtual Wi-Fi
const char* WIFI_PASSWORD = "";
const char* TS_SERVER = "api.thingspeak.com";
String TS_API_KEY = "B2Z5U7O7FEUOZEOQ"; // Replace with your key

#define DHTPIN 15          // GPIO pin connected to DHT22
#define DHTTYPE DHT22      
#define PUMP_PIN 2         // GPIO pin for Virtual Pump (LED/Relay)

DHT dht(DHTPIN, DHTTYPE);

// --- Simulation Variables ---
float soilMoisture = 65.0; // Starting moisture percentage
float temperature = 0.0;
float humidity = 0.0;
bool pumpState = false;

// Automation Threshold
const float MOISTURE_THRESHOLD_LOW = 30.0;
const float MOISTURE_THRESHOLD_HIGH = 80.0;

unsigned long lastUploadTime = 0;
const unsigned long uploadInterval = 15000; // ThingSpeak accepts updates every 15s

void setup() {
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  
  dht.begin();
  
  // Connect to Virtual Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
}

void loop() {
  // 1. Read environmental data from DHT22
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Check if readings failed
  if (isnan(temperature) || isnan(humidity)) {
    temperature = 28.5; // Fallback simulated values if sensor fails
    humidity = 60.0;
  }

  // 2. Moisture Simulation Algorithm
  // If pump is active, moisture goes up. If inactive, it dries out.
  if (pumpState) {
    soilMoisture += 4.5; // Simulate watering effect
    if (soilMoisture > 100.0) soilMoisture = 100.0;
  } else {
    // Evaporation rates scale slightly faster if temperature is higher
    float evaporationRate = (temperature > 30) ? 0.8 : 0.4;
    soilMoisture -= evaporationRate;
    if (soilMoisture < 0.0) soilMoisture = 0.0;
  }

  // 3. Automation Control Logic
  if (soilMoisture < MOISTURE_THRESHOLD_LOW && !pumpState) {
    pumpState = true;
    digitalWrite(PUMP_PIN, HIGH);
    Serial.println("[ALERT] Soil dry! Turning Pump ON.");
  } 
  else if (soilMoisture > MOISTURE_THRESHOLD_HIGH && pumpState) {
    pumpState = false;
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("[INFO] Target moisture reached. Turning Pump OFF.");
  }

  // Local Monitoring Output
  Serial.printf("Moisture: %.1f%% | Temp: %.1f°C | Hum: %.1f%% | Pump: %s\n", 
                soilMoisture, temperature, humidity, pumpState ? "ON" : "OFF");

  // 4. Send Data to ThingSpeak Dashboard
  if (millis() - lastUploadTime >= uploadInterval) {
    uploadToCloud();
    lastUploadTime = millis();
  }

  delay(2000); // Local tracking loop updates every 2 seconds
}

void uploadToCloud() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    if (client.connect(TS_SERVER, 80)) {
      // Construct API request string
      String url = "/update?api_key=" + TS_API_KEY;
      url += "&field1=" + String(soilMoisture);
      url += "&field2=" + String(temperature);
      url += "&field3=" + String(humidity);
      url += "&field4=" + String(pumpState ? 1 : 0);
      
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + TS_SERVER + "\r\n" +
                   "Connection: close\r\n\r\n");
      
      Serial.println(">> Data successfully transmitted to ThingSpeak Dashboard.");
    }
    client.stop();
  } else {
    Serial.println("[ERROR] Wi-Fi Disconnected. Unable to send telemetry.");
  }
}
