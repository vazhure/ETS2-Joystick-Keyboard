/// Joystick or Keyboard for Euro Truck Simulator 2 / American Truck Simulator
/// Based on Arduino Pro Micro
/// Programmer: USBTinyISP
/// Board: Arduino Leonardo gSender
/// libraries:
/// ArduinoJoystickLibrary
/// https://github.com/MHeironimus/ArduinoJoystickLibrary
/// Keyboard
/// https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
///

// Note: Pins are grounded when they are pressed

#define USE_KEYBOARD  // comment this line if u want act as Joystick

enum BTN_TYPE { ACTUAL_PUSH_STATE,
                CLICK_DELAY_CLICK,
                CLICK_ON_PUSH,
                CLICK_ON_RELEASE,
                CLICK_ON_PUSH_RELEASE };

#define READ_PIN(pin) pin >= A0 ? !(analogRead(pin) > 200) : !digitalRead(pin)
// Number of buttons
#define PIN_CNT 18
// Delay between push and release
#define CLICK_DELAY 100
// User input deley to detect short push
#define SHORT_PUSH_MS 500
// Delay used to turn off blinker (lazy turn indicator mode)
#define RELEASE_DELAY_TIME 5000

#ifdef USE_KEYBOARD
#include <Keyboard.h>
#define SET_BUTTON(key, value) value ? Keyboard.press(key) : Keyboard.release(key)
#else
#include <Joystick.h>
#define SET_BUTTON(idx, value) Joystick.setButton(idx, value)
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   PIN_CNT, 0,            // Button Count, Hat Switch Count
                   false, false, false,   // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW 0xD7
#define KEY_UP_ARROW 0xDA
#define KEY_DOWN_ARROW 0xD9

#endif

struct Button {
  byte pin;
  Button *pSibling;
  BTN_TYPE type;
  int lastButtonState;
  int buttonState;
  bool _delay;
  unsigned long lastTime;
  uint16_t debounce;
  char key;

  inline void Debounce() {
    //    buttonState = READ_PIN(pin);
    int state = READ_PIN(pin);
    if (state) {
      if (debounce == 32768) buttonState = HIGH;
      else debounce <<= 1;
    } else {
      if (debounce == 1) buttonState = LOW;
      else debounce >>= 1;
    }
  }

  inline void Click() {
    SET_BUTTON(key, true);
    delay(CLICK_DELAY);
    SET_BUTTON(key, false);
  }

  void Process() {
    Debounce();
    switch (type) {
      case BTN_TYPE::CLICK_ON_PUSH_RELEASE:
        {
          if (lastButtonState != buttonState) {
            lastButtonState = buttonState;
            Click();
          }
        }
        break;
      case BTN_TYPE::CLICK_ON_PUSH:
        {
          if (lastButtonState != buttonState) {
            lastButtonState = buttonState;
            if (buttonState)
              Click();
          }
        }
        break;
      case BTN_TYPE::CLICK_ON_RELEASE:
        {
          if (lastButtonState != buttonState) {
            lastButtonState = buttonState;
            if (!buttonState)
              Click();
          }
        }
        break;
      case BTN_TYPE::CLICK_DELAY_CLICK:
        {
          if (_delay && !buttonState) {
            if (millis() - lastTime >= RELEASE_DELAY_TIME) {
              lastButtonState = buttonState;
              Click();
              _delay = false;
            }
            return;
          }

          if (lastButtonState != buttonState) {
            lastButtonState = buttonState;
            if (buttonState > 0)  // push
            {
              if (!_delay) {
                lastTime = millis();
                _delay = false;
                if (pSibling != NULL)
                  pSibling->_delay = false;  // reset oposite blinker off timer
                Click();
              }
            } else  // release
            {
              if (lastTime == 0) {
                SET_BUTTON(key, false);
                return;
              }

              if (millis() - lastTime <= SHORT_PUSH_MS) {
                _delay = true;
                lastTime = millis();
              } else {
                if (!_delay) {
                  Click();
                }
              }
            }
          }
        }
        break;
      default:
        {
          if (lastButtonState != buttonState) {
            lastButtonState = buttonState;
            SET_BUTTON(key, buttonState);
          }
        }
        break;
    }
  }
};

// Buttons declaration
Button btns[] = {
  { 2, NULL, BTN_TYPE::CLICK_DELAY_CLICK, 0, 0, false, 0, 1, '[' },                    // left blinker
  { 3, NULL, BTN_TYPE::CLICK_DELAY_CLICK, 0, 0, false, 0, 1, ']' },                    // right blinker
  { 4, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, 'l' },                    // light mode
  { 5, NULL, BTN_TYPE::CLICK_ON_PUSH_RELEASE, 0, 0, false, 0, 1, 'k' },                // high beam
  { 6, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, 'p' },                    // wipers mode
  { 7, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, 'w' },                    // rear wipers
  { 8, NULL, BTN_TYPE::CLICK_ON_PUSH, 0, 0, false, 0, 1, 'r' },                        // Reverse gear
  { 9, NULL, BTN_TYPE::CLICK_ON_PUSH, 0, 0, false, 0, 1, 'n' },                        // Neutral
  { 10, NULL, BTN_TYPE::CLICK_ON_PUSH, 0, 0, false, 0, 1, 'd' },                       // Drive
  { 14, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, (char)KEY_UP_ARROW },    // shift up
  { 15, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, (char)KEY_DOWN_ARROW },  // shift down
  { 16, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, ' ' },                   // Parking Brake
  { A0, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, 'e' },                   // Engine
  { A1, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, 't' },                   // Trailer
  { A2, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, 'f' },                   // Hazard warning
  { A3, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, 'o' },                   // Beacon
  { 0, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, (char)KEY_LEFT_ARROW },   // extra 1 (RX)
  { 1, NULL, BTN_TYPE::ACTUAL_PUSH_STATE, 0, 0, false, 0, 1, (char)KEY_RIGHT_ARROW },  // extra 2 (TX)
};

// Initialization
void setup() {
  // Initialize Button Pins
  for (int t = 0; t < PIN_CNT; t++)
    pinMode(btns[t].pin, INPUT_PULLUP);

  // Turn indicators cancel each other
  btns[0].pSibling = &btns[1];
  btns[1].pSibling = &btns[0];

#ifdef USE_KEYBOARD
  // Initialize Keyboard Library
  Keyboard.begin();
#else
  // Initialize Joystick Library
  Joystick.begin();
  // Replace key code to btn index
  for (int t = 0; t < PIN_CNT; t++)
    btns[t].key = t;
#endif
}

// Main loop
void loop() {
  // Processing buttons
  for (int index = 0; index < PIN_CNT; index++) {
    btns[index].Process();
  }
  delay(1);
}