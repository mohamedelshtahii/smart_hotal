# 📄 Smart Hotel – AIoT (Full Project Documentation)

# 🌍 Smart Hotel – AIoT (Solar‑Powered)

A fully integrated **Smart Hotel** system powered 100% by **solar energy**, designed using **ESP32**, **Raspberry Pi**, **IoT sensors**, and **Blynk IoT Cloud**.

This project focuses on:
- 🔐 Enhanced security  
- ⚡ Real‑time automation  
- 🧠 AIoT smart decisions  
- 👤 Guest comfort  
- 📊 Live monitoring & resource management  

---

# 🚀 1. Project Overview
The Smart Hotel is divided into multiple IoT zones, each controlled by **ESP32 boards**, all connected through a **Raspberry Pi** acting as the main local server + MQTT broker.

All real-time monitoring and controls are available through:
- 🖥 **Blynk IoT Dashboard**  
- 📊 **Local Smart Hotel Web Dashboard (Simulation UI)**  
- 🎛 **Raspberry Pi Control Center**

Each zone includes sensors + actuators that work autonomously and send/receive data through MQTT.

---

# 🧠 2. System Architecture

## 🔌 Main Components
- **ESP32 (per zone)**
- **Raspberry Pi 4** (MQTT Broker + Server)
- **Blynk IoT Cloud**
- **Sensors & Modules:**
  - DHT22 / DHT11  
  - PIR Motion Sensors  
  - Ultrasonic HC‑SR04  
  - MQ‑2, MQ‑135 (Gas)  
  - Flame sensors  
  - RFID RC522 Module  
  - Cameras (OpenMV Cam)  
  - Relays / Fans / Lights / Servos  

---

## 🔗 Communication Layers

| Layer | Technology |
|-------|------------|
| ESP32 → Raspberry Pi | MQTT over Wi‑Fi |
| ESP32 → Blynk Cloud | MQTT + HTTPS |
| Raspberry Pi → Dashboard | Local HTTP Server |
| Internal Communication | JSON Packets |

The Raspberry Pi works as:
- MQTT Broker  
- Device Sync Manager  
- Local Database  
- Backup controller if Wi‑Fi/Internet fails  

---

# 🏨 3. Smart Zones & Their Functions

---

## 🚪 **Entrance Gate**
### Components:
- Ultrasonic Sensor (distance detection)  
- Servo Motor  
- ESP32  

### Functions:
- Detects approaching people  
- Automatically opens gate  
- Manual control from Blynk  

---

## 🛎 **Reception**
### Components:
- DHT22  
- Smoke Sensor  
- OpenMV Camera  
- Display screen  

### Functions:
- Live temp/humidity readings  
- Smoke/fire detection  
- People counting  
- Welcome screen & instructions  

### Blynk:
- View crowd (chart)  
- Emergency lights control  
- Environment monitoring  

---

## 🛣 **Corridors**
### Components:
- PIR sensors  
- Gas & fire sensors  
- Siren  
- Emergency switches  

### Functions:
- Lights turn ON/OFF based on motion  
- Fire/gas alarms  
- Siren control  

### Blynk:
- View alarms  
- Control lights  

---

## 🍳 **Kitchen**
### Components:
- Gas sensor (MQ‑2)  
- DHT22  
- Kitchen water tank sensor  
- Fan relay  

### Automation:
- Fan auto‑starts on high gas  
- Water tank tracking  
- Temp/humidity monitoring  

---

## 🚻 **Public Bathrooms**
### Components:
- PIR  
- Humidity Sensor  
- Fan Relay  

### Automation:
- Auto lights  
- Fan auto ON on high humidity  

---

## 🍽 **Restaurant**
### Components:
- Temp/Humidity  
- MQ‑135 (Air Quality)  
- Smart Fans & Lights  

### Features:
- Comfort monitoring  
- Air quality  
- Crowd tracking  
- Full Blynk control  

---

## ☕ **Café**
### Components:
- Temp/Humidity  
- Camera  
- PIR  
- Smart fan/light  

### Dashboard:
- Café screen: time + weather  
- Occupancy chart  
- Environment monitoring  

---

## 🛏 **Guest Rooms**
### Components:
- RFID lock  
- Sound sensor  
- PIR  
- DHT22  
- Smart TV / Display  
- Bathroom light/fan  

### Features:
- Automatic lights  
- Noise monitoring  
- Temp/humidity regulation  
- Blynk: control lights, fans, TV  
- Live room environment  

---

## 🚗 **Parking Garage**
### Components:
- Ultrasonic sensors  
- IR break beam  
- Display screen  
- Servo gate  

### Features:
- Counts cars in/out  
- Shows available spots  
- App notifications for free spots  

---

## ☀️ **Solar Power System**
### Components:
- Solar panels  
- Charge controller  
- Battery bank  
- Inverter  
- Relay control  

### Blynk Shows:
- Battery %  
- Solar power generated  
- Load consumption  

---

# 🎛 4. Central Control (Raspberry Pi)

The Raspberry Pi is the **core of the hotel**, running:
- 🔄 MQTT Broker (Mosquitto)  
- 🐍 Python scripts for ESP synchronization  
- 💾 Local logging/database  
- 🌐 Internal APIs for dashboards  

### Data Flow:[ESP32 Devices]
↓ MQTT
[Raspberry Pi]
↓ Sync + Storage
[Blynk Cloud] ←→ User App
↓
[Local Web Dashboard]---

# 🛠 5. Hardware List

### 🔌 Microcontrollers:
- ESP32 DevKit v1 (x14+)
- Raspberry Pi 4 (4GB)

### 🔍 Sensors:
- DHT22  
- PIR Motion  
- Ultrasonic HC‑SR04  
- MQ‑2, MQ‑135  
- Flame Sensor  
- Microphone Sound Sensor  
- IR Sensors  
- Water Level Sensor  

### 🔧 Actuators:
- Relays  
- Fans  
- Lights  
- Buzzers  
- Servo Motors  

### 🖥 Displays:
- LCD 16×2  
- OLED Display  
- Restaurant / Café Screens  

---

# 🧵 6. How Devices Communicate
[ESP32 Nodes]
↓ MQTT (JSON)
[Raspberry Pi MQTT Broker]
↓ Data routing
[Blynk Cloud] ←→ [Blynk Mobile App]
↓
[Local Web Dashboard Simulation]---

# 📊 7. Web Dashboard (UI)
The project includes a custom **Smart Hotel Dashboard** showing:

- Real‑time sensor data  
- Crowd & people counts  
- Alerts (fire, gas, smoke)  
- Smart room control states  
- Live parking occupancy  
- Solar system graphs  
- Kitchen tank levels  
- Motion tracking  
- Device states  

⚠ **Note:** Dashboard data is fully simulated for demo purposes.

---

# 🌟 8. Benefits of the System

- ⚡ **Fully solar powered** → eco‑friendly  
- 🔐 **High security** → smoke, fire, gas, RFID  
- 🤖 **Smart automation** → lights, fans, gates  
- 👤 **Improved guest experience**  
- 📉 **Resource efficiency** → water, power, ventilation  
- 📲 **Real‑time insights using Blynk IoT**  

---

# 📦 9. Future Improvements

- AI face recognition at Reception  
- ML-based energy prediction  
- Auto‑booking + room scheduling  
- Integration with hotel PMS  
- Voice‑controlled room features  

---

# 📚 10. License
This project is **open-source** and free for personal and educational use.

---

# 👨‍💻 Developer
**Mohamed**  
IoT Engineer — Embedded Systems Developer  

