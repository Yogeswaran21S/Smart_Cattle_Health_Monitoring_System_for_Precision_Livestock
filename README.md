#🐄 Smart Cattle Health Monitoring System for Precision Livestock

[![IoT](https://img.shields.io/badge/IoT-Enabled-blue?style=for-the-badge)]()
[![ESP32](https://img.shields.io/badge/ESP32-Powered-orange?style=for-the-badge)]()
[![LoRa](https://img.shields.io/badge/LoRa-Communication-green?style=for-the-badge)]()
[![ThingSpeak](https://img.shields.io/badge/Cloud-ThingSpeak-lightgrey?style=for-the-badge)]()
[![Status](https://img.shields.io/badge/Status-Prototype-success?style=for-the-badge)]()

#🌾 Project Overview
The Smart Cattle Health Monitoring System is an IoT-based embedded solution designed to monitor and manage the health of cattle in real-time.  
It tracks key health metrics like body temperature, pulse rate, and motion activity, transmitting them via **LoRa** for long-range communication.  
This helps farmers detect early signs of illness and ensures efficient livestock management — even in remote areas.

#⚙️ Features
✅ Real-time monitoring of vital health parameters  
✅ LoRa wireless communication for long-distance, low-power data transmission  
✅ Environmental sensing via DHT22 (humidity + temperature)  
✅ Pulse and motion tracking using dedicated sensors  
✅ ThingSpeak cloud integration for data visualization and analysis  
✅ GPS tracking for cattle location management  
✅ Low-cost, low-power embedded design  

#🔧 Components Used
|-----------------------------------------------------------------------|
|       Component        |                  Function                    |
|------------------------|----------------------------------------------|
| **ESP32 Wroom**        | Central microcontroller, Wi-Fi + BLE capable |
| **LoRa SX1278**        | Long-range communication                     |
| **DHT22 Sensor**       | Measures temperature & humidity              |
| **Pulse Rate Sensor**  | Monitors cattle heart rate                   |
| **MPU6050**            | Detects movement & orientation               |
| **GPS Module**         | Provides location data                       |
| **Relay Module**       | Controls devices based on data               |
| **Power Supply**       | Battery/solar supported for field use        |
|-----------------------------------------------------------------------|

# 💡 Objective

To create a smart, scalable, and sustainable IoT solution that empowers farmers to monitor livestock health remotely, make data-driven decisions, and improve cattle productivity.

## 📊 System Architecture

plaintext
[Sensors] → [ESP32 Node] → [LoRa Transmitter] ⇄ [LoRa Receiver] → [ThingSpeak Cloud] → [User Interface / Mobile App]
