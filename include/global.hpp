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
    int side;  // 0 = red, 1 = blue, 2 = skills
};

// -1 = red, 1 = blue, 0 = skills
extern int autonColor;
#define IS_BLUE (autonColor == 1)
#define IS_RED  (autonColor == -1)
#define IS_SKILLS (autonColor == 0)



extern int current_auton_selection;

// master list (ALL autons)
extern std::vector<AutonOption> auton_master_list;

// filtered list (only shows selected group)
extern std::vector<AutonOption> auton_list;

// Define global variables and objects here
extern pros::Controller masterController;


// Define VEX Motors
extern pros::MotorGroup left_motor_group;
extern pros::MotorGroup right_motor_group;

extern pros::Motor scorer;
extern pros::Motor intake;

// Define Pneumatics
extern pros::adi::Pneumatics blocker;
extern pros::adi::Pneumatics extender;
extern pros::adi::Pneumatics scrapper;

// Define VEX Sensors
extern pros::Imu imu;
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
