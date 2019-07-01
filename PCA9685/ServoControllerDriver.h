//          Copyright Scott Timmermans 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)


#ifndef SERVOCONTROLLERDRIVER_H
#define SERVOCONTROLLERDRIVER_H

#include "hwlib.hpp"

/// @file
#define PCA9685_SUBADR1 0x2 /**< i2c bus address 1 */
#define PCA9685_SUBADR2 0x3 /**< i2c bus address 2 */
#define PCA9685_SUBADR3 0x4 /**< i2c bus address 3 */

#define PCA9685_MODE1 0x0 /**< Mode Register 1 */
#define PCA9685_PRESCALE 0xFE /**< Prescaler for PWM output frequency */
#define PCA9685_AI 0x20 /**< Auto Increment (AI) mode to keep writing data */

#define LED0_ON_L 0x6 /**< LED0 output and brightness control byte 0 */
#define LED0_ON_H 0x7 /**< LED0 output and brightness control byte 1 */
#define LED0_OFF_L 0x8 /**< LED0 output and brightness control byte 2 */
#define LED0_OFF_H 0x9 /**< LED0 output and brightness control byte 3 */

#define ALLLED_ON_L 0xFA /**< load all the LEDn_ON registers, byte 0 */
#define ALLLED_ON_H 0xFB /**< load all the LEDn_ON registers, byte 1 */
#define ALLLED_OFF_L 0xFC /**< load all the LEDn_OFF registers, byte 0 */
#define ALLLED_OFF_H 0xFD /**< load all the LEDn_OFF registers, byte 1 */

/// @brief PWM driver class
/// 
/// This class has all the needed functions 
/// to set the PWM pulse signals on the PCA9685 chip.
/// It also includes some functions to help with calculations 
/// such as degree mapping to pulse widths.
/// To create the class you need to give it an i2c_bus na dits address (default is 0x40)
class PWMDriver{
public:

    PWMDriver(hwlib::i2c_bus & bus, uint8_t address = 0x40):// default I2C Addr
        bus(bus),i2c_address(address),
        servoMin(100),servoMax(400){} 


    void begin(void);
    void reset(void);

    void minMaxServo(uint16_t min, uint16_t max);

    // needed help functions     
    uint16_t min(uint16_t a, uint16_t b);
    uint8_t floor(float val);
    long map(long x, long in_min, long in_max, long out_min, long out_max);
    uint16_t degToPwm(float deg);

    // functions that set the freq for the 
    // controller and set de freq of the pins
    void setPWMFreq(float freq);
    void setPWM(uint8_t num, uint16_t on, uint16_t off);
    void setPin(uint8_t num, uint16_t val, bool invert = false);



private:

    hwlib::i2c_bus & bus;
    
    uint8_t i2c_address;

    uint16_t servoMin; 
    uint16_t servoMax;
    
    uint8_t read8(uint8_t address);
    void write8(uint8_t address, uint8_t d);    
    

};

#endif








