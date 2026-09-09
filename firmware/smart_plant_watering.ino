/************* BLYNK SETTINGS *************/
#define BLYNK_TEMPLATE_ID   "TMPL6qbqAKJ9S"
#define BLYNK_TEMPLATE_NAME "esp32 plant watering"
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN"   // <-- replace with your own token from the Blynk console

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/************* DHT11 SETTINGS *************/
#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/************* PIN DEFINITIONS *************/
#define SOIL_MOISTURE_PIN 34
#define MOISTURE_VPIN V1
#define TEMP_VPIN V2
#define HUMID_VPIN V3
#define PUMP_SWITCH V4
#define AUTO_MODE_VPIN V5
#define PUMP_PIN 2

/************* RELAY / ADC CALIBRATION *************/
// Your relay is active-LOW!
// LOW  = Pump ON
// HIGH = Pump OFF
#define RELAY_ACTIVE_HIGH 0
#define WET_ADC 1286
#define DRY_ADC 4095

/************* THRESHOLDS *************/
#define MOISTURE_START 70
#define MOISTURE_STOP 90
#define TEMP_THRESHOLD 40

/************* WIFI *************/
char ssid[] = "YOUR_WIFI_SSID";       // <-- replace with your network name
char pass[] = "YOUR_WIFI_PASSWORD";   // <-- replace with your network password

BlynkTimer timer;

bool autoMode = true;
bool manualPump = false;

/************* Relay control *************/
void pumpOn() {
  if (RELAY_ACTIVE_HIGH)
    digitalWrite(PUMP_PIN, HIGH);
  else
    digitalWrite(PUMP_PIN, LOW);   // active-LOW relay ON
}

void pumpOff() {
  if (RELAY_ACTIVE_HIGH)
    digitalWrite(PUMP_PIN, LOW);
  else
    digitalWrite(PUMP_PIN, HIGH);  // active-LOW relay OFF
}

/************* MAIN PUMP LOGIC *************/
void evaluatePump(int soilPercent, float temperature) {
  Serial.println("----- Pump Evaluation -----");
  Serial.print("Auto Mode: "); Serial.println(autoMode);
  Serial.print("Manual Pump: "); Serial.println(manualPump);
  Serial.print("Moisture: "); Serial.println(soilPercent);
  Serial.print("Temp: "); Serial.println(temperature);

  // Manual override
  if (manualPump) {
    pumpOn();
    Serial.println("PUMP: MANUAL ON");
    return;
  }

  // Auto mode
  if (autoMode) {
    if (soilPercent < MOISTURE_START || temperature > TEMP_THRESHOLD) {
      pumpOn();
      Serial.println("PUMP: AUTO ON");
    }
    else if (soilPercent >= MOISTURE_STOP) {
      pumpOff();
      Serial.println("PUMP: AUTO OFF (moisture high)");
    }
    else {
      pumpOff();
      Serial.println("PUMP: AUTO OFF (in range)");
    }
  }
  else {
    pumpOff();
    Serial.println("PUMP: OFF (Auto disabled)");
  }

  Serial.println("----------------------------");
}

/************* SENSOR READ FUNCTION *************/
void readAllSensors() {
  /** Soil Moisture **/
  int soilRaw = analogRead(SOIL_MOISTURE_PIN);
  int soilPercent = map(soilRaw, DRY_ADC, WET_ADC, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  Serial.print("Soil Raw: "); Serial.println(soilRaw);
  Serial.print("Moisture %: "); Serial.println(soilPercent);
  Blynk.virtualWrite(MOISTURE_VPIN, soilPercent);

  /** DHT11 **/
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    Blynk.virtualWrite(TEMP_VPIN, temperature);
    Blynk.virtualWrite(HUMID_VPIN, humidity);
    Serial.print("Temp: "); Serial.println(temperature);
    Serial.print("Humidity: "); Serial.println(humidity);
  } else {
    Serial.println("DHT ERROR — Using fallback temperature");
    temperature = 25; // safe fallback
  }

  /** Evaluate Pump **/
  evaluatePump(soilPercent, temperature);
}

/************* BLYNK HANDLERS *************/
BLYNK_WRITE(PUMP_SWITCH)
{
  manualPump = (param.asInt() != 0);
  if (manualPump) {
    Serial.println("MANUAL: Pump ON (Blynk)");
    pumpOn();
  } else {
    Serial.println("MANUAL: Pump OFF (Blynk)");
    pumpOff();
  }
}

BLYNK_WRITE(AUTO_MODE_VPIN)
{
  autoMode = (param.asInt() != 0);
  Serial.print("AUTO MODE: ");
  Serial.println(autoMode ? "ENABLED" : "DISABLED");
  readAllSensors();
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(PUMP_SWITCH);
  Blynk.syncVirtual(AUTO_MODE_VPIN);
}

/************* SETUP *************/
void setup() {
  Serial.begin(115200);
  pinMode(PUMP_PIN, OUTPUT);
  pumpOff(); // safe startup (relay OFF)

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(3000L, readAllSensors);
}

/************* LOOP *************/
void loop() {
  Blynk.run();
  timer.run();
}
