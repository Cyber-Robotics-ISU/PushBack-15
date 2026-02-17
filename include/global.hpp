#pragma once
#include <vector>
#include "api.h"
#include "lemlib/api.hpp"
#include "autons.hpp"  
#include "driver_profile.hpp"
#include "pid.hpp"

/** Define global variables and objects here */
struct ProfileOption { // profile option struct
    const char* name;
    void (*init)(); // runs once when switching
    void (*loop)(); // runs every cycle
    const char* description; 
};
extern int current_profile_selection; // current profile selection
extern std::vector<ProfileOption> profile_list;

struct AutonOption {
    const char* name;
    const char* description;
    void (*func)();
    int side;  // 0 = red, 1 = blue, 2 = both
};

// -1 = red, 1 = blue
extern int autonColor;
#define IS_BLUE (autonColor == 1)
#define IS_RED  (autonColor == -1)



extern int current_auton_selection;

// master list (ALL autons)
extern std::vector<AutonOption> auton_master_list;

// filtered list (only shows blue/red/both)
extern std::vector<AutonOption> auton_list;

// Define global variables and objects here
extern pros::Controller masterController;


extern pros::adi::Pneumatics scrapperPneumatics;
extern pros::adi::Pneumatics liftPneumatics;
extern pros::adi::Pneumatics hoodPneumatics;
// Define VEX Motors

// Define VEX Motor Groups
extern pros::MotorGroup left_motor_group;
extern pros::MotorGroup right_motor_group;

extern pros::MotorGroup intake_group_lower;
extern pros::MotorGroup intake_group_upper;
extern pros::MotorGroup shooter;

// Define VEX Sensors
extern pros::Imu imu;
extern pros::Distance distance;
extern pros::Rotation horizontal_encoder;
extern pros::Rotation vertical_encoder;

// Define LebLib
extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::ExpoDriveCurve steer_curve;

extern lemlib::TrackingWheel horizontal_tracking_wheel;
extern lemlib::TrackingWheel vertical_tracking_wheel;
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;
extern lemlib::OdomSensors sensors;
extern lemlib::Drivetrain drivetrain;
extern lemlib::Chassis chassis;
