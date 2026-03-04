#include "autons.hpp"
#include "global.hpp"

#include "liblvgl/lvgl.h"
#include "api.h"

#include <vector>  
#include <string> 
#include <cmath>

// Actual autonomous implementations
void auton_left() {
    // Reset pose so 12 inches forward is along +Y
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 12, 2000, {}, false);
}

void auton_right() {
    // Turn 90 degrees to the right from current heading
    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(90, 2000, {}, false);
}

void auton_skills() {

}
