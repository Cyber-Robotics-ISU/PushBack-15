#pragma once

#include <vector>
#include <string>
#include <cmath>

#include "liblvgl/lvgl.h"

#include "autons.hpp"
#include "global.hpp"
#include "driver_profile.hpp"

// LVGL Screens
void create_main_screen();
void create_auton_color_screen();
void create_auton_screen();
void create_profiles_screen();

// Auton helpers
void auton_red_select();
void auton_blue_select();
void auton_skills_select();
void updateAutonList();
