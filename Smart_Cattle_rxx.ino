// -------------------- Receiver Code (ESP32-WROOM32 + LoRa + ThingSpeak) --------------------
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <LoRa.h>

// -------------------- Wi-Fi Setup --------------------
const char* ssid = "realme 7";
const char* password = "sanjay1877";

// -------------------- ThingSpeak Setup --------------------
unsigned long channelID = 3115385;  // 🔹 Replace with your Channel ID
String writeAPIKey = "OXZS5ST5GY3BG7FI";  // 🔹 Replace with your Write API Key
const char* server = "http://api.thingspeak.com/update";

// -------------------- LoRa Pins --------------------
#define SS   5
#define RST  14
#define DIO0 26

String LoRaData;
float temp, hum, bpm, accX, accY, accZ, lat, lon;

void setup() {
  Serial.begin(115200);
  Serial.println("=== LoRa Receiver + ThingSpeak ===");

  // ---- Initialize Wi-Fi ----
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 30) {
    delay(1000);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Wi-Fi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Wi-Fi Connection Failed!");
  }

  // ---- Initialize LoRa ----
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("❌ LoRa start failed!");
    while (1);
  }
  Serial.println("✅ LoRa initialized successfully.");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    LoRaData = "";
    while (LoRa.available()) {
      LoRaData += (char)LoRa.read();
    }

    Serial.println("\n📩 Received LoRa Packet:");
    Serial.println(LoRaData);
    parseLoRaData(LoRaData);

    Serial.printf("Temp: %.2f°C | Hum: %.2f%% | BPM: %.0f\n", temp, hum, bpm);
    Serial.printf("Accel X: %.2f | Y: %.2f | Z: %.2f\n", accX, accY, accZ);
    Serial.printf("GPS -> Lat: %.6f | Lon: %.6f\n", lat, lon);

    if (WiFi.status() == WL_CONNECTED) {
      sendToThingSpeak();
    } else {
      Serial.println("⚠ Wi-Fi disconnected, retrying...");
      WiFi.reconnect();
    }
  }
}

// -------------------- Parse LoRa Data --------------------
void parseLoRaData(String data) {
  int index;
  index = data.indexOf("TEMP:"); if (index != -1) temp = data.substring(index + 5, data.indexOf(",", index)).toFloat();
  index = data.indexOf("HUM:");  if (index != -1) hum = data.substring(index + 4, data.indexOf(",", index)).toFloat();
  index = data.indexOf("BPM:");  if (index != -1) bpm = data.substring(index + 4, data.indexOf(",", index)).toFloat();
  index = data.indexOf("ACCX:"); if (index != -1) accX = data.substring(index + 5, data.indexOf(",", index)).toFloat();
  index = data.indexOf("ACCY:"); if (index != -1) accY = data.substring(index + 5, data.indexOf(",", index)).toFloat();
  index = data.indexOf("ACCZ:"); if (index != -1) accZ = data.substring(index + 5, data.indexOf(",", index)).toFloat();
  index = data.indexOf("LAT:");  if (index != -1) lat = data.substring(index + 4, data.indexOf(",", index)).toFloat();
  index = data.indexOf("LON:");  if (index != -1) lon = data.substring(index + 4).toFloat();
}

// -------------------- Send Data to ThingSpeak --------------------
void sendToThingSpeak() {
  HTTPClient http;

  String url = String(server) + "?api_key=" + writeAPIKey +
               "&field1=" + String(temp) +
               "&field2=" + String(hum) +
               "&field3=" + String(bpm) +
               "&field4=" + String(accX) +
               "&field5=" + String(accY) +
               "&field6=" + String(accZ) +
               "&field7=" + String(lat, 6) +
               "&field8=" + String(lon, 6);

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.println("✅ Data uploaded to ThingSpeak successfully!");
  } else {
    Serial.println("❌ Failed to upload data.");
  }

  http.end();
  delay(20000); // ThingSpeak minimum delay = 15 sec
}
