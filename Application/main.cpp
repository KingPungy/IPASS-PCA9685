#include "hwlib.hpp"
#include <cmath>
#include "../PCA9685/ServoControllerDriver.h"

#define SERVO_0 70
#define SERVO_180 470

#define ArmLength 8.0

// global variables for the servo rotations
float posRot;
float posFor;
float posUp;
float posGrab ;

// declarations of the functions 
void setPositions(float x, float y, float z);
void setCoords(float array[], int pos);
void updateRobotServos(PWMDriver &pwm);
void clawAction(bool action);




int main( void ){
	// using target instead of hwlib::target
    namespace target = hwlib::target;

    auto scl = target::pin_oc(target::pins::scl1);
    auto sda = target::pin_oc(target::pins::sda1);
	auto i2c_bus  = hwlib::i2c_bus_bit_banged_scl_sda( scl, sda );
    
	auto pwm = PWMDriver(i2c_bus);
	pwm.begin();
	pwm.minMaxServo(SERVO_0,SERVO_180);

	// more than 4 positions makes  it stop working????
	float coords[] = {0.0, 170.0, 110.0, 0,    90.0, 100.0, 170.0, 1,    180.0, 150.0, 150.0, 0,     135.0, 180.0, 110.0, 1};
				//   rot   for    up     grab

	for(;;){
		// cycles through all 4 positions stored in coords[]
		for (int i = 0; i < 4; i++)
		{
			// sets new servo degrees stored in coords[]
			setCoords(coords,i);
			// waits 10 ms to make sure all positions have been stored
			hwlib::wait_ms(10); 
			// updates all the servo positions 
			updateRobotServos(pwm);
			// 4 seconds to get to the positions because of 
			// the plastic servos and friction on the base plate
			hwlib::wait_ms(4000); 
			
		}
		
	}

}




// this function doesnt work, pow() and sqrt() 
// cause problems; it does work in the Arduino IDE (the math is right)
void setPositions(float x, float y, float z){

	hwlib::cout << "before ltt\n" << hwlib::flush;
  	auto ltt = sqrt( ( pow(x,2) + pow(y, 2) ) );
	  hwlib::cout << "after ltt\n" << hwlib::flush;
  	auto lttZ = sqrt( ( pow(z, 2) + pow(ltt, 2) ) );

  	posRot = 90.0 - (atan(x / y) * (180.0 / 3.141592653));

  	auto dDown = abs((atan(abs(z) / ltt) * (180.0 / 3.141592653)));

  	float rservoD = acos((lttZ / 2) / ArmLength) * (180.0 / 3.141592653)  ;
  	float lservoD = abs((90.0 - rservoD) * 2.0) ;

  	if (z <= 0.0) {
    	rservoD -= dDown;
  	} else if (z > 0.0) {
    	rservoD += dDown;
  	}

  	lservoD -= (90.0 - rservoD);
  
  	posFor = (180.0 - rservoD);
  	posUp = (90.0 + lservoD);

}
// Sets the global vars of the rotations to 
// the degrees stored in the coords array[]
// pos determines the position in the array (counting from 0)
void setCoords(float array[], int pos){

	posRot   = array[(pos*4)];
	posFor   = array[(pos*4)+1];
  	posUp    = array[(pos*4)+2];
	// 4th value stored in coords[] is a boolean used for claw rotation
	clawAction(array[(pos*4)+3]);  // 0/1
}


// Updates the servos rotations
// Requires a PWMDriver object as argument
void updateRobotServos( PWMDriver &pwm ){
	pwm.setPin( 0, pwm.degToPwm( posRot ));
	pwm.setPin( 1, pwm.degToPwm( posFor ));
	pwm.setPin( 2, pwm.degToPwm( posUp  ));
	pwm.setPin( 3, pwm.degToPwm( posGrab));
}

// sets the rotation of the grab servo
// 1 = Closed;  0 = Open
void clawAction(bool action){
	float grabMin = 70.0;
	float grabMax = 130.0;
	if (action){
		posGrab = grabMin;
	}else {
		posGrab = grabMax;
	}
}


