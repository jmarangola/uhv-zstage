/**
 * @file DigitalIO.h
 * @author John Marangola (you@domain.com)
 * @brief DigitalIO header file
 **/

#pragma once
#include <Arduino.h>
#include <cstdint>
#define E_GPIO_NOT_DEF 257

class DigitalIO {
    protected:
        int8_t pin;
        int8_t lastState = (mode == NC) ? 0 : 1; 
        int8_t getPin();
    public:
        enum digitalMode {NC = 0, NO = 1} mode;
        DigitalIO(uint8_t pin, digitalMode mode) : pin(pin), mode(mode) {}
        void initPullup();
        int8_t getState();
        bool checkState(int8_t *newState);
};