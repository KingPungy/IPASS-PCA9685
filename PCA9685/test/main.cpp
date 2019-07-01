#include "hwlib.hpp"
#include <cmath>
#include "../ServoControllerDriver.h"

#define SERVO_0 70
#define SERVO_180 470


int main( void ){
	// using target instead of hwlib::target
    namespace target = hwlib::target;

    auto scl = target::pin_oc(target::pins::scl1);
    auto sda = target::pin_oc(target::pins::sda1);
	auto i2c_bus  = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
    
	auto pwm = PWMDriver(i2c_bus);
	pwm.begin();
	pwm.minMaxServo(SERVO_0,SERVO_180);

	
	

	for(;;){

		for (float i = 0; i < 180; i+=0.5)
		{	// pin 0 of the PCA9685 chip 0 to 180 degrees in steps of half a degree
			pwm.setPin(0, pwm.degToPwm(i)); 
			hwlib::wait_ms(50);
		}

		pwm.setPin(0, pwm.degToPwm(90));
		hwlib::wait_ms(50);

		for (float j = 90; j < 180; j+=0.5)
		{
			pwm.setPin(1, pwm.degToPwm(j));
			hwlib::wait_ms(50);
		}

		pwm.setPin(1, pwm.degToPwm(90));
		hwlib::wait_ms(50);

		for (float k = 110; k < 180; k+=0.5)
		{
			pwm.setPin(2, pwm.degToPwm(k));
			hwlib::wait_ms(50);
		}

		pwm.setPin(2, pwm.degToPwm(150));
		hwlib::wait_ms(50);
		for (float l = 70; l < 130; l+=0.5)
		{
			pwm.setPin(3, pwm.degToPwm(l));
			hwlib::wait_ms(50);
		}

		pwm.setPin(3, pwm.degToPwm(130));
		hwlib::wait_ms(50);
		
	}

}




