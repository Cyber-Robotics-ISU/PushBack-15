#include "driver_profile.hpp"


void driver_profile_init(){
    masterController.set_text(0, 1, "Default");
}

void driver_profile_loop(){
    int leftY = masterController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = masterController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    leftY = throttle_curve.curve(leftY);
    rightX = steer_curve.curve(rightX);

    // move the robot
    chassis.arcade(leftY, rightX);
}
