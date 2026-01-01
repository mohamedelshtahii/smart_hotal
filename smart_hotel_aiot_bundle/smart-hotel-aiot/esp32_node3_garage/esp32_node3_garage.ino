
// ===================== Smart Hotel AIoT - Node #3 =====================
// Area: Smart Garage
// Features: Gate (ultrasonic + servo), slot occupancy via IR sensors, live counts
// Dashboard: Blynk Dashboard 1 (or dedicated)

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "SmartHotel_Node3"
#define BLYNK_AUTH_TOKEN  "YOUR_DEVICE_TOKEN"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";

// Gate ultrasonic + servo
#define G_TRIG    4
#define G_ECHO    2
#define SERVO_PIN 15

// IR sensors for slots (adjust number)
const int NUM_SLOTS = 10;
int irPins[NUM_SLOTS] = {32,33,25,26,27,14,12,13,21,22}; // example pins

// Blynk virtual pins
#define V_CARS_COUNT     V40
#define V_SLOTS_FREE     V41
#define V_GATE_STATE     V42

Servo gateServo;
BlynkTimer timer;

float readUltrasonicCM(int trig, int echo) {
  pinMode(trig, OUTPUT); pinMode(echo, INPUT);
  digitalWrite(trig, LOW); delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000UL);
  return duration * 0.034f / 2.0f;
}

void gateTask() {
  float d = readUltrasonicCM(G_TRIG, G_ECHO);
  bool open = (d > 0 && d < 90);
  gateServo.write(open ? 90 : 0);
  Blynk.virtualWrite(V_GATE_STATE, open ? 1 : 0);
}

void slotsTask() {
  int occupied = 0;
  for (int i = 0; i < NUM_SLOTS; i++) {
    int v = digitalRead(irPins[i]);
    // Adjust logic to your sensor: many IR modules are LOW when blocked
    if (v == LOW) occupied++;
  }
  int freeSlots = NUM_SLOTS - occupied;
  Blynk.virtualWrite(V_CARS_COUNT, occupied);
  Blynk.virtualWrite(V_SLOTS_FREE, freeSlots);
}

void setup() {
  Serial.begin(115200);
  gateServo.attach(SERVO_PIN);
  for (int i = 0; i < NUM_SLOTS; i++) pinMode(irPins[i], INPUT_PULLUP);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(400L, gateTask);
  timer.setInterval(1000L, slotsTask);
}

void loop() {
  Blynk.run();
  timer.run();
}
