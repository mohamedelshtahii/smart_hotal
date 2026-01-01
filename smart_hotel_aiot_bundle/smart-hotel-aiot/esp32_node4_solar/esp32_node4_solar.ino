
// ===================== Smart Hotel AIoT - Node #4 =====================
// Area: Solar Power & Battery System
// Features: Monitor volt/current via INA219 & ADC, compute power & SOC, control inverter relay
// Dashboard: Blynk Dashboard 1

#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_DEVICE_NAME "SmartHotel_Node4"
#define BLYNK_AUTH_TOKEN  "YOUR_DEVICE_TOKEN"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";

// ADC pins (voltage dividers)
#define VBAT_ADC   34
#define VPV_ADC    35

// Relay to control DC-AC inverter
#define RELAY_INVERTER  26

// Blynk Vpins
#define V_VBAT       V60
#define V_VPV        V61
#define V_IPV        V62
#define V_P_PV       V63
#define V_SOC        V64
#define V_INV_STATE  V65
#define V_ALERT_TEXT V66

Adafruit_INA219 ina219; // for panel current
BlynkTimer timer;

float adcToVoltage(int adcPin, float rTop, float rBottom) {
  int raw = analogRead(adcPin);
  float vADC = (raw / 4095.0f) * 3.3f;
  float vReal = vADC * ((rTop + rBottom) / rBottom);
  return vReal;
}

float estimateSOC(float vbat) {
  // Very rough voltage-based estimate for 12V lead-acid (adjust to chemistry)
  if (vbat >= 12.8f) return 100.0f;
  if (vbat >= 12.5f) return 90.0f;
  if (vbat >= 12.2f) return 75.0f;
  if (vbat >= 12.0f) return 60.0f;
  if (vbat >= 11.8f) return 40.0f;
  if (vbat >= 11.6f) return 25.0f;
  return 10.0f;
}

void powerTask() {
  // Adjust divider resistors to your circuit
  float vbat = adcToVoltage(VBAT_ADC, 30000.0f, 7500.0f); // example 30k/7.5k
  float vpv  = adcToVoltage(VPV_ADC, 30000.0f, 7500.0f);

  float ipv = 0.0f;
  if (ina219.begin()) {
    ipv = ina219.getCurrent_mA() / 1000.0f; // A
  }
  float p_pv = vpv * ipv;

  float soc = estimateSOC(vbat);

  Blynk.virtualWrite(V_VBAT, vbat);
  Blynk.virtualWrite(V_VPV, vpv);
  Blynk.virtualWrite(V_IPV, ipv);
  Blynk.virtualWrite(V_P_PV, p_pv);
  Blynk.virtualWrite(V_SOC, soc);

  static bool invOn = false;
  if (soc > 50.0f && p_pv > 50.0f) invOn = true;   // thresholds to tune
  if (soc < 25.0f) {
    invOn = false;
    Blynk.logEvent("low_soc", "Battery SOC low, inverter turned OFF");
    Blynk.virtualWrite(V_ALERT_TEXT, "Low battery: Inverter OFF");
  }
  digitalWrite(RELAY_INVERTER, invOn ? HIGH : LOW);
  Blynk.virtualWrite(V_INV_STATE, invOn ? 1 : 0);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_INVERTER, OUTPUT);
  digitalWrite(RELAY_INVERTER, LOW);
  Wire.begin();
  ina219.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1500L, powerTask);
}

void loop() {
  Blynk.run();
  timer.run();
}
