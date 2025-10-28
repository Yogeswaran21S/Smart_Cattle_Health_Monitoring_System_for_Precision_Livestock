// -------------------- Transmitter Code --------------------
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define PULSE_PIN 34

// LoRa Pins
#define SS 5
#define RST 14
#define DIO0 2

Adafruit_MPU6050 mpu;
DHT dht(DHTPIN, DHTTYPE);
TinyGPSPlus gps;
HardwareSerial SerialGPS(1);

void setup() {
  Serial.begin(115200);
  SerialGPS.begin(9600, SERIAL_8N1, 16, 17); // GPS: TX=17, RX=16
  dht.begin();
  Wire.begin(21, 22); // SDA=21, SCL=22

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    while (1);
  }
  Serial.println("MPU6050 connected!");

  // Initialize LoRa
  Serial.println("LoRa Transmitter");
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {   // Frequency: 433MHz
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // --- Read Sensors ---
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int pulseValue = analogRead(PULSE_PIN);
  float voltage = pulseValue * (3.3 / 4095.0);
  int bpm = (voltage / 3.3) * 675;

  sensors_event_t accel, gyro, temp_mpu;
  mpu.getEvent(&accel, &gyro, &temp_mpu);

  // --- Read GPS ---
  while (SerialGPS.available()) {
    gps.encode(SerialGPS.read());
  }

  float latitude = gps.location.lat();
  float longitude = gps.location.lng();

  // --- Display Values ---
  Serial.println("------ Sensor Data ------");
  Serial.printf("Temp: %.2f°C  Hum: %.2f%%  BPM: %d\n", temp, hum, bpm);
  Serial.printf("Accel X: %.2f  Y: %.2f  Z: %.2f\n", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
  Serial.printf("GPS: %.6f , %.6f\n", latitude, longitude);

  // --- Combine Data for LoRa Packet ---
  String data = "TEMP:" + String(temp) +
                ",HUM:" + String(hum) +
                ",BPM:" + String(bpm) +
                ",ACCX:" + String(accel.acceleration.x) +
                ",ACCY:" + String(accel.acceleration.y) +
                ",ACCZ:" + String(accel.acceleration.z) +
                ",LAT:" + String(latitude, 6) +
                ",LON:" + String(longitude, 6);

  // --- Send via LoRa ---
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();

  Serial.println("Data Sent via LoRa ✅");
  delay(2000);
}
