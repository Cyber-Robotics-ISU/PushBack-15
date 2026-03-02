#include "driver_controls.hpp"

#include "api.h"
#include "global.hpp"

namespace {
constexpr int kIntakeSpeed = 127;
constexpr int kScorerToggleSpeed = 90;
constexpr double kScorerToggleDegrees = 1000.0;

// If the blocker moves the wrong way, flip this to false.
constexpr bool kBlockerDownIsExtended = true;

bool scorer_up = false;

void set_blocker_down(bool down) {
    if (kBlockerDownIsExtended) {
        down ? blocker.extend() : blocker.retract();
    } else {
        down ? blocker.retract() : blocker.extend();
    }
}
} // namespace

void driver_controls_init() {
    scorer.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // Coast only the drivetrain when stopped
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
}

void driver_controls_update() {
    // Intake: R2 forward, L2+R2 reverse
    if (masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        int speed = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_L2) ? -kIntakeSpeed : kIntakeSpeed;
        intake.move(speed);
    } else {
        intake.move(0);
    }

    // Lever toggle on R1, sync blocker to lever state
    if (masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
        scorer_up = !scorer_up;
        double delta = scorer_up ? kScorerToggleDegrees : -kScorerToggleDegrees;
        scorer.move_relative(delta, kScorerToggleSpeed);
        set_blocker_down(scorer_up);
    }

    // Extender toggle on L1
    if (masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
        extender.toggle();
    }

    // Separate blocker toggle on B
    if (masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
        blocker.toggle();
    }
}
