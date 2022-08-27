/**
 * @file zEEPROM.cpp
 * @author John Marangola (you@domain.com)
 * @brief IO functions for accessing and manipulating EEPROM memory
 * @version 0.1
 * @date 2022-03-19
 * 
 * @copyright Copyright (c) 2022
 * 
 * local
 */
#include "zEEPROM.h"

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

// /**
//  * @brief Set a random EEPROM address to the failsafe value with a leading signature bit
//  * Randomization to avoid EEPROM burnout of 100k EEPROM writes/byte 
//  * 
//  */
// void initializeEEPROM() {
//   // int addr = random(0, 512);
//   // EEPROM.update(addr - 1, 0);
//   // EEPROM.update(addr, 8);
// }