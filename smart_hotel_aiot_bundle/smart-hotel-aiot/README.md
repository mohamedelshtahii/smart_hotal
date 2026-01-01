
# Smart Hotel AIoT — Full Project (ESP32 + OpenMV + Blynk IoT)

**Author:** Mohamed (IoT Engineer)
**Date:** 2025-12-16

A complete, production-ready prototype for a sustainable **Smart Hotel** powered by solar energy and orchestrated with AIoT. The system is divided into **four ESP32 nodes** plus an **OpenMV Cam H7** for computer vision. Control and monitoring are unified via **Blynk IoT** dashboards.

---
## 1) System Overview
- **Node #1 – Public Areas:** Entrance gate (ultrasonic + servo), reception, corridors, kitchen, restaurant, café. Environment monitoring (DHT22), gas alarm (MQ-series), PIR-based lighting automation, water tank level via ultrasonic. Receives person count from OpenMV over UART.
- **Node #2 – Guest Rooms:** Room automation (lights, TV, bathroom light, exhaust fan), environment monitoring (DHT22), occupancy (PIR), **noise alert** (analog sound sensor), optional **RFID** (MFRC522) for guest card.
- **Node #3 – Smart Garage:** Automated gate (ultrasonic + servo). Slot occupancy with IR sensors for each parking place. Live counters (occupied/free).
- **Node #4 – Solar & Battery:** Panel/battery voltage and current (ADC + INA219). Basic battery SOC estimate and automatic inverter relay control based on thresholds.
- **OpenMV Cam H7:** Face/person counting and streaming the count to Node #1 via UART; count displayed in Blynk.

> ⚠️ **Safety**: Any **AC mains** control must use certified, properly-rated **isolated relays/contactors**, correct wiring, grounding, and protection (fuses/breakers). Calibrate gas and sound sensors. Always test with a qualified electrician.

---
## 2) Software & Tools Used
- **Arduino IDE 2.x** (or **PlatformIO**) for ESP32 sketches
- **Blynk IoT** (cloud + mobile app) — device templates, dashboards, events/notifications
- **OpenMV IDE** for camera programming
- **GitHub** for version control (optional)

### Key Libraries (Arduino)
- `Blynk` (new IoT platform) — `#include <BlynkSimpleEsp32.h>`
- `WiFi.h` (ESP32 Wi-Fi)
- `DHT` (DHT22 temperature/humidity)
- `ESP32Servo` (servo control)
- `MFRC522` (RFID)
- `Adafruit_INA219` (current monitor)

---
## 3) Hardware Components (reference)
- **ESP32** × 4
- **Ultrasonic sensors** (HC‑SR04) for gates & tank level
- **Servo motors** for gates
- **DHT22** sensors (reception, kitchen, café, rooms)
- **PIR** sensors (corridors, rooms, public bathrooms)
- **MQ-series gas sensor** (kitchen)
- **Analog sound sensor** (rooms)
- **RFID MFRC522** module (rooms – optional)
- **IR parking sensors** (one per slot)
- **Ina219** current sensor (solar panel string)
- **Relays** (isolated) for AC bulbs, TV, exhaust fan, inverter control
- **LM2596** (voltage regulator) and proper DC power distribution
- **Solar**: PV panels, charge controller, battery bank, DC‑to‑AC inverter

---
## 4) Blynk Virtual Pins (DataStreams)

### Dashboard 1 – Public Areas & Energy
- `V0` Outside lights (Button)
- `V1` Inside lights (Button)
- `V2` Reception temperature (Gauge)
- `V3` Reception humidity (Gauge)
- `V4` Person count (Number/Label)
- `V5` Corridor lights state (Button/LED)
- `V6` Kitchen gas level (Gauge)
- `V7` Kitchen temperature (Gauge)
- `V8` Kitchen humidity (Gauge)
- `V9` Water tank level % (Gauge/Ramp)
- `V10` Alert text (Text)
- `V60` Battery voltage (Gauge)
- `V61` Panel voltage (Gauge)
- `V62` Panel current (Gauge)
- `V63` PV power (Chart/Gauge)
- `V64` Battery SOC % (Gauge)
- `V65` Inverter state (LED)
- `V66` Energy alert text (Text)

### Dashboard 2 – Guest Rooms
- `V20` Room light (Button)
- `V21` TV (Button)
- `V22` Bathroom light (Button)
- `V23` Bathroom exhaust (Button)
- `V24` Room temperature (Gauge)
- `V25` Room humidity (Gauge)
- `V26` Noise level (Gauge)
- `V27` Noise alarm (LED)
- `V28` RFID UID (Label)
- `V29` PIR status (LED)
- `V30` Room alert text (Text)

---
## 5) How to Build & Flash
1. **Install libraries** via Arduino Library Manager: Blynk, DHT sensor, ESP32Servo, MFRC522, Adafruit INA219.
2. Create a **Blynk Template** and **add DataStreams** as listed above. Create device(s) and copy the `TEMPLATE_ID`, `DEVICE_NAME`, and `AUTH_TOKEN` to each sketch.
3. Open each folder’s `.ino` in Arduino IDE, set **Board** = `ESP32 Dev Module` (or your variant), choose the right **COM port**.
4. Replace `YOUR_SSID`, `YOUR_PASSWORD`, and Blynk credentials in every sketch.
5. Flash Node #1..#4 individually.
6. In **OpenMV IDE**, open `openmv_person_counter/main.py`, set UART pins to your board, and run/save to the camera.

---
## 6) Wiring Notes
- **Ultrasonic**: Trig/Echo to GPIO (use level shifting if needed). Keep cable lengths short.
- **Servo**: Power with a **separate 5V** supply (adequate current), common GND with ESP32.
- **Relays**: Use opto-isolated modules; flyback protection for DC loads; follow AC safety rules.
- **MQ Gas**: Requires preheat time and calibration. Place in ventilated area.
- **Sound sensor**: Tune threshold after measuring ambient noise.
- **INA219**: Place shunt in series with panel line; check current range.

---
## 7) Events & Notifications (Blynk)
Create events named: `gas_alarm`, `room_noise`, `low_soc`. The sketches log these events so you can enable app/email notifications.

---
## 8) Repository Layout
```
smart-hotel-aiot/
├─ README.md
├─ esp32_node1_public_areas/esp32_node1_public_areas.ino
├─ esp32_node2_rooms/esp32_node2_rooms.ino
├─ esp32_node3_garage/esp32_node3_garage.ino
├─ esp32_node4_solar/esp32_node4_solar.ino
└─ openmv_person_counter/main.py
```

---
## 9) License & Use
This bundle is provided as a working prototype. Use at your own risk; ensure compliance with electrical codes and safety standards.
