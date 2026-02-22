#include "ui.hpp"
#include <vector>
#include <string>
#include <cmath>

#include "liblvgl/lvgl.h"

#include "global.hpp"
#include "autons.hpp"  

// ============================================================================
//                               AUTON LOGIC
// ============================================================================

void updateAutonList() {
    std::string last_selected_name = "";
    if (!auton_list.empty() && current_auton_selection >= 0 && current_auton_selection < auton_list.size()) {
        last_selected_name = auton_list[current_auton_selection].name;
    }

    auton_list.clear();

    for (auto &a : auton_master_list) {
        if (a.side == 2) {
            auton_list.push_back(a); 
        } else if (a.side == 1 && autonColor == 1) {
            auton_list.push_back(a); 
        } else if (a.side == 0 && autonColor == -1) {
            auton_list.push_back(a); 
        }
    }

    current_auton_selection = 0; 
    for (size_t i = 0; i < auton_list.size(); i++) {
        if (last_selected_name == auton_list[i].name) {
            current_auton_selection = i;
            break; 
        }
    }
}

// ============================================================================
//                            GENERAL UI HELPERS
// ============================================================================

lv_obj_t* create_button(lv_obj_t* parent, const char* text, lv_align_t align, int x_ofs, int y_ofs, void (*callback)()) {
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_size(btn, 180, 70); // Standardized size
    lv_obj_set_align(btn, align);
    lv_obj_set_pos(btn, x_ofs, y_ofs);

    lv_obj_t* lbl = lv_label_create(btn);
    lv_label_set_text(lbl, text);
    lv_obj_center(lbl);

    lv_obj_add_event(btn, [](lv_event_t* e) {
        auto func = reinterpret_cast<void (*)()>(lv_event_get_user_data(e));
        if (func) func();
    }, LV_EVENT_CLICKED, reinterpret_cast<void*>(callback));

    return btn;
}

// ============================================================================
//                                MAIN SCREEN
// ============================================================================

void create_main_screen() {
    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_screen_load(screen);

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x222244), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    // Generate Auton Button Text
    std::string autonInfo = "Auton Select\n";
    if (auton_list.empty()) {
        autonInfo += "(None)";
    } else {
        if (autonColor == 1) autonInfo += "[BLUE] ";
        else if (autonColor == -1) autonInfo += "[RED] ";
        else autonInfo += "[?] ";

        if (current_auton_selection >= 0 && current_auton_selection < auton_list.size()) {
            autonInfo += auton_list[current_auton_selection].name;
        } else {
            autonInfo += "Unknown";
        }
    }

    // Top Left: Auton Select
    create_button(screen, autonInfo.c_str(), LV_ALIGN_TOP_LEFT, 20, 20, create_auton_color_screen);

    // Top Right: Profiles
    create_button(screen, "Profiles", LV_ALIGN_TOP_RIGHT, -20, 20, create_profiles_screen);

    // Bottom Left: Odometry
    //create_button(screen, "Odometry", LV_ALIGN_BOTTOM_LEFT, 20, -20, create_pid_screen);

    // Bottom Right: PID Tuning
    //create_button(screen, "PID Tuning", LV_ALIGN_BOTTOM_RIGHT, -20, -20, create_pid_screen);
}

// ============================================================================
//                            AUTON SELECTION
// ============================================================================

void auton_red_select() {
    autonColor = -1; 
    updateAutonList(); 
    create_auton_screen();
}

void auton_blue_select() {
    autonColor = 1; 
    updateAutonList();
    create_auton_screen();
}

void create_auton_color_screen() {
    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_screen_load(screen);

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x223355), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "Choose Side");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t* btnRed = create_button(screen, "RED AUTONS", LV_ALIGN_LEFT_MID, 30, 0, auton_red_select);
    lv_obj_t* btnBlue = create_button(screen, "BLUE AUTONS", LV_ALIGN_RIGHT_MID, -30, 0, auton_blue_select);

    // Highlight Current Selection
    lv_obj_t* activeBtn = (autonColor == -1) ? btnRed : (autonColor == 1) ? btnBlue : nullptr;
    if (activeBtn) {
        lv_obj_set_style_border_color(activeBtn, lv_color_hex(0xFFFF00), 0);
        lv_obj_set_style_border_width(activeBtn, 4, 0);
    }

    lv_obj_t* back = lv_button_create(screen);
    lv_obj_set_size(back, 120, 50);
    lv_obj_set_align(back, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(back, -10);

    lv_obj_t* blabel = lv_label_create(back);
    lv_label_set_text(blabel, "Back");
    lv_obj_center(blabel);

    lv_obj_add_event_cb(back, [](lv_event_t* e) { create_main_screen(); }, LV_EVENT_CLICKED, nullptr);
}

void create_auton_screen() {
    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_screen_load(screen);

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x223355), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    // Title
    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "Select Auton Routine");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Data Labels
    lv_obj_t* name_label = lv_label_create(screen);
    lv_label_set_text(name_label, auton_list[current_auton_selection].name);
    lv_obj_align(name_label, LV_ALIGN_CENTER, 0, -50);

    lv_obj_t* desc_label = lv_label_create(screen);
    lv_label_set_text(desc_label, auton_list[current_auton_selection].description);
    lv_label_set_long_mode(desc_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(desc_label, 200); 
    lv_obj_set_style_text_align(desc_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(desc_label, LV_ALIGN_CENTER, 0, 10);

    // Arrows
    auto create_arrow = [&](const char* symbol, lv_align_t align, int d) {
        lv_obj_t* btn = lv_button_create(screen);
        lv_obj_set_size(btn, 60, 100);
        lv_obj_set_align(btn, align);
        lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
        
        lv_obj_t* l = lv_label_create(btn);
        lv_label_set_text(l, symbol);
        lv_obj_center(l);
        
        lv_obj_add_event_cb(btn, [](lv_event_t* e) {
            lv_obj_t** labels = (lv_obj_t**)lv_event_get_user_data(e);
            // d is hidden in the event user data pointer math (hacky but standard for simple C callbacks)
            // But here we need to capture 'd'. Since this is a lambda, we can't easily pass 'd' via user_data without a struct.
            // Simplified: We will just inline the logic in a normal loop or split functions.
            // For safety, let's revert to the robust style used in profiles.
        }, LV_EVENT_CLICKED, nullptr);
        return btn;
    };

    // --- Arrow Logic Inline ---
    lv_obj_t* left = lv_button_create(screen);
    lv_obj_set_size(left, 80, 150);
    lv_obj_align(left, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_opa(left, LV_OPA_TRANSP, 0);
    lv_obj_t* l_lbl = lv_label_create(left);
    lv_label_set_text(l_lbl, LV_SYMBOL_LEFT);
    lv_obj_center(l_lbl);
    
    lv_obj_add_event_cb(left, [](lv_event_t* e) {
         lv_obj_t** labels = (lv_obj_t**)lv_event_get_user_data(e);
         current_auton_selection--;
         if (current_auton_selection < 0) current_auton_selection = auton_list.size() - 1;
         lv_label_set_text(labels[0], auton_list[current_auton_selection].name);
         lv_label_set_text(labels[1], auton_list[current_auton_selection].description);
    }, LV_EVENT_CLICKED, new lv_obj_t*[2]{name_label, desc_label});

    lv_obj_t* right = lv_button_create(screen);
    lv_obj_set_size(right, 80, 150);
    lv_obj_align(right, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_set_style_bg_opa(right, LV_OPA_TRANSP, 0);
    lv_obj_t* r_lbl = lv_label_create(right);
    lv_label_set_text(r_lbl, LV_SYMBOL_RIGHT);
    lv_obj_center(r_lbl);

    lv_obj_add_event_cb(right, [](lv_event_t* e) {
         lv_obj_t** labels = (lv_obj_t**)lv_event_get_user_data(e);
         current_auton_selection++;
         if (current_auton_selection >= (int)auton_list.size()) current_auton_selection = 0;
         lv_label_set_text(labels[0], auton_list[current_auton_selection].name);
         lv_label_set_text(labels[1], auton_list[current_auton_selection].description);
    }, LV_EVENT_CLICKED, new lv_obj_t*[2]{name_label, desc_label});

    // Back
    lv_obj_t* back = lv_button_create(screen);
    lv_obj_set_size(back, 100, 50);
    lv_obj_align(back, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_t* blabel = lv_label_create(back);
    lv_label_set_text(blabel, "Back");
    lv_obj_center(blabel);
    lv_obj_add_event_cb(back, [](lv_event_t* e){ create_main_screen(); }, LV_EVENT_CLICKED, nullptr);
}

// ============================================================================
//                            PROFILES SCREEN
// ============================================================================

void create_profiles_screen() {
    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_screen_load(screen);

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x334466), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "Driver Profiles");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t* name_label = lv_label_create(screen);
    lv_label_set_text(name_label, profile_list[current_profile_selection].name);
    lv_obj_align(name_label, LV_ALIGN_CENTER, 0, -50);

    lv_obj_t* desc_label = lv_label_create(screen);
    lv_label_set_text(desc_label, profile_list[current_profile_selection].description);
    lv_label_set_long_mode(desc_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(desc_label, 200);
    lv_obj_set_style_text_align(desc_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(desc_label, LV_ALIGN_CENTER, 0, 10);

    // Left Arrow
    lv_obj_t* left = lv_button_create(screen);
    lv_obj_set_size(left, 80, 150);
    lv_obj_align(left, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_opa(left, LV_OPA_TRANSP, 0);
    lv_obj_t* l_lbl = lv_label_create(left);
    lv_label_set_text(l_lbl, LV_SYMBOL_LEFT);
    lv_obj_center(l_lbl);
    
    lv_obj_add_event_cb(left, [](lv_event_t* e) {
        lv_obj_t** labels = (lv_obj_t**)lv_event_get_user_data(e);
        current_profile_selection--;
        if (current_profile_selection < 0) current_profile_selection = profile_list.size() - 1;
        lv_label_set_text(labels[0], profile_list[current_profile_selection].name);
        lv_label_set_text(labels[1], profile_list[current_profile_selection].description);
    }, LV_EVENT_CLICKED, new lv_obj_t*[2]{name_label, desc_label});

    // Right Arrow
    lv_obj_t* right = lv_button_create(screen);
    lv_obj_set_size(right, 80, 150);
    lv_obj_align(right, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_set_style_bg_opa(right, LV_OPA_TRANSP, 0);
    lv_obj_t* r_lbl = lv_label_create(right);
    lv_label_set_text(r_lbl, LV_SYMBOL_RIGHT);
    lv_obj_center(r_lbl);

    lv_obj_add_event_cb(right, [](lv_event_t* e) {
        lv_obj_t** labels = (lv_obj_t**)lv_event_get_user_data(e);
        current_profile_selection++;
        if (current_profile_selection >= (int)profile_list.size()) current_profile_selection = 0;
        lv_label_set_text(labels[0], profile_list[current_profile_selection].name);
        lv_label_set_text(labels[1], profile_list[current_profile_selection].description);
    }, LV_EVENT_CLICKED, new lv_obj_t*[2]{name_label, desc_label});

    // Back Button
    lv_obj_t* back = lv_button_create(screen);
    lv_obj_set_size(back, 100, 50);
    lv_obj_align(back, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_t* blabel = lv_label_create(back);
    lv_label_set_text(blabel, "Back");
    lv_obj_center(blabel);
    lv_obj_add_event_cb(back, [](lv_event_t* e){ create_main_screen(); }, LV_EVENT_CLICKED, nullptr);
}