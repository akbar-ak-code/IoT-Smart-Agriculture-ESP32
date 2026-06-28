# IoT Smart Agriculture Monitoring System (ESP32)

A virtual Embedded Systems and IoT simulation that models an automated irrigation system. Built using an ESP32 microcontroller, this project simulates dynamic soil moisture evaporation and automated pump control, streaming real-time telemetry to a cloud dashboard.

##Features
* **Dynamic Moisture Simulation:** Algorithmic calculation of soil moisture based on simulated temperature and evaporation rates.
* **Automated Control Logic:** Hysteresis loop implementation (Pump activates at <30% moisture and deactivates at >80%).
* **Multi-Field Expansion:** Handles multiple environmental zones with independent drying rates.
* **Cloud Integration:** Real-time data logging and visualization using the ThingSpeak API.

## 🛠️ Tech Stack & Hardware (Virtual)
* **Microcontroller:** ESP32 (DevKit)
* **Sensors:** DHT22 (Temperature & Humidity)
* **Actuator:** Virtual Water Pump (LED indicator on GPIO 2)
* **Cloud Dashboard:** ThingSpeak
* **Simulation Environment:** Wokwi

## 📸 System Architecture & Dashboard
*(To do: Edit this file and drag-and-drop your Wokwi circuit screenshot here)*

*(To do: Edit this file and drag-and-drop your ThingSpeak graphs screenshot here)*

## ⚙️ How to Run the Simulation
1. Copy the `diagram.json` and `sketch.ino` files into a new [Wokwi ESP32 Project](https://wokwi.com/).
2. Ensure the `DHT sensor library` is added to the project via the Library Manager.
3. Replace the `TS_API_KEY` string in the code with your own ThingSpeak Write API Key.
4. Run the simulation and adjust the DHT22 temperature sliders to observe the dynamic evaporation and pump automation.
