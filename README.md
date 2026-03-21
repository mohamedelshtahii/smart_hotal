# 📄 Smart Hotel – AIoT (Full Project Documentation)
<img width="1856" height="845" alt="Screenshot 2026-03-18 044906" src="https://github.com/user-attachments/assets/ef6e0905-b305-4d23-bb93-ce6e075a6657" />

📊 Web Dashboard 
https://ubiquitous-kleicha-7cdf3f.netlify.app/
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
  
<img width="1758" height="489" alt="Screenshot 2026-03-18 044917" src="https://github.com/user-attachments/assets/3ce02f5d-08ab-4bdb-b8b6-003a0bbe54c0" />

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
<img width="1723" height="671" alt="Screenshot 2026-03-18 044928" src="https://github.com/user-attachments/assets/4163b323-2d99-4164-a233-61754a842dda" />

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

<img width="1723" height="671" alt="Screenshot 2026-03-18 044928" src="https://github.com/user-attachments/assets/922c935b-4bfa-4555-be9a-3d79fd7df547" />

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
  
<img width="1719" height="517" alt="Screenshot 2026-03-18 044938" src="https://github.com/user-attachments/assets/2166b8bf-9bba-465e-859d-8f88bca09e0a" />

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

<img width="1741" height="516" alt="Screenshot 2026-03-18 044946" src="https://github.com/user-attachments/assets/2d0c17d2-1f8e-4bb2-af85-83cfca9c20c3" />

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
  
<img width="1473" height="561" alt="Screenshot 2026-03-18 044957" src="https://github.com/user-attachments/assets/5a3b7a33-88c8-4dbb-a4ed-b7ada76df0d4" />

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

<img width="1362" height="809" alt="Screenshot 2026-03-18 045010" src="https://github.com/user-attachments/assets/b1f7f3ea-4f18-4332-b331-b20b3d62a3ca" />

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
https://ubiquitous-kleicha-7cdf3f.netlify.app/
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

