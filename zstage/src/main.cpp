/**
 * @file main.cpp
 * @author John Marangola (marangol@bc.edu)
 * @brief ZStage stepper motion implementation
 * @version 0.1
 * @date 2021-07-27
 * 
 * @copyright Copyright (c) 2021
 * 
 * local
 */

#include <Arduino.h>
#include <AccelStepper.h>
#include "ZStepper.h"

ZStepper zstage(6, 4, 7);
const int8_t limit_pin = 13;

/**
 * @brief Home the linear translator to the limit switch efficiently, setting positional index to 0.
 * 
 */
void home_axis() {
  // Run quickly to limit
  zstage.stepper.setSpeed(5000);
  while (digitalRead(limit_pin)) {
    zstage.stepper.runSpeed();
  }
  zstage.stepper.setCurrentPosition(0);
  zstage.stepper.runToNewPosition(-7500);
  zstage.stepper.stop();
  // Back off limit by about a quarter of an inch and reapproach at slower speed to optimize accuracy
  zstage.stepper.setSpeed(3000);
  while (digitalRead(limit_pin)) {
    zstage.stepper.runSpeed();
  }
  zstage.stepper.setCurrentPosition(0);
}

void setup() {
  pinMode(limit_pin, INPUT_PULLUP); // Pullup limit switch
  Serial.begin(9600);
  home_axis();
  zstage.set_enable(false);

}

void loop() {

}
