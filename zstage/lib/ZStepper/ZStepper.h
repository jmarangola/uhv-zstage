/**
 * @file ZStepper.h
 * @author John Marangola (marangol@bc.edu)
 * @brief ZStepper header
 */

#include <Arduino.h>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "DigitalIO.h"

enum velType {constant, trapezoidal, none};

class ZStepper {
    velType curVel = trapezoidal;
    // Control pins
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t enablePin = -1;
    // System state for control
    volatile int_fast32_t targetFreq;
    volatile double curPos;
    double targetPos;
    uint8_t cDir = 1;
    // Motor specific parameters
    int_fast32_t freqRange[2] = {1000, 3500};
    uint8_t microstep = 1; 
    const double pulleyCircumference = 1 * PI;
    const double gearRatio = 1.1;
    // Hardware timer related
    volatile long targetTicks;
    hw_timer_t * timer;
    void posToTargetTicks(double targetPos);
    // Ramp
    volatile int rampDeltaFreq = 400/8;
    int_fast32_t rampTicks = 400/8;
    int_fast32_t deccelPhaseStart;
    bool enabled = true;

    public:
        volatile int_fast32_t curFreq = freqRange[0];
        volatile int_fast32_t ticks;
        volatile int_fast32_t nextRampBreakpoint=rampTicks;
        volatile bool resIter = false;
        ZStepper(uint8_t stepPin, uint8_t dirPin, hw_timer_t * timer) : stepPin(stepPin), dirPin(dirPin), timer(timer) {}
        ZStepper(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin, hw_timer_t *timer) : stepPin(stepPin), dirPin(dirPin), enablePin(enablePin) {}
        void setup() {
            pinMode(stepPin, OUTPUT);
            pinMode(dirPin, OUTPUT);
            if (enablePin > -1)
                pinMode(enablePin, OUTPUT);
        }
        uint8_t IRAM_ATTR getStepPin() {
            return stepPin;
        }
        int_fast32_t IRAM_ATTR getTargetTicks() {
            return targetTicks;
        }
        void changeTargetFreq(int_fast8_t newTarget);
        void setMinAbsFreq(int_fast8_t minFreq) {
            freqRange[0] = minFreq;
        }
        enum velType getCurrentVelType() { 
            return curVel; 
        }
        void IRAM_ATTR setPosToTarget() {
            curPos = targetPos;
            curVel = velType::none;
        }
        bool isEnabled() {
            return enabled;
        }
        void disableZStepper() {
            enabled = false;
            if (enablePin > -1) 
                digitalWrite(enablePin, HIGH);
        }
        void enableZStepper() {
            enabled = true;
            if (enablePin > -1)
                digitalWrite(enablePin, LOW);
        }
        double getCurrentPosition() {
            return curPos;
        }
        void setCurrentPosition(double pos) {
            curPos = pos;
        }
        bool isCompleted();
        void linRampUpdate();
        void homeZStage(DigitalIO * limit, int32_t m_delayms);
        void setTargetPos(double targetPos);
        void setBoundaries();
};