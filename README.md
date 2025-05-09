# **Auxiliary System Control for Motorized Wheelchair**

This project implements a remote control system for motorized wheelchairs, providing a safe, efficient, and innovative mobility experience. It uses the **VESC 6.9 controller** to manage BLDC motors and integrates the **ESP32 microcontroller** for communication, real-time monitoring, and advanced safety features.

---

## **Index**
1. [Overview](#overview)  
2. [Features](#features)  
3. [Project Architecture](#project-architecture)  
4. [Installation Instructions](#installation-instructions)  
5. [Usage Guide](#usage-guide)  
6. [Code Structure](#code-structure)  
7. [Components and Materials Used](#components-and-materials-used)  
8. [Future Improvements](#future-improvements)  
9. [License](#license)

---

## **Overview**

The system is composed of hardware and software that work together to control the BLDC motor, monitor the power battery, and continuously provide feedback to the user. It supports control via **Bluetooth and USB**, displaying information on an **OLED display**, such as:
- Battery level  
- Current speed  
- Active operation mode  
- Operational efficiency status  

Additionally, it includes an **emergency module** that can quickly shut down the system in critical situations.

---

## **Features**

1. **Dynamic Speed Control**  
   - Speed adjustment using a rotary encoder  
   - Safe stop by pressing the encoder  

2. **Smart Operation Modes**  
   - **Inactive Mode**: System is disabled  
   - **Indoor Mode**: Speed limited for indoor environments  
   - **Outdoor Mode**: Increased power and speed for outdoor terrain  

3. **Real-Time Battery Monitoring**  
   - Detailed data on charge, voltage, current, and battery temperature via Bluetooth  

4. **Flexible Connectivity**  
   - Communication via Bluetooth or USB with automatic fallback  

5. **Visual and Audio Feedback**  
   - OLED display for system information  
   - Audible alerts for pairing, adjustments, and failures  

6. **Advanced Safety**  
   - Emergency module for instant shutdown  
   - Overload and overheating protection  

---

## **Project Architecture**

### **Hardware**
- **VESC 6.9 Controller**: Manages torque, current, and speed of the BLDC motor  
- **ESP32 DEVKITV1**: Microcontroller for processing and communication  
- **OLED Display (128x64 px)**: Visual interface for the user  
- **KY-040 Rotary Encoder**: Speed control  
- **Emergency Module**: Instantly shuts down the system in critical situations  
- **BMS (Battery Management System)**: Monitors the power battery  

### **Software**
- **ESP32 Firmware**: Handles communication between VESC, BMS, and remote control  
- **Key Libraries**:
  - `VescUart.cpp`: Manages UART communication with the VESC  
  - `datatypes.h`: Defines types and structures used by the firmware  
  - `u8g2`: Handles display management and its responsiveness  

---

## **Installation Instructions**

### **Requirements**
- Arduino IDE or PlatformIO installed  
- `esp32` board package added to the environment  
- Hardware wired according to the schematic  

### **Steps**
1. Clone this repository:
   ```bash
   git clone https://github.com/lgiorgenons/Izi-Mobile-Firmware
