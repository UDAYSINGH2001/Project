# Smart Garden System using STM32F446RE

## Project Overview
The **Smart Garden System** is an automated plant-watering and environmental monitoring solution using the **STM32F446RE** microcontroller. The system ensures efficient irrigation by monitoring soil moisture levels and automatically activating a **relay-controlled water pump** when needed. Additional environmental data, including **temperature and humidity (DHT11)**, is logged using a **DS1307 RTC** and displayed on an **LCD 16x2** screen. The project also includes **Bluetooth (HM10) communication**, enabling users to monitor sensor readings and control the water pump remotely via a smartphone.

## Features
- **Automatic Plant Watering:** Uses a **soil moisture sensor** to detect dryness and activate a **relay-controlled water pump**.
- **Environmental Monitoring:** Captures **temperature and humidity** using a **DHT11 sensor**.
- **Data Logging:** Logs time-stamped sensor data using a **DS1307 RTC**.
- **LCD Display:** Shows real-time moisture, temperature, and humidity values on an **LCD 16x2**.
- **Bluetooth Connectivity:** Enables remote monitoring and pump control via a smartphone using an **HM10 Bluetooth module**.
- **Manual Control via Bluetooth:** Users can send commands via a smartphone to manually turn the pump **ON or OFF**.

## Components Used
### 1. **Microcontroller:**
   - STM32F446RE

### 2. **Sensors:**
   - Soil Moisture Sensor (Analog)
   - DHT11 (Temperature & Humidity Sensor)
   - DS1307 (Real-Time Clock)

### 3. **Actuators & Modules:**
   - Relay Module (to control the pump)
   - HM10 Bluetooth Module
   - LCD 16x2 with I2C Module

### 4. **Other Components:**
   - Jumper Wires
   - 5V Water Pump
   - Power Supply (5V/12V as required)

## Pin Connections
| Component | STM32F446RE Pin |
|-----------|---------------|
| Soil Moisture Sensor (Analog) | PA1 (ADC1) |
| DHT11 Data Pin | PA8 |
| DS1307 RTC SDA | PB7 (I2C1_SDA) |
| DS1307 RTC SCL | PB6 (I2C1_SCL) |
| LCD I2C SDA | PB7 (I2C1_SDA) |
| LCD I2C SCL | PB6 (I2C1_SCL) |
| Relay (Water Pump Control) | PB0 |
| HM10 TX | PA3 (USART2_RX) |
| HM10 RX | PA2 (USART2_TX) |

## Software Requirements
- **STM32CubeIDE** (for writing and compiling code)
- **Keil uVision (Optional)**
- **ST-Link Utility** (for flashing the firmware)
- **Android App for Bluetooth Control (e.g., Serial Bluetooth Terminal)**

## Installation & Setup
### 1. Clone the repository:
```sh
git clone https://github.com/yourusername/Smart-Garden-STM32.git
cd Smart-Garden-STM32
```
### 2. Open the project in STM32CubeIDE and build it.
### 3. Flash the firmware to STM32F446RE using ST-Link Utility.
### 4. Power the system and connect to the **HM10 Bluetooth module** using a smartphone.

## Bluetooth Commands
| Command | Action |
|---------|--------|
| `W` | Turn ON the water pump |
| `O` | Turn OFF the water pump |

## How It Works
1. The **STM32F446RE** reads the soil moisture level via **ADC**.
2. If the moisture level is below a threshold, the **relay activates** the pump.
3. The **DHT11 sensor** measures **temperature and humidity**.
4. The **DS1307 RTC** logs irrigation time and sensor readings.
5. The data is displayed on an **LCD 16x2** and sent via **Bluetooth (HM10)**.
6. Users can control the **water pump manually** using a smartphone app.

## Future Improvements
- Adding **IoT connectivity** for remote monitoring via the Internet.
- Integrating a **mobile app** for better UI/UX control.
- Using a **solar-powered battery system** for energy efficiency.

---
### 📌 *Feel free to contribute to the project and improve its functionality!*

