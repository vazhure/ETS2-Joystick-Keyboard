# 🚛 ETS2 / ATS Custom Controller (Arduino Pro Micro)

A custom macro pad / shifter controller designed specifically for **Euro Truck Simulator 2 (ETS2)** and **American Truck Simulator (ATS)**. 
Based on the **Arduino Pro Micro** (ATmega32U4), this project features advanced logic like "lazy" turn signal cancellation and sequential wiper speed simulation.

## ✨ Features
- **Dual Mode:** Can act as a standard Keyboard or a virtual Gamepad (Joystick).
- **Lazy Turn Signals:** Blinkers automatically turn off after 5 seconds or when the opposite blinker is pressed.
- **Wiper Speed Logic:** Simulates ETS2 wiper speeds by sending sequential forward/back pulses based on the selected speed button.
- **Debounce Logic:** Built-in software debouncing to prevent signal ghosting.

---

## ⚙️ 1. Mode Selection: Keyboard vs. Joystick

The firmware supports two distinct operating modes. You can switch between them by editing a single line at the top of the `JoystickKeyboard.ino` file.

### ⌨️ Keyboard Mode (Default)
- **How to enable:** Ensure `#define USE_KEYBOARD` is **uncommented**.
- **Behavior:** The board acts as a standard USB Keyboard. It sends keystrokes (e.g., `[`, `]`, `Space`, `Arrows`).
- **Best for:** Users who prefer direct keybindings in the game settings without configuring virtual controllers.

### 🎮 Joystick Mode
- **How to enable:** **Comment out** the line: `//#define USE_KEYBOARD`
- **Behavior:** The board acts as a Gamepad (Joystick) with 18 buttons. 
- **Requirements:** Requires the [ArduinoJoystickLibrary by MHeironimus](https://github.com/MHeironimus/ArduinoJoystickLibrary).
- **Best for:** Native controller support in ETS2/ATS, allowing you to map buttons directly in the "Controllers" menu.

*(Optional)* If you are using Joystick mode and want the wiper speed buttons to act as standard separate buttons (useful for games like Assetto Corsa), uncomment `#define ENABLE_WIPER_SPEED_BTNS`.

---

## 🔌 2. Wiring Diagram & Pinout Scheme

**Important Wiring Rule:** The code uses internal pull-up resistors (`INPUT_PULLUP`). 
**Switches must be connected between the Arduino PIN and GND.** When the button is pressed, it grounds the pin (Active LOW). Do NOT connect buttons to 5V/VCC.

| Index | Arduino Pin | Function (ETS2/ATS) | Keyboard Key | Joystick Btn |
| :--- | :--- | :--- | :--- | :--- |
| **0** | D2 | Right Blinker | `]` | 0 |
| **1** | D3 | Left Blinker | `[` | 1 |
| **2** | D4 | High Beam (Pull) | `k` | 2 |
| **3** | D5 | High Beam (Push) | `k` | 3 |
| **4** | D0 (RXI) | Wipers Forward | `p` | 4 |
| **5** | D7 | Wipers Backward | `w` | 5 |
| **6** | D8 | Wiper Speed 1 | *Macro* | 6 |
| **7** | D9 | Wiper Speed 2 | *Macro* | 7 |
| **8** | D1 (TXI) | Wiper Speed 3 | *Macro* | 8 |
| **9** | D14 (MISO)| Left Window Down | `Left Arrow` | 9 |
| **10** | D15 (SCK) | Left Window Up | `Down Arrow`* | 10 |
| **11** | D6 | Parking Brake | `Space` | 11 |
| **12** | A0 | Right Window Down | `Right Arrow`| 12 |
| **13** | D10 | Right Window Up | `Up Arrow` | 13 |
| **14** | A1 | Trailer Attach | `t` | 14 |
| **15** | A2 | Hazard Warning | `f` | 15 |
| **16** | A3 | Beacon Lights | `o` | 16 |
| **17** | D16 (MOSI)| Extra 1 | `e` | 17 |

*\*Note: Index 10 maps to Left Window UP but sends the Down Arrow key in the default code. You can swap the keys in the `btns[]` array if your physical wiring differs.*

---

## 🛠️ 3. Troubleshooting (Errors & Solutions)

### ❌ Error: "Board not recognized" or "Upload failed"
* **Cause:** Pro Micro boards often lack native drivers on Windows, or the COM port drops during upload.
* **Solution:** 
  1. Install the **SparkFun Pro Micro drivers**.
  2. In Arduino IDE, select Board: `Arduino Leonardo` (Pro Micro uses the same ATmega32U4 chip).
  3. **Double-tap the RESET button** on the Pro Micro right before clicking "Upload" in the IDE to force it into bootloader mode.

### ❌ Error: Buttons are inverted (Pressed when released, released when pressed)
* **Cause:** Incorrect wiring.
* **Solution:** Ensure your switches are wired between the **Signal Pin** and **GND**. If you wired them to 5V, change `INPUT_PULLUP` to `INPUT` and add external pull-down resistors (not recommended).

### ❌ Error: Joystick doesn't show up in Windows / Game
* **Cause:** Missing library or wrong mode selected.
* **Solution:** 
  1. Ensure `#define USE_KEYBOARD` is commented out (`//`).
  2. Download and install the [ArduinoJoystickLibrary](https://github.com/MHeironimus/ArduinoJoystickLibrary).
  3. Check Windows "Set up USB game controllers" to verify the device is recognized.

### ❌ Error: Keyboard spamming or Ghosting (Multiple presses)
* **Cause:** Mechanical switch bounce exceeds the software debounce threshold.
* **Solution:** The code includes a shifting debounce algorithm. If issues persist, check your physical wiring for loose ground connections, or slightly increase the `CLICK_DELAY` value in the code.

### ❌ Error: Compilation Error "Joystick.h not found"
* **Cause:** The MHeironimus library is not installed.
* **Solution:** Go to Sketch -> Include Library -> Manage Libraries -> Search for `Joystick` and install the one by *Matthew Heironimus*.

---

## 📥 Installation Guide
1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install the required libraries (Keyboard is built-in, Joystick must be downloaded).
3. Copy the `.ino` code provided below into your Arduino IDE.
4. Select **Board:** `Arduino Leonardo` and choose the correct **COM Port**.
5. Upload the code.
6. Map the buttons inside ETS2 / ATS settings!

*Happy Trucking! 🚚💨*
