#pragma once
#include <Arduino.h>
#include <EEPROM.h>

bool checkNewEEPROM();
bool safelyPoweredOff();
void initializeEEPROM();