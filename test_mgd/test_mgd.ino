#include <ButtonDebounce.h>

// do NOT use Po.20, Po.22, Po.31

#define BUTTON_1 2 // use the purple nomenclature for pins
ButtonDebounce button_1(BUTTON_1, 5);

#define LED_PIN LED_BUILTIN

void button_1Change(int state) {
  digitalWrite(LED_PIN, !state );
}


void setup() {
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  // set up each button
  pinMode(BUTTON_1, INPUT_PULLUP);
  button_1.setCallback( button_1Change );
}

void loop() {
  button_1.update();
}
