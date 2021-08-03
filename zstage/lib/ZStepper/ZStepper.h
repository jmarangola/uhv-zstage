#pragma once

#include <Arduino.h>
#include <AccelStepper.h>
#include <stdint.h>

class ZStepper {
    public:
        ZStepper(uint8_t step_p, uint8_t dir_pin, uint8_t enable_pin);
        void set_enable(bool enable);
        bool get_enable();
        void runZStage(int speed, int position);
        void rampZStage(int initial_speed, int max_speed, int delta_ramp_time, int delta_ramp_speed, long position);
        AccelStepper stepper;
    private:
        uint8_t step_pin;
        uint8_t dir_pin;
        uint8_t enable_pin;
        bool enable_status;
        const float MAXIMUM_SPEED = 9000000;
        const float MAXIMUM_ACC = 5000000;
        const bool INVERT_DIRECTION = true;
};