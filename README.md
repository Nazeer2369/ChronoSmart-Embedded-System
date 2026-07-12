# ⏰ ChronoSmart

A **Real-Time RTC-Based Appliance Automation & Temperature Monitoring System** developed using the **LPC2148 ARM7 Microcontroller**. The project automates appliance control based on user-defined schedules while continuously monitoring ambient temperature using the **LM35 temperature sensor**.

---

# 📖 Overview

**ChronoSmart** is a real-time embedded automation system that combines **Real-Time Clock (RTC)** scheduling with **temperature monitoring** to automate household or industrial appliances.

The system allows users to configure ON/OFF timings for two independent devices through a secure password-protected menu. It continuously displays the current date, time, and temperature on a 16×2 LCD while automatically controlling connected devices based on the configured schedule.

The project demonstrates practical implementation of **RTC**, **ADC**, **GPIO**, **External Interrupts**, **Keypad Interfacing**, and **Embedded C Firmware Development** on the LPC2148 ARM7 microcontroller.

---

# ✨ Features

## ⏰ RTC-Based Appliance Automation

- Automatic ON/OFF scheduling for two independent devices
- Real-time clock synchronization
- Individual scheduling for each device
- Continuous RTC display on LCD

## 🌡 Temperature Monitoring

- Real-time temperature measurement using LM35
- ADC-based temperature conversion
- Live temperature display on LCD
- Configurable temperature threshold

## 🔐 Secure Configuration

- Password-protected settings menu
- Interrupt-driven configuration mode
- Edit RTC date and time
- Modify device schedules
- Change password
- Enable/Disable devices

## 🛡 Security Features

- Password authentication
- Three incorrect password attempt protection
- Temporary system lock after failed attempts
- LCD status notifications
- Buzzer indication for security alerts

---

# 🏗 System Architecture

## Main Controller

- Reads RTC information
- Monitors temperature through ADC
- Displays date, time, and temperature
- Controls connected appliances
- Handles user requests

## User Interface

- 16×2 LCD Display
- 4×4 Matrix Keypad
- Interrupt-based configuration switch
- Password authentication system

## Device Control

- Device-1 independent scheduling
- Device-2 independent scheduling
- GPIO-based control
- Automatic ON/OFF operation

---

# ⚙️ Hardware Components

- LPC2148 ARM7 Microcontroller
- 16×2 LCD Display
- 4×4 Matrix Keypad
- LM35 Temperature Sensor
- LEDs / Relay Modules
- Push Button (External Interrupt)
- Buzzer
- USB-to-UART Converter

---

# 💻 Software Tools

- Embedded C
- Keil µVision
- Flash Magic

---

# 🔌 Peripherals Used

## RTC

- Real-time clock management
- Date and time display
- Schedule comparison

## ADC

- LM35 temperature acquisition
- Temperature conversion
- Threshold monitoring

## GPIO

- LCD interfacing
- Device control
- Buzzer operation

## External Interrupt (EINT)

- Event-driven configuration mode
- User request handling

---

# 🔄 System Workflow

1. Initialize RTC, LCD, ADC, GPIO, Keypad, and External Interrupt.
2. Display current date, time, and temperature.
3. Continuously monitor RTC and temperature.
4. Compare current time with stored schedules.
5. Automatically switch Device-1 and Device-2 ON/OFF.
6. On interrupt request:
   - Prompt for password.
   - Validate user credentials.
   - Enter secure configuration mode.
   - Allow editing of RTC, schedules, password, and temperature threshold.
7. Save updated settings.
8. Return to continuous monitoring mode.

---

# 📊 Project Results

- ✅ Successfully implemented RTC-based appliance automation.
- ✅ Developed automatic scheduling for two independent devices.
- ✅ Implemented real-time temperature monitoring using LM35.
- ✅ Designed a secure password-protected configuration system.
- ✅ Integrated LCD, Keypad, ADC, RTC, GPIO, and External Interrupts.
- ✅ Achieved reliable real-time embedded system operation.

---

# 🚀 Future Enhancements

- EEPROM-based schedule storage
- GSM notification system
- Wi-Fi/IoT connectivity
- Mobile application control
- Multiple appliance support
- Energy consumption monitoring
- Cloud-based monitoring dashboard

---

# 🛠 Technologies Used

- ARM7 LPC2148
- Embedded C
- RTC
- ADC
- GPIO
- External Interrupts
- LCD Interfacing
- Matrix Keypad
- LM35 Temperature Sensor
- Keil µVision
- Flash Magic
