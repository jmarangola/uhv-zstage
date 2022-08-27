/**
 * @file main.cpp
 * @author John Marangola (marangol@bc.edu)
 * @brief ZStage system control 
 * @version 0.1
 * @date 2021-07-27
 * 
 * @copyright Copyright (c) 2021
 * 
 * local
 */

#include <Arduino.h>
#include "ZStepper.h"
#include "zEEPROM.h"
#include <iostream>
#include <cstdint>
#include "RotaryEncoder.h"
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>

int SamplingRate = 2000; //Read 1000 values in one second.
hw_timer_t * timer = NULL; 
volatile bool interruptbool1 = false;
ZStepper motor(2, 4, 14, timer);

// Input
RotaryEncoder positionKnob(16, 17, 15, 25, 1, std::vector<double> {0, 400*40});

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL(&timerMux);
  if (interruptbool1) {
    interruptbool1 = false;
    GPIO.out_w1ts = (1 << motor.getStepPin());
  }
  else {
    interruptbool1 = true;
    GPIO.out_w1tc = (1 << motor.getStepPin());
  }
  motor.ticks += 1;
  portEXIT_CRITICAL(&timerMux);
}

// Always called directly after ZStepper::setTargetPosition()
void startTrapRamp(ZStepper * motor) {
    timerAlarmDisable(timer);
    timerAlarmWrite(timer, 1000000/(motor->curFreq), true);
    motor->resIter = false;
    timerAlarmEnable(timer); 
}

void setup() {
    Serial.begin(9600);
    positionKnob.setupEncoder();
    motor.setup();
    pinMode(2, OUTPUT);
    // Hardware timer initialization
    timer = timerBegin(0, 80, true);               
    timerAttachInterrupt(timer, &onTimer, true);   
    unsigned int timerFactor = 1000000/motor.curFreq; 
    motor.homeZStage(&positionKnob, 1);
    motor.setTargetPos(400*40);
    startTrapRamp(&motor);
}

double maxSpeed, targetP;
int8_t newState;
void loop() {
    if (motor.getCurrentVelType() != velType::none) { // Check if motor should be engaged
        if (motor.isCompleted()) {
            timerAlarmDisable(timer);
            motor.setPosToTarget();
            Serial.println("position reached.");
            motor.disableZStepper();
        }
        if (motor.getCurrentVelType() == velType::trapezoidal) {
            if (motor.resIter == true) {
                timerAlarmEnable(timer);
                motor.resIter = false;
            }
            else if (motor.ticks >= motor.nextRampBreakpoint) {
                motor.linRampUpdate();
                timerAlarmDisable(timer);
                timerAlarmWrite(timer, 1000000/motor.curFreq, true);
            }
        }
    }
}

