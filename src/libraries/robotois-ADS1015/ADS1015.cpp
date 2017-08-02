/*
 * File:   ADS1015.cpp
 * Author: yova
 *
 * Created on 19 de mayo de 2016, 08:29 PM
 */

#include <stdint.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include "ADS1015.h"
#include <vector>

#include <thread>
#include <chrono>

ADS1015::ADS1015(uint8_t _addr) {
    uint8_t result;
    if(_addr > 1){ // - 1 bit for custom address
        printf("Wrong slave address for the Digital IO Module...\n");
        return;
    }

    bcm_init();

    addr = ADS1015_DEFAULT_ADDRESS | _addr;
    selectModule();

    wBuf[0] = ADS1015_CONFIG_REG;
    result = bcm2835_i2c_read_register_rs(wBuf,rBuf,2);
    if(result == 0x01){
        printf("Error reading from the Analog Module...\n");
        return;
    }
}

ADS1015::ADS1015(const ADS1015& orig) {
}

ADS1015::~ADS1015() {
}

void ADS1015::selectModule(){
    bcm2835_i2c_setSlaveAddress(addr);
}

/**
 * Set the input port to be read and the gain. It is recomended that the gain is selected
 * according to the maximum input voltage for the given input address, this way
 * we'll obtain a good resolution.
 * @param _inputAdd
 * @param _gain
 */
void ADS1015::selectInput(uint8_t _inputAdd, uint8_t _gain){
    selectModule();
    inputAdd = _inputAdd;
    inputGain = _gain;
    uint8_t config_low = ADS1015_CONF_REG_COMP_QUE_DC |
                      ADS1015_CONF_REG_COMP_LAT_NL |
                      ADS1015_CONF_REG_COMP_POL_AL |
                      ADS1015_CONF_REG_COMP_MODE_TRAD |
                      ADS1015_CONF_REG_DR_1600SPS;
    uint8_t config_high = ADS1015_CONF_REG_MODE_SINGLE;

    if(inputAdd < 0x04 or inputAdd > 0x07){ // - Seleccion de entrada individual
        printf("Invalid Input...\n");
        return;
    }

    if(inputGain > ADS1015_512_GAIN){ // - Seleccion de entrada individual
        printf("Invalid Gain...\n");
        return;
    }
    switch(inputGain){
        case ADS1015_512_GAIN:
            resolution = ADS1015_512_RES;
            break;
        case ADS1015_1024_GAIN:
            resolution = ADS1015_1024_RES;
            break;
        case ADS1015_2048_GAIN:
            resolution = ADS1015_2048_RES;
            break;
        case ADS1015_4096_GAIN:
            resolution = ADS1015_4096_RES;
            break;
        case ADS1015_6144_GAIN:
            resolution = ADS1015_6144_RES;
            break;
    }

    wBuf[0] = ADS1015_CONFIG_REG;
    // -- Upper Byte, set the input channel and input gain;
    config_high |= (uint8_t)(inputAdd << 4) | (uint8_t)(inputGain << 1);
    // -
    config_high |= ADS1015_CONF_REG_OS_SINGLE;

    wBuf[1] = config_high;
    // - Lower Byte => 128 SPS, Disable Comparator
    wBuf[2] = config_low;

    bcm2835_i2c_write(wBuf, 3);
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
}

/**
 * Returns the ADC Conversion Register, returns a 12 bit register.
 * @return
 */
int16_t ADS1015::readRawInput(){
    selectModule();
    uint16_t input = 0x00;

    wBuf[0] = ADS1015_CONV_REG; // - Registro de conversion
    bcm2835_i2c_read_register_rs(wBuf,rBuf,2);

    input = (uint16_t)(rBuf[0] << 8) |(rBuf[1]);
    input = input >> 4;

    return fullRangeMeas(input);
}

int16_t ADS1015::fullRangeMeas(uint16_t reading){
    if(reading > 0x07FF)
        return -(4095 - reading);
    else
        return (int16_t)reading;
}

/**
 * Returns the current input reading in volts.
 * @return
 */
float ADS1015::readInput(){
    int16_t input = readRawInput();

    return input*resolution;
}

void ADS1015::bcm_init(){
    // - 400kHz aproximadamente...
    uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_626;
    // - La direccion del esclavo se establece en cada modulo

    if (!bcm2835_init()){
        printf("BCM2835 Error!!...\n");
        exit(1);
    }

    bcm2835_i2c_begin();

    bcm2835_i2c_setClockDivider(clk_div);
}

void ADS1015::bcm_end(){
    bcm2835_i2c_end();
    bcm2835_close();
}

void ADS1015::release(){
    // bcm_end();
}

std::vector<float> ADS1015::inputSampling(uint8_t port, uint8_t gain, uint16_t sampleTime){
    selectModule();
    initContinuousMode(port, gain);
    std::vector<float> samples;
    
    auto startTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    while (
        sampleTime >
        std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count()
    ){
        samples.push_back(readSample());
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    }
//    printf("Samples Size: %d\n", samples.size());
    return samples;
}

std::vector<float> ADS1015::minMax(std::vector<float> samples){
    float min = 2.5, max = 2.5;
    std::vector<float> minmax;
    for(float sample : samples){
        if(sample < min){
            min = sample;
        } else {
            if(sample > max){
                max = sample;
            }
        }
    }
    minmax.push_back(min);
    minmax.push_back(max);
    return minmax;
}

float ADS1015::readSample(){
    uint16_t input = 0x00;

    wBuf[0] = ADS1015_CONV_REG; // - Registro de conversion
    bcm2835_i2c_read_register_rs(wBuf,rBuf,2);

    input = (uint16_t)(rBuf[0] << 8) |(rBuf[1]);
    input = input >> 4;

    int16_t fullInput = fullRangeMeas(input);
    return fullInput*resolution;
}

void ADS1015::initContinuousMode(uint8_t port, uint8_t gain){
    inputAdd = port;
    inputGain = gain;

    if(inputAdd < 0x04 or inputAdd > 0x07){ // - Seleccion de entrada individual
        printf("Invalid Input...\n");
        return;
    }

    if(inputGain > ADS1015_512_GAIN){ // - Seleccion de entrada individual
        printf("Invalid Gain...\n");
        return;
    }
    
    switch(inputGain){
        case ADS1015_512_GAIN:
            resolution = 0.512/2048.0;
            break;
        case ADS1015_1024_GAIN:
            resolution = 1.024/2048.0;
            break;
        case ADS1015_2048_GAIN:
            resolution = 2.048/2048.0;
            break;
        case ADS1015_4096_GAIN:
            resolution = 4.096/2048.0;
            break;
        case ADS1015_6144_GAIN:
            resolution = 6.144/2048.0;
            break;
    }

    uint8_t config_low = ADS1015_CONFIG_DR_3300SPS |
                        ADS1015_CONFIG_COMP_QUE_DC;
    uint8_t config_high = ADS1015_CONFIG_OS_SINGLE;

    // -- Upper Byte, set the input channel and input gain;
    config_high |= (uint8_t)(inputAdd << 4) | (uint8_t)(inputGain << 1);

    wBuf[0] = ADS1015_CONFIG_REG;
    wBuf[1] = config_high;
    wBuf[2] = config_low;

    bcm2835_i2c_write(wBuf, 3);
}