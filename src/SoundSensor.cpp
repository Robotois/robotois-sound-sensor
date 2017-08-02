#include <iostream>
#include <cmath>
#include "SoundSensor.h"

SoundSensor::SoundSensor(uint8_t _addr){
  analogModule = new ADS1015(_addr);
  scaleFactor = 100.0f / 5.0f; // Voltage range => (-2) - 2
}

SoundSensor::~SoundSensor(){
}

void SoundSensor::selectPort(uint8_t _port){
    switch(_port){
        case 0x01:
            adsPort = ADS1015_SENSOR1;
            break;
        case 0x02:
            adsPort = ADS1015_SENSOR2;
            break;
        case 0x03:
            adsPort = ADS1015_SENSOR3;
            break;
        case 0x04:
            adsPort = ADS1015_SENSOR4;
            break;
        default:
            printf("Error selecting the Analog Port...\n");
            return;
    }
}

float SoundSensor::getValue(){
    std::vector<float> minmax;
    minmax = analogModule->minMax(
        analogModule->inputSampling(adsPort, ADS1015_6144_GAIN, 100)
    );
    float amplitude = minmax[1] - minmax[0];
//    printf("min: %f, max: %f => Amplitude: %f\n", minmax[0], minmax[1], amplitude);
    // set the ADS1015 in single shot mode
    analogModule->selectInput(adsPort, ADS1015_6144_GAIN);
    return amplitude;
}

uint8_t SoundSensor::getBasicValue(){
  return (uint8_t) (std::round(getValue() * scaleFactor));
}

void SoundSensor::release(){
  delete analogModule;
  printf("[SoundSensor] => Released\n");
}

