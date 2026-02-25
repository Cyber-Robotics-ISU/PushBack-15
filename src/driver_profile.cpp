#include "driver_profile.hpp"
#include <vector>  
#include <string> 
#include <cmath>

#include "liblvgl/lvgl.h"
#include "api.h"

#include "global.hpp"


bool toggleIntake = false;

void toggleIntakeFn(){
    if(toggleIntake){
        intake_motor_group.move(127);
    }
    else {
        intake_motor_group.move(0);
    }
    toggleIntake = !toggleIntake;
}
bool toggleScore = false;

void toggleScoreFn(){
    if(toggleScore){
        scoring_motor_group.move(127);
    }
    else {
        scoring_motor_group.move(0);
    }
    toggleScore = !toggleScore;
}

void default_profile_init(){
    masterController.set_text(0, 1, "Default");
}

void default_profile_loop(){
    const bool r1 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    const bool l1 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    const bool r2 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    const bool l2 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    const bool y = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_Y);

    if(r1){
        intake_motor_group.move(127);
    }
    else {
        intake_motor_group.move(0);
    }
    if(l1){
        scoring_motor_group.move(127);
    }
    else{
        scoring_motor_group.move(0);
    }
    if(y){
        //togglePneumatics();
    }

    int leftY = masterController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = masterController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    leftY = throttle_curve.curve(leftY);
    rightX = steer_curve.curve(rightX);

    // move the robot
    chassis.arcade(-leftY, rightX);
    
}

void toggle_profile_init(){
    masterController.set_text(0, 1, "Default");
}

void toggle_profile_loop(){
    const bool r1 = masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1);
    const bool l1 = masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
    const bool r2 = masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2);
    const bool l2 = masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2);
    const bool y = masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y);

    if(r1){
        toggleIntakeFn();  
    }
    if(l1){
        toggleScoreFn();
    }
    if(y){
        chassis.setPose(0, 0, 0);
        chassis.turnToHeading(90, 2000,{}, false);
    }
    if(l2){
        chassis.setPose(0, 0, 0);
        chassis.moveToPoint(0, 24, 2000, {}, false);
    }

    int leftY = masterController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = masterController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    leftY = throttle_curve.curve(leftY);
    rightX = steer_curve.curve(rightX);

    // move the robot
    chassis.arcade(-leftY, rightX);
    
}

