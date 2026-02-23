#pragma once
#include <vector>  
#include <string> 
#include <cmath>
#include "liblvgl/lvgl.h"
#include "api.h"

#include "global.hpp"



// Helper for pneumatics (since they are instant toggles, not continuous states)
void togglePneumatics();