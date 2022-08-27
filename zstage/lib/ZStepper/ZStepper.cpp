/**
 * @file ZStepper.cpp
 * @author John Marangola (marangol@bc.edu)
 * @brief A custom motor control class for controlling stepper motors at high frequencies using hardware timers and interrupts
 */

#include "ZStepper.h"

/**
 * @brief Set the target position of the motor in inches, begin accelerating towards target. 
 * 
 * @param targetPos Target position in inches
 */
void ZStepper::setTargetPos(double targetPos) {
    this->targetPos = targetPos;
    if (targetPos < 0) 
        targetPos = 0;
    ticks = 0;
    if (targetPos == curPos)
        return;
    posToTargetTicks(targetPos); // set targetTicks
    cDir = (targetPos > curPos) ? 1 : 0;
    digitalWrite(dirPin, cDir);
    targetTicks = targetPos;
    curFreq = freqRange[0];
    targetFreq = freqRange[1];
    // Determine acceleration profile 
    // suppose the target is max:
    targetFreq = freqRange[1];
    // Assuming max accel cycle, is it possible to obtain target speed with this motion profile and the above constraint?
    int32_t ticksnonAccel = targetTicks - 2*(targetFreq/rampDeltaFreq)*rampTicks;
    if (ticksnonAccel < rampTicks) { // change to: newTarget = (∆f/2∑(flat)) * (∑ - ∑(flat)) {
        targetFreq = (targetTicks - rampTicks) * rampDeltaFreq / (2 * rampTicks);
        curVel = velType::constant;
        if (targetFreq < freqRange[0])
            curVel = velType::constant;    
    }
    else
        curVel = velType::trapezoidal;
    deccelPhaseStart = targetTicks - (targetFreq / rampDeltaFreq) * rampTicks;
    curFreq = freqRange[0];
    enableZStepper();
}

void ZStepper::setBoundaries() {
 // TODO:
}

void ZStepper::changeTargetFreq(int_fast8_t newTarget) {
    // TODO: Feature to add: update speed and velocity profile live while already moving to setpoints
}

/**
 * @brief Converts a target position in inches to a number of steps for the motor
 * 
 * @param targetPos Target position in inches
 */
void ZStepper::posToTargetTicks(double targetP) {
    // targetTicks =  (targetPos / pulleyCircumference) * gearRatio * 200 * microstep;
    targetTicks = std::abs(targetP);
}

bool ZStepper::isCompleted() {
    return (ticks >= std::abs(targetTicks));
}
void ZStepper::linRampUpdate() {  
    if (ticks >= deccelPhaseStart && curFreq - rampDeltaFreq >= freqRange[0]) {
       curFreq -= rampDeltaFreq;
    }  
    else if (curFreq < targetFreq - rampDeltaFreq) {
        curFreq += rampDeltaFreq;
    }
    resIter = true;
    nextRampBreakpoint += rampTicks;
}

/**
 * @brief Home axis, blocking
 * 
 */
void ZStepper::homeZStage(DigitalIO * limit, int32_t m_delayms) {
    while (true) {
        if (limit->getState()) 
            break;
        GPIO.out_w1ts = (1 << stepPin);
        delay(m_delayms);
        GPIO.out_w1tc = (1 << stepPin);
        delay(m_delayms);
    }
    curPos = 0;
    targetPos = 0;
    curVel = velType::none;
}