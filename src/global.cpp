#include "global.hpp"


/** Define Variables  */
int current_profile_selection = 0; // Currently selected profile index
std::vector<ProfileOption> profile_list = {
    {"Default", default_profile_init, default_profile_loop, "Alternate driver profile"},
    {"Calvin", calvin_profile_init, calvin_profile_loop, "Precision driver profile"},
    {"Driver 2", driver2_profile_init, driver2_profile_loop, "Alternate driver profile"}
};

// -1 = red, 1 = blue, 0 = skills
int autonColor = 1; // default blue

int current_auton_selection = 0;

// ALL autons stored here
std::vector<AutonOption> auton_master_list = {
    { "Red Rush",
      "Rush the middle mogo. Scores 2 rings.\nFast and consistent.",
      auton_right,
      0 }, // red

    { "Blue Rush",
      "Blue version of Mogo Rush.\nScores 2 rings.",
      auton_left,
      1 }, // blue

    { "Skills Auton",
      "Runs full skills path.\nLonger routine for skills.",
      auton_skills,
      2 }, // skills
};

// Starts empty - will be filled based on auton group
std::vector<AutonOption> auton_list = {};

// Define variables 

// Defined VEX Main Master Controller
pros::Controller masterController(pros::E_CONTROLLER_MASTER);

// Define VEX Motors

// Define VEX Motor Groups
pros::MotorGroup left_motor_group({11,12,13}, pros::MotorGearset::blue); // left motors use 600 RPM cartrifges
pros::MotorGroup right_motor_group({-18,-19,-20}, pros::MotorGearset::blue); // right motors use 600 RPM cartridges
pros::Motor scorer(-9, pros::MotorGearset::red); // four intake motors run together with blue cartridges (flip signs if wiring requires reversal)
pros::Motor intake(1, pros::MotorGearset::blue);

// Define Pneumatics
pros::adi::Pneumatics extender({'A'}, false);
pros::adi::Pneumatics blocker({'B'}, false);
pros::adi::Pneumatics scrapper({'H'}, false);


// Define VEX Sensors
pros::Imu imu(15); 
pros::Rotation horizontal_encoder(12); // horizontal tracking wheel Rotation sensor
pros::Rotation vertical_encoder(11); // vertical tracking wheel Rotation sensor

lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// Define LebLib 
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_2, -4); // horizontal tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_2, 0); // vertical tracking wheel
lemlib::ControllerSettings angular_controller(
    3.4, 0, 0,   // kP, kI, kD
    0,         // antiWindup
    0, 0,      // small error range & timeout
    0, 0,      // large error range & timeout
    0          // max slew
);

lemlib::ControllerSettings lateral_controller(
    5.5, 0, 0,   // kP, kI, kD
    0,         // antiWindup
    0, 0,      // small error range & timeout
    0, 0,      // large error range & timeout
    0          // max slew
);

// Odometry settings
lemlib::OdomSensors sensors(&vertical_tracking_wheel, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &horizontal_tracking_wheel, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// Drivetrain settings
lemlib::Drivetrain drivetrain(&left_motor_group, // left motor group
                              &right_motor_group, // right motor group
                              10.75, // 10 inch track width
                              lemlib::Omniwheel::NEW_275, // using the 2.75 omnis
                              600, // drivetrain rpm is 360
                              0 // horizontal drift is 2 (for now)
);

// Create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);
