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
#include "EEPROM.h"

ZStepper zstage(6, 4, 7);
const int8_t lowerLimitPin = 13;
const int8_t upperLimitPin = 9;
const int n_positions = 3;
const int8_t positionButtons[n_positions] = {1, 2, 3};
const int8_t targetPositions[n_positions] = {1, 2, 3};
const int8_t extendButton = 19;
const int8_t retractButton = 21;
const int8_t okButton = 23;
bool errorState = false;

/**
 * @brief Home the linear translator to the limit switch efficiently, setting positional index to 0.
 * 
 */
void home_axis() {
  // Run quickly to limit
  zstage.stepper.setSpeed(7000);
  while (digitalRead(lowerLimitPin)) {
    zstage.stepper.runSpeed();
  }
  zstage.stepper.setCurrentPosition(0);
  zstage.stepper.runToNewPosition(-7500);
  zstage.stepper.stop();
  // Back off of the lower limit by about a tiny amount and reapproach at slower speed to optimize accuracy
  zstage.stepper.setSpeed(3000);
  while (digitalRead(lowerLimitPin)) {
    zstage.stepper.runSpeed();
  }
  zstage.stepper.setCurrentPosition(0);
}

/**
 * @brief Checks whether a new Arduino board was inserted into the system.
 * 
 * @return true : new board which needs to be initialized
 * @return false : old board
 */
bool checkNewEEPROM() {
  for (int byte = 0; byte < 512; byte++) {
    if (EEPROM.read(byte) != 255) 
      return false;
  }
  return true;
}

/**
 * @brief Determine whether the arduino was safely powered off last or whether it lost power due
 * to an unexpected power cycle or loss of power
 * 
 * @return true : Safely powered off -- Safe to begin homing routine
 * @return false : Unaccounted loss of power -- Cleaver could be extended and long manipulator down
 */
bool safelyPoweredOff() {
  for (int byte = 0; byte < 512; byte++) {
    if (EEPROM.read(byte) == 0) {
      if (EEPROM.read(++byte) == 64) // Safe power down or power cycle (position should be approximately homed and safe to home immediately)
        return true;
      else // Failsafe shutdown/power cycle reboot
        return false;
    }
  }
}

/**
 * @brief Set a random EEPROM address to the failsafe value with a leading signature bit
 * Randomization to avoid EEPROM burnout of 100k EEPROM writes/byte 
 * 
 */
void initializeEEPROM() {
  int addr = random(0, 512);
  EEPROM.update(addr - 1, 0);
  EEPROM.update(addr, 8);
}

/**
 * @brief Arduino initialization
 * 
 */
void setup() {
  pinMode(lowerLimitPin, INPUT_PULLUP); // Pullup lower limit switch
  pinMode(upperLimitPin, INPUT_PULLUP); // Pullup upper limit switch
  for (int i = 0; i < n_positions; i++) pinMode(positionButtons[i], INPUT_PULLUP);
  pinMode(okButton, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("[Serial Communication Opened]");
  home_axis();
  //zstage.runZStage(-8000, -300000);
    Serial.println("enter");
  zstage.rampZStage(-4000, -8500, -500, 500, -15000);

}

/**
 * @brief Main execution loop
 * 
 */
void loop() {}
