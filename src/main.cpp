#include "main.h"
#include "driver_profile.hpp"
#include "global.hpp"
#include "lemlib/api.hpp" 
#include "pros/motors.h"

// basic
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio> 

// lvgl vars
#include "liblvgl/lvgl.h"

#include "ui.hpp" 
#include "pros/apix.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    horizontal_encoder.reset_position();
    vertical_encoder.reset_position();
    pros::delay(1000);
    chassis.calibrate(); // calibrate sensors

	
   chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {  // Creates a motor group with forwards port 5 and reversed ports 4 & 6
	int last_profile_selection = current_profile_selection;
    profile_list[current_profile_selection].init();

	while (true) {
         if (current_profile_selection != last_profile_selection) {
            profile_list[current_profile_selection].init();
            last_profile_selection = current_profile_selection;
        }

        profile_list[current_profile_selection].loop();
		pros::delay(20);                               // Run for 20 ms then update
	}
} // End of opcontrol 