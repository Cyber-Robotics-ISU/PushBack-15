#pragma once
#include <vector>
#include "api.h"
#include "lemlib/api.hpp"
#include "driver_profile.hpp"

// Define global variables and objects here
struct ProfileOption { // profile option struct
    const char* name;
    void (*init)(); // runs once when switching
    void (*loop)(); // runs every cycle
    const char* description; 
};
extern int current_profile_selection; // current profile selection
extern std::vector<ProfileOption> profile_list;

extern pros::Controller masterController;

// Define VEX Motor Groups
extern pros::MotorGroup left_motor_group;
extern pros::MotorGroup right_motor_group;


extern pros::MotorGroup intake_motor_group;
extern pros::MotorGroup scoring_motor_group;


// Define Pneumatics
extern pros::adi::Pneumatics pneumatics;


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
