#include "driver_controls.hpp"

#include "api.h"
#include "global.hpp"
#include <algorithm>
#include <cstdint>

namespace {
constexpr int kIntakeSpeed = 127;

// Scorer/lever move settings (motor encoder degrees).
constexpr int kScorerMaxSpeed = 40;
constexpr double kScorerDownAngle = 0.0;
constexpr double kScorerUpAngleRetracted = 750.0;
constexpr double kScorerUpAngleExtended = 650.0;
constexpr int kBlockerDownDelayMs = 100;

// If the blocker moves the wrong way, flip this to false.
constexpr bool kBlockerDownIsExtended = true;

bool scorer_up = false;
bool scorer_zeroed = false;
bool lever_up_delay_pending = false;
std::uint32_t lever_up_delay_start_ms = 0;

double scorer_target_for_state(bool is_up, bool extender_extended) {
    if (!is_up) {
        return kScorerDownAngle;
    }
    return extender_extended ? kScorerUpAngleExtended : kScorerUpAngleRetracted;
}

void set_blocker_down(bool down) {
    if (kBlockerDownIsExtended) {
        down ? blocker.extend() : blocker.retract();
    } else {
        down ? blocker.retract() : blocker.extend();
    }
}

void start_lever_up_delay() {
    lever_up_delay_pending = true;
    lever_up_delay_start_ms = pros::millis();
}

void scorer_move_to_state() {
    const double target = scorer_target_for_state(scorer_up, extender.is_extended());
    scorer.move_absolute(target, kScorerMaxSpeed);
}
} // namespace

void driver_controls_init() {
    scorer.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // Coast only the drivetrain when stopped
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    if (!scorer_zeroed) {
        scorer.tare_position(); // assume lever is at the "down" reference on first init
        scorer_zeroed = true;
    }
    scorer_move_to_state();
}

void driver_controls_update() {
    // Intake: R2 forward, L2+R2 reverse
    if (masterController.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        int speed = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_L2) ? -kIntakeSpeed : kIntakeSpeed;
        intake.move(speed);
    } else {
        intake.move(0);
    }

    // Lever toggle on R1 (shift+R1 for extender), sync blocker to lever state
    if (masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
        const bool shift = masterController.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
        if (shift) {
            extender.toggle();
            if (scorer_up) {
                if (!lever_up_delay_pending) {
                    scorer_move_to_state();
                }
            }
        } else {
            scorer_up = !scorer_up;
            if (scorer_up) {
                set_blocker_down(true);
                start_lever_up_delay();
            } else {
                lever_up_delay_pending = false;
                scorer_move_to_state();
                set_blocker_down(false);
            }
        }
    }

    // Scrapper toggle on L1
    if (masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
        scrapper.toggle();
    }

    // Separate blocker toggle on B
    if (masterController.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
        lever_up_delay_pending = false;
        blocker.toggle();
    }

    if (lever_up_delay_pending &&
        (pros::millis() - lever_up_delay_start_ms >= static_cast<std::uint32_t>(kBlockerDownDelayMs))) {
        scorer_move_to_state();
        lever_up_delay_pending = false;
    }

    // No PID hold on the lever
}
