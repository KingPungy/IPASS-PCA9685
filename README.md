# Adafruit PCA9685 Library
IPASS repository for my library for the PCA9685 pwm driver controller chip

link to original repository:
https://github.com/KingPungy/IPASS-PCA9685

makeup of zip folder

Zipfile
    |Ipass/
	| Makefile.link make file so its able to be flashed on the Arduino Due  
	|
        |/Application
        |---_
        |    |-main.cpp
        |    |-Makefile
        |
        |/PCA9685/
        |---_
        |    |-/html = documentation
        |    |-/latex is part of the documentation
        |    |-/test contains test code
        |    |-Doxyfile doxygen config file
        |    |-ServoControllerDriver.cpp class function file 
    ____|    |-ServoControllerDriver.h   Library class  file
    |    
    | posterwhite.pdf ipass library poster
    |
    | PvA   Plan of Action




For the application i used a 4dof (4 degrees of freedom) robot arm.
There are a lot of them you can buy but i recommend one with metal 
gear servos wich can handle the rotations and weight of the arm. 
Those with metal gear servos are pricier though.

Here is one: 
https://www.dennisdeal.com/products/4dof-assembling-acrylic-mechine-robot-arm-with-mg90-metal-gear-servo-for-robot-diy

The chip the library is writen for is the PCA9685 from adafruit ( https://www.adafruit.com/product/815 ). 
Part of my library is based on their code, but writen so that it works together with hwlib from the github-user Wovo 
( https://github.com/wovo/hwlib ) 

For the Doxygen documentations, look in the PCA9685/html and click on one of the html files to look at the generated documentation.


Distributed under the Boost Software License, Version 1.0. (See accompanying file LICENSE_1_0.txt in PCA9685 or copy at http://www.boost.org/LICENSE_1_0.txt)