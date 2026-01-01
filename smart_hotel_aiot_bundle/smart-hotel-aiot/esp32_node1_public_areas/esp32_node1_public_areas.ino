
// ===================== Smart Hotel AIoT - Node #1 =====================
// Areas: Entrance, Reception, Corridors, Kitchen, Restaurant, Café
// Features: Ultrasonic + Servo gate, DHT22 env, MQ Gas alarm, PIR lighting,
//           Water tank level (ultrasonic), Person count from OpenMV via UART
// Dashboard: Blynk Dashboard 1

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "SmartHotel_Node1"
#define BLYNK_AUTH_TOKEN  "YOUR_DEVICE_TOKEN"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <ESP32Servo.h>

char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";

// ---------- Pins (Adjust to your wiring) ----------
#define DHT_PIN_RECEPTION   4
#define DHT_PIN_KITCHEN     23
#define DHT_PIN_CAFE        25
#define DHT_TYPE            DHT22

#define GAS_PIN_KITCHEN     34      // MQ-2 analog
#define BUZZER_PIN          26

#define PIR_CORRIDOR        27
#define PIR_PUBLIC_BATH     21

#define RELAY_OUTSIDE       33      // outside lights
#define RELAY_INSIDE        32      // inside lights master
#define RELAY_CORRIDOR      25      // corridor lights

// Entrance gate (ultrasonic + servo)
#define US_TRIG             5
#define US_ECHO             18
#define SERVO_PIN_GATE      19

// Water tank ultrasonic (optional)
#define TANK_TRIG           14
#define TANK_ECHO           12

// UART to OpenMV (person counter)
#define UART_RX             16
#define UART_TX             17

// ---------- Blynk Virtual Pins ----------
#define V_LIGHT_OUTSIDE     V0
#define V_LIGHT_INSIDE      V1
#define V_TEMP_RECEPTION    V2
#define V_HUM_RECEPTION     V3
#define V_PERSON_COUNT      V4
#define V_CORRIDOR_LIGHTS   V5
#define V_KITCHEN_GAS       V6
#define V_KITCHEN_TEMP      V7
#define V_KITCHEN_HUM       V8
#define V_TANK_LEVEL        V9
#define V_ALERT_TEXT        V10

DHT dhtReception(DHT_PIN_RECEPTION, DHT_TYPE);
DHT dhtKitchen(DHT_PIN_KITCHEN, DHT_TYPE);
DHT dhtCafe(DHT_PIN_CAFE, DHT_TYPE);
Servo gateServo;
BlynkTimer timer;

// Use LEDC for buzzer tone (ESP32 doesn't have tone())
const int BUZZER_CHANNEL = 0;

float readUltrasonicCM(int trig, int echo) {
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000UL); // 30ms timeout
  return duration * 0.034f / 2.0f;
}

void buzzerTone(int freq, int ms) {
  ledcWriteTone(BUZZER_CHANNEL, freq);
  delay(ms);
  ledcWriteTone(BUZZER_CHANNEL, 0);
}

// --------- Blynk Callbacks ---------
BLYNK_WRITE(V_LIGHT_OUTSIDE) { digitalWrite(RELAY_OUTSIDE, param.asInt()); }
BLYNK_WRITE(V_LIGHT_INSIDE)  { digitalWrite(RELAY_INSIDE,  param.asInt()); }
BLYNK_WRITE(V_CORRIDOR_LIGHTS){ digitalWrite(RELAY_CORRIDOR, param.asInt()); }

void readEnvTask() {
  float tR = dhtReception.readTemperature();
  float hR = dhtReception.readHumidity();
  if (!isnan(tR)) Blynk.virtualWrite(V_TEMP_RECEPTION, tR);
  if (!isnan(hR)) Blynk.virtualWrite(V_HUM_RECEPTION, hR);

  float tK = dhtKitchen.readTemperature();
  float hK = dhtKitchen.readHumidity();
  if (!isnan(tK)) Blynk.virtualWrite(V_KITCHEN_TEMP, tK);
  if (!isnan(hK)) Blynk.virtualWrite(V_KITCHEN_HUM, hK);

  int gasRaw = analogRead(GAS_PIN_KITCHEN);
  Blynk.virtualWrite(V_KITCHEN_GAS, gasRaw);
  if (gasRaw > 2800) {
    buzzerTone(1800, 250);
    Blynk.logEvent("gas_alarm", "High gas level detected in kitchen!");
    Blynk.virtualWrite(V_ALERT_TEXT, "Gas Alarm!");
  }
}

float tankLevelPercent() {
  float d = readUltrasonicCM(TANK_TRIG, TANK_ECHO);
  float height = 100.0f;   // cm, adjust to your tank
  float offset = 15.0f;    // sensor offset
  float level = constrain(height - (d - offset), 0.0f, height);
  return (level / height) * 100.0f;
}

void tankTask() {
  Blynk.virtualWrite(V_TANK_LEVEL, tankLevelPercent());
}

void automationTask() {
  float d = readUltrasonicCM(US_TRIG, US_ECHO);
  gateServo.write((d > 0 && d < 80) ? 90 : 0);

  // PIR-based corridor lights
  digitalWrite(RELAY_CORRIDOR, digitalRead(PIR_CORRIDOR));
}

String uartBuf = "";
void readPersonCountTask() {
  while (Serial2.available()) {
    char c = (char)Serial2.read();
    if (c == '
') {
      // Expect format: COUNT:12
      if (uartBuf.startsWith("COUNT:")) {
        int cnt = uartBuf.substring(6).toInt();
        Blynk.virtualWrite(V_PERSON_COUNT, cnt);
      }
      uartBuf = "";
    } else {
      uartBuf += c;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_OUTSIDE, OUTPUT);
  pinMode(RELAY_INSIDE, OUTPUT);
  pinMode(RELAY_CORRIDOR, OUTPUT);
  pinMode(PIR_CORRIDOR, INPUT);
  pinMode(PIR_PUBLIC_BATH, INPUT);
  pinMode(GAS_PIN_KITCHEN, INPUT);
  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);
  pinMode(TANK_TRIG, OUTPUT);
  pinMode(TANK_ECHO, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  ledcSetup(BUZZER_CHANNEL, 2000, 8); // channel, base freq, resolution
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);

  dhtReception.begin();
  dhtKitchen.begin();
  dhtCafe.begin();
  gateServo.attach(SERVO_PIN_GATE);

  Serial2.begin(115200, SERIAL_8N1, UART_RX, UART_TX);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, readEnvTask);
  timer.setInterval(3000L, tankTask);
  timer.setInterval(500L, automationTask);
  timer.setInterval(500L, readPersonCountTask);

  // defaults
  digitalWrite(RELAY_OUTSIDE, LOW);
  digitalWrite(RELAY_INSIDE, LOW);
  digitalWrite(RELAY_CORRIDOR, LOW);
  gateServo.write(0);
}

void loop() {
  Blynk.run();
  timer.run();
}
