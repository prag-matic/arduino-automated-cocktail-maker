# 🍹 Automated Cocktail Maker (Arduino)

> An Arduino-based automated cocktail-making machine designed to dispense consistent and accurate mocktails using pumps, relays, sensors, and a simple menu-driven user interface.  
> This project combines hardware + embedded software to automate drink preparation while preventing spills using cup detection.

---

## 📌 Features

- **Menu-based selection** using 16x2 I2C LCD
- **Two-button navigation** system (Next / Select)
- Supports **multiple flavors**
- Supports **different drink sizes**
- **Ultrasonic cup detection** (prevents dispensing without a cup)
- **Relay-controlled water pumps**
- Visual **LED indicator** for cup detection
- **Consistent dispensing** using timed pump control

---

## 🧠 System Overview

### Workflow
1. User powers on the system  
2. Selects flavor  
3. Selects drink size  
4. System checks for cup presence  
5. Dispenses ingredients automatically  
6. Displays completion message  
7. Returns to main menu  

### 🖼 System Architecture Diagram
![project_diagram] (diagrams/Diagram for project.png)

---

## 🔧 Hardware Components

| Component | Description |
|-----------|-------------|
| Arduino UNO R3 | Main microcontroller |
| 6V–12V DC Water Pumps | For liquid dispensing |
| 4-Channel Relay Module | Controls pumps |
| HC-SR04 Ultrasonic Distance Sensor | Detects cup presence |
| 16x2 LCD Display with I2C Adapter | User interface display |
| Push Buttons (Next / Select) | Navigation input |
| 12V 2A SMPS Power Supply | Powers pumps and system |
| Buck Converter (12V → 5V) | Steps down voltage for Arduino |
| Silicone Tubes | Liquid channels |
| LED Indicator | Visual cup detection feedback |

---

## 🧪 Software & Tools

- Arduino IDE
- C / C++ (Embedded)
- `LiquidCrystal_I2C` Library

---

## 🔌 Pin Configuration

| Component | Arduino Pin |
|-----------|-------------|
| Next Button | D13 |
| Select Button | D12 |
| Relay 1 (Sprite) | D5 |
| Relay 2 (Apple) | D6 |
| Relay 3 (Strawberry/Guava) | D7 |
| Ultrasonic Trigger | D9 |
| Ultrasonic Echo | D10 |
| Cup Detection LED | D8 |

---

## 🧠 Key Logic Highlights

- **Debounced buttons** using `millis()`
- **Medium press** → cycle options
- **Long press** → confirm selection
- **Cup detection threshold**: ≤ 10 cm
- **Safety lock**: dispensing only starts if cup is detected

---

## 🚀 How to Run

1. Clone this repository
2. Open `src/cocktail_maker.ino` in Arduino IDE
3. Install required libraries
4. Upload to Arduino UNO
5. Power the system
6. Select flavor & size
7. Place cup and enjoy 🍹

---

## 🎓 Academic Context

Developed as part of:  
**IT1040 – Fundamentals of Computing**  
Sri Lanka Institute of Information Technology (SLIIT)  
Year 1, Semester 1

---

## 👥 Team Members

- Perera B.P.N. (IT24100327)
- Pathberiya H.A.
- Pabasara H.E.T.
- De Silva L.J.
- V. Thuvarkan
- Munasinghe T.H.

---

## 📜 License

This project is licensed under the **MIT License** – feel free to use and modify with attribution.
