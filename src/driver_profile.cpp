#include "driver_profile.hpp"
#include <vector>  
#include <string> 
#include <cmath>

#include "liblvgl/lvgl.h"
#include "api.h"

#include "global.hpp"
#include "driver_controls.hpp"


void default_profile_init() {
    masterController.set_text(0,1, "Driver 2");
    driver_controls_init();
}

void default_profile_loop() {

    int leftY = masterController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = masterController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    if (masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
        auton_right();
    }

    leftY = throttle_curve.curve(leftY);
    rightX = steer_curve.curve(rightX);
    //chassis.arcade(leftY, rightX);
    //hold task

    chassis.arcade(leftY, rightX);
    driver_controls_update();
    pros::delay(10);

}

void calvin_profile_init() {
   masterController.set_text(0,1, "Calvin");
   driver_controls_init();
}

void calvin_profile_loop() {
   default_profile_loop();
}

void driver2_profile_init() {
    masterController.set_text(0,1, "test 2");
    driver_controls_init();
}

void driver2_profile_loop() {
    default_profile_loop();
}
