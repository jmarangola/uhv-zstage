/**
 * @file ZStepper.cpp
 * @author John Marangola (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-07-27
 * 
 * @copyright Copyright (c) 2021
 * 
 * local
 */

#include "ZStepper.h"

ZStepper::ZStepper(uint8_t step_p, uint8_t dir_p, uint8_t enable_p) {
    step_pin = step_p;
    dir_pin = dir_p;
    enable_pin = enable_p;
    stepper = AccelStepper(AccelStepper::FULL2WIRE, step_pin, dir_pin);
    enable_status = true;
    stepper.setMaxSpeed(MAXIMUM_SPEED);
    stepper.setAcceleration(MAXIMUM_ACC);
    if (INVERT_DIRECTION)
        stepper.setPinsInverted(true, false, false);
}

void ZStepper::set_enable(bool is_enabled) {
    //digitalWrite(enable_pin, (is_enabled ? LOW : HIGH));
    enable_status = is_enabled;
}

bool ZStepper::get_enable() {
    return enable_status;
}
