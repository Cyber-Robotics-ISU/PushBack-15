#include "driver_profile.hpp"
#include <vector>  
#include <string> 
#include <cmath>

#include "liblvgl/lvgl.h"
#include "api.h"

#include "global.hpp"





void default_profile_init() {
    masterController.set_text(0,1, "default");
}

void default_profile_loop() {

    const bool shift = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    const bool r2 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    const bool r1 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    const bool down = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);
    const bool b = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_B);

    int leftY = masterController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = masterController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    leftY = throttle_curve.curve(leftY);
    rightX = steer_curve.curve(rightX);
    //chassis.arcade(leftY, rightX);
    //hold task

    chassis.arcade(leftY, rightX);
    pros::delay(10);

}

void calvin_profile_init() {
   masterController.set_text(0,1, "Calvin");
}

void calvin_profile_loop() {
   default_profile_loop();
}

void unknown_profile_init() {
    masterController.set_text(0,1, "test 2");
}

void unknown_profile_loop() {
    
}
