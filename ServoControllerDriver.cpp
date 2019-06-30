
#include "hwlib.hpp"
#include "ServoControllerDriver.h"

/// @file

/// @brief Function to start the PCA9685 chip
/// 
/// Resets all the setting son the chip 
/// and sets a frequency usable for servos
void PWMDriver::begin(void) {
    reset();  
    setPWMFreq(50);
}

/// @brief Function to reset the mode of the PCA9685 chip
///
/// Writes the reset signal to the mode register 
/// and waits 10 ms for it to be done
void PWMDriver::reset(void) {
    write8(PCA9685_MODE1, 0x80);
    hwlib::wait_ms(10);
}

/// @brief Help-Function that returns the lowest value
///
/// Returns lowest value
uint16_t PWMDriver::min(uint16_t a, uint16_t b){
    if(a<b){
        return a;
    }
    return b;
}

/// @brief Help-Function that floors a float value
///
/// casts a float to an interger to round it and then 
/// returns it if it is lower than the original float else -1
uint8_t PWMDriver::floor(float val){
    uint8_t nval = int(val);
    if(nval < val){
        return nval;
    }
    return nval - 1;
}

/// @brief Help-Function to map one long between 2 values
///
/// Re-maps a number from one range to another
long PWMDriver::map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/// @brief read8 function to read data from register
///
/// Writes to a register to request data and then read from its location
uint8_t PWMDriver::read8(uint8_t reg) {
    bus.write( i2c_address ).write( reg );
    return bus.read( i2c_address ).read_byte();
}

/// @brief write8 function to write data to a register
///
/// writes the given value to the given register on the stored i2c-bus
void PWMDriver::write8(uint8_t reg, uint8_t value){
    auto transaction = bus.write( i2c_address );
    transaction.write( reg );
    transaction.write( value );
}

/// @brief Function to set the frequency (Hz) of the PCA9685 chip
///
/// This functions sets the update frequency of the chip
/// not all pwm driven devices work on the same frequency 
/// normal servos work best on 50Hz for Leds you want a higher frequency to reduce stutter
void PWMDriver::setPWMFreq(float freq) {

	freq *= 0.9; // Correct for overshoot in the frequency setting
    float prescaleval = 25000000;
    prescaleval /= 4096;
    prescaleval /= freq;
    prescaleval -= 1;

    uint8_t prescale = floor(prescaleval + 0.5);

	uint8_t oldmode = read8(PCA9685_MODE1);
	uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep

	write8(PCA9685_MODE1, newmode);            // go to sleep
	write8(PCA9685_PRESCALE, prescale);        // set the prescaler
	write8(PCA9685_MODE1, oldmode);

	hwlib::wait_ms(5);

	write8(PCA9685_MODE1,oldmode | PCA9685_AI); //  This sets the MODE1 register to turn on auto increment.

}

/// @brief function to set PWM for given pin num
///
/// This funcion sets the pulse width of the given pin
/// on refers to the timing when to signal is meant to 
/// be on and off is when the signal should be turned off
void PWMDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {
    // starting write transaction
    auto transaction = bus.write( i2c_address ); transaction.write( LED0_ON_L + 4 * num );
    transaction.write( on );
    transaction.write( on >> 8 );
    transaction.write( off );
    transaction.write( off >> 8 );
    bus.primitives.write_stop(); 
    // Use the overwriten write_stop 
    // function from primitives to end the transaction
}

/// @brief function that maps degrees to pulse width
uint16_t PWMDriver::degToPwm(float deg){
    return map(deg, 0.0, 180.0, servoMin, servoMax);
}

/// @brief funtion that sets the pwm for a single pin
///
/// This function calculates the timings for the setPWM 
/// function and inserts them into it
/// Invert can be used to flip the pulse signal horizontaly 
/// so that instead of it starting high it starts low
void PWMDriver::setPin(uint8_t num, uint16_t val, bool invert) {
    // Clamp value between 0 and 4095 inclusive.
  
    val = min(val, (uint16_t)4095);
    if (invert) {
        if (val == 0) {
            // Special value for signal fully on.
            setPWM(num, 4096, 0);
        } else if (val == 4095) {
            // Special value for signal fully off.
            setPWM(num, 0, 4096);
        } else {
            setPWM(num, 0, 4095 - val);
        }
    } else {
        if (val == 4095) {
            // Special value for signal fully on.
            setPWM(num, 4096, 0);
        } else if (val == 0) {
            // Special value for signal fully off.
            setPWM(num, 0, 4096);
        } else {
            setPWM(num, 0, val);
        }
    }
}

/// @brief Sets minimum and maximum values for degree mapping
///
/// This functions is for the minimum and maximum pulse widths of your servos of choice
/// Some servos have a lower minimum value or a higher maximum and this way 
/// you can edit those so your servos dont break or wear down after a while
void PWMDriver::minMaxServo(uint16_t min, uint16_t max){
    servoMin = min; 
    servoMax = max;
}

