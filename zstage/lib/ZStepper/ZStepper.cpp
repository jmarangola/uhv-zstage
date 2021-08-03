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
#include <math.h>

/**
 * @brief Construct a new ZStepper::ZStepper object
 * 
 * @param step_p Step pin
 * @param dir_p Direction pin
 * @param enable_p Enable pin
 */
ZStepper::ZStepper(uint8_t step_p, uint8_t dir_p, uint8_t enable_p) {
    step_pin = step_p;
    dir_pin = dir_p;
    enable_pin = enable_p;
    stepper = AccelStepper(AccelStepper::FULL2WIRE, step_pin, dir_pin);
    enable_status = true;
    //stepper.setMinPulseWidth(100);
    stepper.setMaxSpeed(MAXIMUM_SPEED);
    stepper.setAcceleration(MAXIMUM_ACC);
    if (INVERT_DIRECTION)
        stepper.setPinsInverted(true, false, false);
}

/**
 * @brief Sets the enable status of the stepper motor
 * 
 * @param is_enabled 
 */
void ZStepper::set_enable(bool is_enabled) {
    //digitalWrite(enable_pin, (is_enabled ? LOW : HIGH));
    enable_status = is_enabled;
}

/**
 * @brief Returns the enable status of the motor
 * 
 * @return true 
 * @return false 
 */
bool ZStepper::get_enable() {
    return enable_status;
}

/**
 * @brief Runs the stepper at a constant speed to a positional step index
 * 
 * @param speed 
 * @param position 
 */
void ZStepper::runZStage(int speed, int position) {
    stepper.setSpeed(speed);
    while (stepper.currentPosition() != position)
        stepper.runSpeed();
}

/**
 * @brief Applies a step-based, trapezoidal motion profile to the stepper motor to increase maximum angular speed 
 * as well as prevent motor burnout. Routine is bi-directional.
 * 
 * @param initial_velocity Initial velocity of stepper
 * @param abs_max_vel Absolute maximum velocity 
 * @param delta_ramp_speed 
 * @param delta_ramp_steps 
 * @param position 
 * 
 *                 "flat"
 *                _ _ _ _ 
 *              /         \ <-- ramp down
 * ramp up --> /           \ 
 * 
 **/
void ZStepper::rampZStage(int initial_velocity, int abs_max_vel, int delta_ramp_speed, int delta_ramp_steps, long position) {   
    const long flat_magnitude = abs(position - ((abs_max_vel - initial_velocity) / delta_ramp_speed) * delta_ramp_steps);
    long last_position = stepper.currentPosition();
    int speed = initial_velocity;
    stepper.setSpeed(speed);
    Serial.println("Pre enter");
    // Linearly ramp up stepper to abs_max_vel from initial_velocity
    while (true) { 
        if (abs(stepper.currentPosition() - last_position) >= delta_ramp_steps) {
            speed += delta_ramp_speed;
            last_position = stepper.currentPosition();
            stepper.setSpeed(speed);
            Serial.println(speed);
            if (speed == abs_max_vel) 
                break;
        }
        stepper.runSpeed();
    }
    // Run stepper at constant maximium speed ("flat" portion of trapezoidal motion profile)
    last_position = stepper.currentPosition();
    while (true) {
        if (abs(stepper.currentPosition() - last_position) >= flat_magnitude) 
            break; 
        stepper.runSpeed();
    }
    last_position = stepper.currentPosition();
    speed = abs_max_vel;
    long current_position;
    // Linearly ramp down stepper to initial speed 
    while (true) { 
        current_position = stepper.currentPosition();
        if (abs(current_position - last_position) >= delta_ramp_steps) {
            speed -= delta_ramp_speed;
            if (speed == initial_velocity) 
                break;
            last_position = current_position;
            stepper.setSpeed(speed);    
        }
        stepper.runSpeed();
    }
}