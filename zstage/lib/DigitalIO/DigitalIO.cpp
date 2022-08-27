/**
 * @file DigitalIO.cpp
 * @author John Marangola (marangol@bc.edu)
 * @brief Base class implementation for interfacing with hardware IO
 * @version 0.1
 * @date 2022-03-19
 */

#include "DigitalIO.h"

int8_t DigitalIO::getPin() {
    return pin;
}

void DigitalIO::initPullup() {
    if (pin != E_GPIO_NOT_DEF)
        pinMode(pin, INPUT_PULLUP);
}

int8_t DigitalIO::getState() {
    int8_t tmp = digitalRead(pin);
    if (mode == digitalMode::NC) 
        return (tmp == LOW);
    else 
        return (tmp == HIGH);
}

bool DigitalIO::checkState(int8_t *newState) {
    if (pin == E_GPIO_NOT_DEF) {
        Serial.println("<ERROR> DigitalIO Pin UNDEF");
        return false;
    }
    int8_t state = getState();
    if (state != lastState) {
        lastState = state;
        *newState = state;
        return true;
    }
    return false;
}
