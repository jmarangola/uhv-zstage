#pragma once

#include <Arduino.h>
#include <AccelStepper.h>
#include <stdint.h>

class ZStepper {
    public:
        ZStepper(uint8_t step_p, uint8_t dir_pin, uint8_t enable_pin);
        void set_enable(bool enable);
        bool get_enable();
        AccelStepper stepper;
    private:
        uint8_t step_pin;
        uint8_t dir_pin;
        uint8_t enable_pin;
        bool enable_status;
        const long MAXIMUM_SPEED = 500000;
        const long MAXIMUM_ACC = 5000000;
        const bool INVERT_DIRECTION = true;
};