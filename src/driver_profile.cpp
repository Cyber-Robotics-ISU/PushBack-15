#include "driver_profile.hpp"
#include "controls.hpp"
#include "global.hpp"


bool pressed = 0;
bool ToggleOn = 0;

void Toggle(){
    bool current = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    if(current && !pressed){
        ToggleOn = !ToggleOn;
    }
    if(ToggleOn){
        intake_motor_group.move(127);
    }
    else{
        intake_motor_group.move(0);
    }
    pressed = current;
}

void default_profile_init(){
    masterController.set_text(0, 1, "Default");
}

void default_profile_loop(){
    int r1 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    int l1 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    int r2 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    int l2 = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    int y = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_Y);

    if(r1){
        intake_motor_group.move(127);
    }
    if(r2){
        intake_motor_group.move(-127);
    }
    if(l1){
        scoring_motor_group.move(127);
    }
    if(l2){
        scoring_motor_group.move(-127);
    }
    if(y){
        togglePneumatics();
    }

    int leftY = masterController.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    int rightX = masterController.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    leftY = throttle_curve.curve(leftY);
    rightX = steer_curve.curve(rightX);

    // move the robot
    chassis.arcade(-leftY, rightX);
    
}

