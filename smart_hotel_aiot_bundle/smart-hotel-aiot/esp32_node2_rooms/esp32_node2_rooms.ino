
// ===================== Smart Hotel AIoT - Node #2 =====================
// Area: Guest Rooms
// Features: Room light/TV/bath light/exhaust (relays), DHT22 env, PIR status,
//           Noise alert via analog sensor, optional RFID guest card
// Dashboard: Blynk Dashboard 2

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "SmartHotel_Node2"
#define BLYNK_AUTH_TOKEN  "YOUR_DEVICE_TOKEN"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <SPI.h>
#include <MFRC522.h>

char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";

#define DHT_PIN     23
#define DHT_TYPE    DHT22
#define PIR_PIN     22
#define SOUND_PIN   35   // analog sound sensor

// RFID (MFRC522)
#define SS_PIN  5
#define RST_PIN 27

// Relays
#define RELAY_ROOM_LIGHT   32
#define RELAY_TV           33
#define RELAY_BATH_LIGHT   25
#define RELAY_BATH_EXHAUST 26

// Blynk Virtual Pins
#define V_ROOM_LIGHT     V20
#define V_TV             V21
#define V_BATH_LIGHT     V22
#define V_EXHAUST        V23
#define V_TEMP_ROOM      V24
#define V_HUM_ROOM       V25
#define V_SOUND_LEVEL    V26
#define V_SOUND_ALARM    V27
#define V_RFID_USER      V28
#define V_PIR_STATUS     V29
#define V_ALERT_TEXT     V30

DHT dht(DHT_PIN, DHT_TYPE);
MFRC522 mfrc522(SS_PIN, RST_PIN);
BlynkTimer timer;

int soundThreshold = 2200; // tune by calibration

BLYNK_WRITE(V_ROOM_LIGHT) { digitalWrite(RELAY_ROOM_LIGHT, param.asInt()); }
BLYNK_WRITE(V_TV)         { digitalWrite(RELAY_TV, param.asInt()); }
BLYNK_WRITE(V_BATH_LIGHT) { digitalWrite(RELAY_BATH_LIGHT, param.asInt()); }
BLYNK_WRITE(V_EXHAUST)    { digitalWrite(RELAY_BATH_EXHAUST, param.asInt()); }

void envTask() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t)) Blynk.virtualWrite(V_TEMP_ROOM, t);
  if (!isnan(h)) Blynk.virtualWrite(V_HUM_ROOM, h);

  int soundRaw = analogRead(SOUND_PIN);
  Blynk.virtualWrite(V_SOUND_LEVEL, soundRaw);
  bool noisy = soundRaw > soundThreshold;
  Blynk.virtualWrite(V_SOUND_ALARM, noisy ? 1 : 0);
  if (noisy) {
    Blynk.logEvent("room_noise", "High noise level in room");
    Blynk.virtualWrite(V_ALERT_TEXT, "Noise Alert! Please reduce volume.");
  }

  Blynk.virtualWrite(V_PIR_STATUS, digitalRead(PIR_PIN));
}

String readRFID() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return "";
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) uid += String(mfrc522.uid.uidByte[i], HEX);
  uid.toUpperCase();
  mfrc522.PICC_HaltA();
  return uid;
}

void rfidTask() {
  String uid = readRFID();
  if (uid.length()) Blynk.virtualWrite(V_RFID_USER, uid);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_ROOM_LIGHT, OUTPUT);
  pinMode(RELAY_TV, OUTPUT);
  pinMode(RELAY_BATH_LIGHT, OUTPUT);
  pinMode(RELAY_BATH_EXHAUST, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  dht.begin();
  SPI.begin();
  mfrc522.PCD_Init();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1500L, envTask);
  timer.setInterval(200L, rfidTask);

  digitalWrite(RELAY_ROOM_LIGHT, LOW);
  digitalWrite(RELAY_TV, LOW);
  digitalWrite(RELAY_BATH_LIGHT, LOW);
  digitalWrite(RELAY_BATH_EXHAUST, LOW);
}

void loop() {
  Blynk.run();
  timer.run();
}
