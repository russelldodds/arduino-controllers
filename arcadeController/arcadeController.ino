/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include <bluefruit.h>
#include <ButtonDebounce.h>
#include "keyboardHex.h"

BLEDis bledis;
BLEHidAdafruit blehid;

#define BOUNCE_DELAY 10
#define LED_PIN LED_BUILTIN // for flashing the light

// do NOT use Po.20, Po.22, Po.31
// create a define for each button/controller
#define BUTTON_1 2 // use the purple nomenclature for pins
ButtonDebounce button_1(BUTTON_1, BOUNCE_DELAY);

#define BUTTON_2 3 // use the purple nomenclature for pins
ButtonDebounce button_2(BUTTON_2, BOUNCE_DELAY);

#define BUTTON_3 4 // use the purple nomenclature for pins
ButtonDebounce button_3(BUTTON_3, BOUNCE_DELAY);

#define BUTTON_4 5 // use the purple nomenclature for pins
ButtonDebounce button_4(BUTTON_4, BOUNCE_DELAY);

#define BUTTON_5 12 // use the purple nomenclature for pins
ButtonDebounce button_5(BUTTON_5, BOUNCE_DELAY);

#define BUTTON_6 13 // use the purple nomenclature for pins
ButtonDebounce button_6(BUTTON_6, BOUNCE_DELAY);

#define BUTTON_JOYSTICK 14 // use the purple nomenclature for pins
ButtonDebounce button_joystick(BUTTON_JOYSTICK, BOUNCE_DELAY);



#define JOYSTICK_LEFT 16 // use the purple nomenclature for pins
ButtonDebounce joystick_left(JOYSTICK_LEFT, BOUNCE_DELAY);

#define JOYSTICK_UP 15 // use the purple nomenclature for pins
ButtonDebounce joystick_up(JOYSTICK_UP, BOUNCE_DELAY);

#define JOYSTICK_RIGHT 7 // use the purple nomenclature for pins
ButtonDebounce joystick_right(JOYSTICK_RIGHT, BOUNCE_DELAY);

#define JOYSTICK_DOWN 11 // use the purple nomenclature for pins
ButtonDebounce joystick_down(JOYSTICK_DOWN, BOUNCE_DELAY);


#define FLIPPER_LEFT 27 // use the purple nomenclature for pins
ButtonDebounce flipper_left(FLIPPER_LEFT, BOUNCE_DELAY);

#define FLIPPER_RIGHT 26 // use the purple nomenclature for pins
ButtonDebounce flipper_right(FLIPPER_RIGHT, BOUNCE_DELAY);

void setup() 
{
  Serial.begin(115200);

  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Bluefruit52");

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather 52");
  bledis.begin();

  /* Start BLE HID
   * Note: Apple requires BLE device must have min connection interval >= 20m
   * ( The smaller the connection interval the faster we could send data).
   * However for HID and MIDI device, Apple could accept min connection interval 
   * up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
   * connection interval to 11.25  ms and 15 ms respectively for best performance.
   */
  blehid.begin();

  /* Set connection interval (min, max) to your perferred value.
   * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
   * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms 
   */
  /* Bluefruit.setConnInterval(9, 12); */

  // Set up and start advertising
  startAdv();

  // setup the led
  pinMode(LED_BUILTIN, OUTPUT);

  // set up each button
  pinMode(BUTTON_1, INPUT_PULLUP);
  button_1.setCallback(button1Change);
  pinMode(BUTTON_2, INPUT_PULLUP);
  button_2.setCallback(button2Change);
  pinMode(BUTTON_3, INPUT_PULLUP);
  button_3.setCallback(button3Change);
  pinMode(BUTTON_4, INPUT_PULLUP);
  button_4.setCallback(button4Change);
  pinMode(BUTTON_5, INPUT_PULLUP);
  button_5.setCallback(button5Change);
  pinMode(BUTTON_6, INPUT_PULLUP);
  button_6.setCallback(button6Change);
  pinMode(BUTTON_JOYSTICK, INPUT_PULLUP);
  button_joystick.setCallback(buttonJoystickChange);

  pinMode(JOYSTICK_LEFT, INPUT_PULLUP);
  joystick_left.setCallback(joystickLeftChange);
  pinMode(JOYSTICK_UP, INPUT_PULLUP);
  joystick_up.setCallback(joystickUpChange);
  pinMode(JOYSTICK_RIGHT, INPUT_PULLUP);
  joystick_right.setCallback(joystickRightChange);
  pinMode(JOYSTICK_DOWN, INPUT_PULLUP);
  joystick_down.setCallback(joystickDownChange);

  pinMode(FLIPPER_LEFT, INPUT_PULLUP);
  flipper_left.setCallback(flipperLeftChange);
  pinMode(FLIPPER_RIGHT, INPUT_PULLUP);
  flipper_right.setCallback(flipperRightChange);

}

void startAdv(void)
{  
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  
  // Include BLE HID service
  Bluefruit.Advertising.addService(blehid);

  // There is enough room for the dev name in the advertising packet
  Bluefruit.Advertising.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void loop() 
{
  // check for updates
  button_1.update();
  button_2.update();
  button_3.update();
  button_4.update();
  button_5.update();
  button_6.update();
  button_joystick.update();

  joystick_left.update();
  joystick_up.update();
  joystick_right.update();
  joystick_down.update();

  flipper_left.update();
  flipper_right.update();

  // Request CPU to enter low-power mode until an event/interrupt occurs
  waitForEvent();  
}

// call back for button state change
void button1Change(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_1);
  }
}

void button2Change(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_2);
  }
}

void button3Change(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_5);
  }
}

void button4Change(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_LEFT_CTRL);
  }
}

void button5Change(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_LEFT_ALT);
  }
}

void button6Change(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_SPACE);
  }
}

void buttonJoystickChange(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_TAB);
  }
}

void joystickLeftChange(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_LEFT);
  }
}

void joystickUpChange(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_UP);
  }
}

void joystickRightChange(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_RIGHT);
  }
}

void joystickDownChange(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_DOWN);
  }
}

void flipperLeftChange(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_A);
  }
}

void flipperRightChange(int state) {
  digitalWrite(LED_PIN, !state );
  if (state) {
    blehid.keyRelease();
  } else {
    blehid.keyPress(KEY_B);
  }
}

/**
 * RTOS Idle callback is automatically invoked by FreeRTOS
 * when there are no active threads. E.g when loop() calls delay() and
 * there is no bluetooth or hw event. This is the ideal place to handle
 * background data.
 * 
 * NOTE: FreeRTOS is configured as tickless idle mode. After this callback
 * is executed, if there is time, freeRTOS kernel will go into low power mode.
 * Therefore waitForEvent() should not be called in this callback.
 * http://www.freertos.org/low-power-tickless-rtos.html
 * 
 * WARNING: This function MUST NOT call any blocking FreeRTOS API 
 * such as delay(), xSemaphoreTake() etc ... for more information
 * http://www.freertos.org/a00016.html
 */
void rtos_idle_callback(void)
{
  // Don't call any other FreeRTOS blocking API()
  // Perform background task(s) here
}
