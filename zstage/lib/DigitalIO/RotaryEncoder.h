/**
 * @file RotaryEncoder.h
 * @author John Marangola (marangol@bc.edu)
 * @brief A class for obtaining rotary encoder input
 * local
 */

#pragma once
#include "DigitalIO.h"
#include <ESP32Encoder.h>
#include <cstdint>
#include <cmath>
#include <vector>

class RotaryEncoder : public DigitalIO {
    ESP32Encoder encoder;
    uint8_t ckPin;
    uint8_t dtPin;
    double defaultValue = 0;
    int64_t lastCount = 0;
    double alpha=1, beta=0;  // Output = alpha * count + beta. (Beta determined by default value)
    std::vector<double> range{-999999, 999999};
    public:
        RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw) : DigitalIO(sw, DigitalIO::digitalMode::NC), ckPin(clk), dtPin(dt) {}
        RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw, int64_t defaultValue) : DigitalIO(sw, DigitalIO::digitalMode::NC), ckPin(clk), dtPin(dt), defaultValue(defaultValue) {}
        RotaryEncoder(uint8_t clk, uint8_t dt, uint8_t sw, double defaultValue, double alpha, std::vector<double> outputRange) : DigitalIO(sw, DigitalIO::digitalMode::NC), ckPin(clk), dtPin(dt), defaultValue(defaultValue), alpha(alpha), range(outputRange) {}
        void setupEncoder() {
            ESP32Encoder::useInternalWeakPullResistors = UP;
            initPullup();
            encoder.attachSingleEdge(ckPin, dtPin);
            double tempDefault = std::floor((defaultValue - beta) / alpha);
            encoder.setCount(tempDefault);
            beta = defaultValue - tempDefault * alpha;
            lastCount = defaultValue;
        }
        double rawToLinearlyScaled() {
            return alpha * encoder.getCount() + beta; 
        }
        bool checkNewValue(double *newValue) {
            double tmp = rawToLinearlyScaled();
            if (tmp == lastCount || tmp > range[1] || tmp < range[0])
                return false;
            lastCount = tmp;
            *newValue = tmp;
            return true;
        }
        void resetValue() { 
            lastCount = defaultValue;
            encoder.setCount(std::floor((defaultValue - beta) / alpha));
        }
};