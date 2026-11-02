# Smart Plant Watering System

**Automated & Remote-Controlled Irrigation using ESP32 and Blynk IoT**

[![Platform](https://img.shields.io/badge/platform-ESP32-blue)](https://www.espressif.com/en/products/socs/esp32)
[![IDE](https://img.shields.io/badge/built%20with-Arduino%20IDE-00979D)](https://www.arduino.cc/en/software)
[![IoT](https://img.shields.io/badge/cloud-Blynk%20IoT-1EA9B8)](https://blynk.io/)
[![License](https://img.shields.io/badge/license-MIT-green)](#-license)

A self-regulating smart irrigation prototype that reads live soil moisture, temperature, and humidity data, decides when a plant needs water, and lets you monitor or override everything remotely from the Blynk mobile/web dashboard.

---

## Table of Contents

- [Overview](#-overview)
- [Demo](#-demo)
- [Features](#-features)
- [Hardware Components](#-hardware-components)
- [Circuit Diagram](#-circuit-diagram)
- [Software & Cloud](#-software--cloud)
- [Blynk Dashboard](#-blynk-dashboard)
- [How It Works](#-how-it-works)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
- [Final Setup](#-final-setup)
- [Evaluation Criteria](#-evaluation-criteria)
- [Achievements](#-achievements)
- [Future Improvements](#-future-improvements)
- [Authors](#-authors)
- [References](#-references)
- [License](#-license)

---

## Overview

Manual plant watering is inconsistent — too much water, too little, or a missed day entirely when nobody's home. This project replaces the guesswork with a closed-loop IoT system: an **ESP32** microcontroller reads a **soil moisture sensor** and a **DHT11 temperature/humidity sensor**, then automatically drives a **relay-controlled mini water pump** to keep the soil in a healthy moisture range — while streaming live readings to the **Blynk IoT Cloud** for remote monitoring and manual control from a phone or browser.

The goal was a low-cost, energy-efficient prototype that proves out automated, data-driven irrigation without needing multi-zone hardware, weather APIs, or heavy compute — just a reliable sense → decide → act loop.

## Demo

 **[Watch the hardware demo video](media/demo/iot%20final%20show%20hardware.mp4)**

*(GitHub will play the video directly when you open the link. If it's too large to preview inline, download it from the same link.)*

## Features

-  Real-time monitoring of **soil moisture, temperature, and humidity**
-  **Automatic pump control** based on configurable moisture/temperature thresholds
-  **Manual override** — turn the pump on/off anytime from the Blynk app
-  **Auto/Manual mode toggle** synced live with the cloud
-  Wireless data logging and control via **Blynk IoT**
-  Safe startup state (pump forced OFF on boot/reconnect)

## Hardware Components

<table>
<tr>
<th>Component</th>
<th>Image</th>
<th>Purpose</th>
</tr>
<tr>
<td>ESP32 DevKit V1</td>
<td><img src="media/images/components/Esp32%20board%20%28model-%20devkit%20v1%29.png" width="160"></td>
<td>Main microcontroller — Wi-Fi connectivity + sensor/pump control</td>
</tr>
<tr>
<td>Soil Moisture Sensor</td>
<td><img src="media/images/components/Soil%20moisture%20sensor.png" width="160"></td>
<td>Detects soil water level (analog reading)</td>
</tr>
<tr>
<td>DHT11 Sensor</td>
<td><img src="media/images/components/DHT11%20Sensor.png" width="160"></td>
<td>Measures ambient temperature and humidity</td>
</tr>
<tr>
<td>Relay Module</td>
<td><img src="media/images/components/Relay%20module-.png" width="160"></td>
<td>Switches the water pump ON/OFF (active-LOW)</td>
</tr>
<tr>
<td>Mini Water Pump</td>
<td><img src="media/images/components/Mini%20Water%20pump%20and%20pipe.png" width="160"></td>
<td>Delivers water to the plant</td>
</tr>
<tr>
<td>Breadboard</td>
<td><img src="media/images/components/Breadboard.png" width="160"></td>
<td>Prototyping and sensor wiring</td>
</tr>
<tr>
<td>Jumper Wires</td>
<td><img src="media/images/components/Jumper%20wires.png" width="160"></td>
<td>Connecting components on the breadboard</td>
</tr>
<tr>
<td>Connecting Cables</td>
<td><img src="media/images/components/Connecting%20wires.png" width="160"></td>
<td>Power and signal connections</td>
</tr>
</table>

**Power:** All components run off a 5V DC supply via USB.

## Circuit Diagram

<img src="docs/Circuit%20diagram.png" width="700">

## Software & Cloud

| Tool | Purpose |
|---|---|
| [Arduino IDE](https://www.arduino.cc/en/software) | Writing and flashing firmware to the ESP32 |
| [Blynk IoT Platform](https://blynk.io/) | Remote dashboard, notifications, and manual control |
| `DHT.h` | Temperature/humidity sensor driver |
| `BlynkSimpleEsp32.h` | Blynk cloud communication over Wi-Fi |

## Blynk Dashboard

<img src="docs_dashboard/Dashboard.png" width="700">

>  This is a **recreated mockup** of the original dashboard (same layout, widgets, and live values as the working project) — the original live screenshots weren't saved during development. See the [demo video](#-demo) for the dashboard running on real hardware.

The dashboard exposes:
- **Pump Switch** — manual pump control
- **Auto Water** — toggle automatic mode on/off
- **Temperature** / **Humidity** — live DHT11 readings
- **Moisture** — live soil moisture gauge (0–100%)

## How It Works

1. **Read** — the ESP32 polls the soil moisture sensor and DHT11 every 3 seconds.
2. **Decide** — in Auto mode, the pump turns **ON** when moisture drops below the low threshold *or* temperature exceeds the safety threshold, and turns **OFF** once moisture recovers above the high threshold.
3. **Act** — the relay switches the pump; the outcome is logged to Serial and pushed to Blynk.
4. **Override** — flipping the Pump Switch in Blynk takes priority over automatic logic at any time.

```
Moisture < 70%  → Pump ON  (auto)
Moisture ≥ 90%  → Pump OFF (auto)
Temperature > 40°C → Pump ON  (safety)
Manual Switch ON   → Pump ON  (overrides auto logic)
```

*(These are the thresholds implemented in firmware. Note: the written project report describes indicative values of 30%/70% for the same logic — the firmware values above are what the hardware actually runs on.)*

## Project Structure

```
Smart plant watering system/
├── README.md
├── docs/
│   ├── Circuit diagram.png
│   └── PROJECT_REPORT.docx
├── docs_dashboard/
│   └── Dashboard.png
├── firmware/
│   └── smart_plant_watering.ino
└── media/
    ├── demo/
    │   └── iot final show hardware.mp4
    └── images/
        ├── Final_Setup1.jpeg
        ├── Final_Setup2.jpeg
        ├── Final_Setup3.jpeg
        ├── Final_Setup4.jpeg
        └── components/
            ├── Breadboard.png
            ├── Connecting wires.png
            ├── DHT11 Sensor.png
            ├── Esp32 board (model- devkit v1).png
            ├── Jumper wires.png
            ├── Mini Water pump and pipe.png
            ├── Relay module-.png
            └── Soil moisture sensor.png
```

## Getting Started

1. **Wire the hardware** according to the [circuit diagram](#-circuit-diagram):
   - Soil moisture sensor → analog pin `34`
   - DHT11 data pin → pin `12`
   - Relay `IN` → pin `2` (relay is **active-LOW**: `LOW` = pump ON)
2. **Install libraries** in Arduino IDE: `Blynk`, `DHT sensor library` (via Library Manager).
3. **Set up Blynk:**
   - Create a new template in the [Blynk Console](https://blynk.cloud/).
   - Add datastreams: `V1` Moisture, `V2` Temperature, `V3` Humidity, `V4` Pump Switch, `V5` Auto Mode.
   - Copy your device **Auth Token**.
4. **Configure firmware:** open `firmware/smart_plant_watering.ino` and replace:
   ```cpp
   #define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"
   char ssid[] = "YOUR_WIFI_SSID";
   char pass[] = "YOUR_WIFI_PASSWORD";
   ```
5. **Flash the ESP32** and open Serial Monitor at `115200` baud to confirm sensor readings and Blynk connection.
6. **Build the dashboard** in Blynk with 2 switches and 3 value/gauge widgets bound to the datastreams above.

## Final Setup

<p float="left">
  <img src="media/images/Final_Setup1.jpeg" width="200">
  <img src="media/images/Final_Setup2.jpeg" width="200">
  <img src="media/images/Final_Setup3.jpeg" width="200">
  <img src="media/images/Final_Setup4.jpeg" width="200">
</p>

## Evaluation Criteria

The prototype was tested against:
- **Sensor Accuracy** — comparing sensor readings against manual measurements
- **Automation Reliability** — consistency of pump ON/OFF triggering
- **Network Responsiveness** — delay between cloud commands and physical pump action
- **User Control Efficiency** — manual override always takes priority over automation
- **Component Stability** — sustained operation over time

## Achievements

- Fully working sense → decide → act automation loop with no manual intervention required
- Real-time two-way sync between hardware and the Blynk cloud dashboard
- Manual override coexists cleanly with automatic control
- Delivered as a low-cost, low-power prototype with no specialized equipment

## Future Improvements

- Switch to a **capacitive** soil moisture sensor for better long-term accuracy (resistive sensors corrode over time)
- Add a **light sensor** for fuller environmental awareness
- Introduce **ML-based adaptive watering** tuned to plant species and conditions
- Add **solar power** for outdoor, off-grid deployment

## Authors

Project developed for **CSE 342: IoT Based Project Development**, School of Science, Engineering and Technology, **East Delta University**.

- Md. Abrar Hossain 
- Hrishika Dhar Tisha
- Saiada Tun Nesa

## References

- SriTu Hobby, *"How to Make a Plant Watering System with ESP32 Board and Blynk App"* — [srituhobby.com](https://srituhobby.com/how-to-make-a-plant-watering-system-with-esp32-board-and-blynk-app/)

## License

This project is open-sourced under the [MIT License](LICENSE).
