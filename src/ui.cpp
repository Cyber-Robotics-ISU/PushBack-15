#include "ui.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>

#include "liblvgl/lvgl.h"

#include "global.hpp"
#include "autons.hpp"

// ============================================================================
//                               AUTON LOGIC
// ============================================================================

enum class AutonGroup {
    Red,
    Blue,
    Skills
};

static AutonGroup current_auton_group = AutonGroup::Blue;
static std::string last_selected_red = "";
static std::string last_selected_blue = "";
static std::string last_selected_skills = "";

static std::string& selected_name_for_group(AutonGroup group) {
    switch (group) {
        case AutonGroup::Red:
            return last_selected_red;
        case AutonGroup::Blue:
            return last_selected_blue;
        case AutonGroup::Skills:
        default:
            return last_selected_skills;
    }
}

static const char* title_for_group(AutonGroup group) {
    switch (group) {
        case AutonGroup::Red:
            return "Select Red Auton";
        case AutonGroup::Blue:
            return "Select Blue Auton";
        case AutonGroup::Skills:
        default:
            return "Select Skills Auton";
    }
}

static const char* label_for_group(AutonGroup group) {
    switch (group) {
        case AutonGroup::Red:
            return "Red";
        case AutonGroup::Blue:
            return "Blue";
        case AutonGroup::Skills:
        default:
            return "Skills";
    }
}

static bool auton_matches_group(const AutonOption& option, AutonGroup group) {
    switch (group) {
        case AutonGroup::Red:
            return option.side == 0;
        case AutonGroup::Blue:
            return option.side == 1;
        case AutonGroup::Skills:
        default:
            return option.side == 2;
    }
}

static std::string preview_auton_name(AutonGroup group) {
    std::string selected = selected_name_for_group(group);
    if (!selected.empty()) {
        return selected;
    }
    for (auto &a : auton_master_list) {
        if (auton_matches_group(a, group)) {
            return a.name;
        }
    }
    return "None";
}

void updateAutonList() {
    std::string last_selected_name = selected_name_for_group(current_auton_group);

    auton_list.clear();
    for (auto &a : auton_master_list) {
        if (auton_matches_group(a, current_auton_group)) {
            auton_list.push_back(a);
        }
    }

    current_auton_selection = 0;
    if (!last_selected_name.empty()) {
        for (size_t i = 0; i < auton_list.size(); i++) {
            if (last_selected_name == auton_list[i].name) {
                current_auton_selection = i;
                break;
            }
        }
    }

    if (!auton_list.empty()) {
        selected_name_for_group(current_auton_group) = auton_list[current_auton_selection].name;
    }
}

// ============================================================================
//                            GENERAL UI HELPERS
// ============================================================================

lv_obj_t* create_button(lv_obj_t* parent, const char* text, lv_align_t align, int x_ofs, int y_ofs, void (*callback)()) {
    lv_obj_t* btn = lv_button_create(parent);
    lv_obj_set_size(btn, 170, 52); // Standardized size
    lv_obj_set_align(btn, align);
    lv_obj_set_pos(btn, x_ofs, y_ofs);

    lv_obj_set_style_radius(btn, 10, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x384662), LV_PART_MAIN);
    const lv_style_selector_t pressed_selector =
        static_cast<lv_style_selector_t>(LV_PART_MAIN) |
        static_cast<lv_style_selector_t>(LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(
        btn,
        lv_color_hex(0x2A344A),
        pressed_selector
    );
    lv_obj_set_style_border_color(btn, lv_color_hex(0x516080), LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN);

    lv_obj_t* lbl = lv_label_create(btn);
    lv_label_set_text(lbl, text);
    lv_obj_center(lbl);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

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
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    updateAutonList();

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "PushBack Control");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 6);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    std::string redAuton = "Red Auton\n" + preview_auton_name(AutonGroup::Red);
    std::string blueAuton = "Blue Auton\n" + preview_auton_name(AutonGroup::Blue);
    std::string skillsAuton = "Skills Auton\n" + preview_auton_name(AutonGroup::Skills);

    // Auton buttons (left column)
    lv_obj_t* red_btn = create_button(screen, redAuton.c_str(), LV_ALIGN_TOP_LEFT, 12, 40, auton_red_select);
    lv_obj_t* blue_btn = create_button(screen, blueAuton.c_str(), LV_ALIGN_TOP_LEFT, 12, 110, auton_blue_select);
    lv_obj_t* skills_btn = create_button(screen, skillsAuton.c_str(), LV_ALIGN_TOP_LEFT, 12, 180, auton_skills_select);

    lv_obj_t* active_auton_btn = nullptr;
    if (current_auton_group == AutonGroup::Red) {
        active_auton_btn = red_btn;
    } else if (current_auton_group == AutonGroup::Blue) {
        active_auton_btn = blue_btn;
    } else {
        active_auton_btn = skills_btn;
    }
    if (active_auton_btn) {
        lv_obj_set_style_border_color(active_auton_btn, lv_color_hex(0xFF3B30), 0);
        lv_obj_set_style_border_width(active_auton_btn, 4, 0);
    }

    // Profiles column (right side)
    const int profile_start_y = 40;
    const int profile_gap = 70;
    for (size_t i = 0; i < profile_list.size(); i++) {
        const int y_ofs = profile_start_y + static_cast<int>(i) * profile_gap;
        lv_obj_t* btn = lv_button_create(screen);
        lv_obj_set_size(btn, 170, 52);
        lv_obj_set_align(btn, LV_ALIGN_TOP_RIGHT);
        lv_obj_set_pos(btn, -12, y_ofs);
        lv_obj_set_style_radius(btn, 10, LV_PART_MAIN);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x384662), LV_PART_MAIN);
        const lv_style_selector_t pressed_selector =
            static_cast<lv_style_selector_t>(LV_PART_MAIN) |
            static_cast<lv_style_selector_t>(LV_STATE_PRESSED);
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x2A344A), pressed_selector);
        lv_obj_set_style_border_color(btn, lv_color_hex(0x516080), LV_PART_MAIN);
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN);

        if (i == static_cast<size_t>(current_profile_selection)) {
            lv_obj_set_style_border_color(btn, lv_color_hex(0x00E5FF), 0);
            lv_obj_set_style_border_width(btn, 4, 0);
        }

        lv_obj_t* lbl = lv_label_create(btn);
        lv_label_set_text(lbl, profile_list[i].name);
        lv_obj_center(lbl);
        lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

        lv_obj_add_event_cb(btn, [](lv_event_t* e) {
            const int idx = static_cast<int>(reinterpret_cast<intptr_t>(lv_event_get_user_data(e)));
            if (idx < 0 || idx >= static_cast<int>(profile_list.size())) return;
            current_profile_selection = idx;
            create_main_screen();
        }, LV_EVENT_CLICKED, reinterpret_cast<void*>(static_cast<intptr_t>(i)));
    }

    // Bottom Left: Odometry
    //create_button(screen, "Odometry", LV_ALIGN_BOTTOM_LEFT, 20, -20, create_pid_screen);

    // Bottom Right: PID Tuning
    //create_button(screen, "PID Tuning", LV_ALIGN_BOTTOM_RIGHT, -20, -20, create_pid_screen);
}

// ============================================================================
//                            AUTON SELECTION
// ============================================================================

void auton_red_select() {
    const bool same_group = current_auton_group == AutonGroup::Red;
    current_auton_group = AutonGroup::Red;
    autonColor = -1; 
    updateAutonList();
    if (same_group && !auton_list.empty()) {
        current_auton_selection = (current_auton_selection + 1) % auton_list.size();
        selected_name_for_group(current_auton_group) = auton_list[current_auton_selection].name;
    }
    create_main_screen();
}

void auton_blue_select() {
    const bool same_group = current_auton_group == AutonGroup::Blue;
    current_auton_group = AutonGroup::Blue;
    autonColor = 1; 
    updateAutonList();
    if (same_group && !auton_list.empty()) {
        current_auton_selection = (current_auton_selection + 1) % auton_list.size();
        selected_name_for_group(current_auton_group) = auton_list[current_auton_selection].name;
    }
    create_main_screen();
}

void auton_skills_select() {
    const bool same_group = current_auton_group == AutonGroup::Skills;
    current_auton_group = AutonGroup::Skills;
    autonColor = 0;
    updateAutonList();
    if (same_group && !auton_list.empty()) {
        current_auton_selection = (current_auton_selection + 1) % auton_list.size();
        selected_name_for_group(current_auton_group) = auton_list[current_auton_selection].name;
    }
    create_main_screen();
}

void create_auton_color_screen() {
    lv_obj_t* screen = lv_obj_create(nullptr);
    lv_screen_load(screen);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "Auton Groups");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t* btnRed = create_button(screen, "RED AUTONS", LV_ALIGN_TOP_LEFT, 20, 40, auton_red_select);
    lv_obj_t* btnBlue = create_button(screen, "BLUE AUTONS", LV_ALIGN_TOP_RIGHT, -20, 40, auton_blue_select);
    lv_obj_t* btnSkills = create_button(screen, "SKILLS AUTON", LV_ALIGN_TOP_MID, 0, 130, auton_skills_select);

    // Highlight Current Selection
    lv_obj_t* activeBtn = nullptr;
    if (current_auton_group == AutonGroup::Red) {
        activeBtn = btnRed;
    } else if (current_auton_group == AutonGroup::Blue) {
        activeBtn = btnBlue;
    } else {
        activeBtn = btnSkills;
    }
    if (activeBtn) {
        lv_obj_set_style_border_color(activeBtn, lv_color_hex(0xFF3B30), 0);
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
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    // Title
    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, title_for_group(current_auton_group));
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    // Data Labels
    lv_obj_t* name_label = lv_label_create(screen);
    const bool has_autons = !auton_list.empty();
    lv_label_set_text(name_label, has_autons ? auton_list[current_auton_selection].name : "None");
    lv_obj_align(name_label, LV_ALIGN_CENTER, 0, -50);
    lv_obj_set_style_text_color(name_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    lv_obj_t* desc_label = lv_label_create(screen);
    lv_label_set_text(desc_label, has_autons ? auton_list[current_auton_selection].description : "No autons in this group.");
    lv_label_set_long_mode(desc_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(desc_label, 200); 
    lv_obj_set_style_text_align(desc_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(desc_label, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_style_text_color(desc_label, lv_color_hex(0xDCE4F2), LV_PART_MAIN);

    struct AutonUiContext {
        lv_obj_t* name_label;
        lv_obj_t* desc_label;
        AutonGroup group;
    };

    struct AutonNavContext {
        AutonUiContext* ui;
        int delta;
    };

    auto* ui_ctx = new AutonUiContext{name_label, desc_label, current_auton_group};

    // --- Arrow Logic ---
    lv_obj_t* left = lv_button_create(screen);
    lv_obj_set_size(left, 80, 150);
    lv_obj_align(left, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_bg_opa(left, LV_OPA_TRANSP, 0);
    lv_obj_t* l_lbl = lv_label_create(left);
    lv_label_set_text(l_lbl, LV_SYMBOL_LEFT);
    lv_obj_center(l_lbl);
    
    lv_obj_add_event_cb(left, [](lv_event_t* e) {
        auto* nav = static_cast<AutonNavContext*>(lv_event_get_user_data(e));
        if (auton_list.empty()) return;
        current_auton_selection += nav->delta;
        if (current_auton_selection < 0) current_auton_selection = auton_list.size() - 1;
        if (current_auton_selection >= (int)auton_list.size()) current_auton_selection = 0;
        lv_label_set_text(nav->ui->name_label, auton_list[current_auton_selection].name);
        lv_label_set_text(nav->ui->desc_label, auton_list[current_auton_selection].description);
        selected_name_for_group(nav->ui->group) = auton_list[current_auton_selection].name;
    }, LV_EVENT_CLICKED, new AutonNavContext{ui_ctx, -1});

    lv_obj_t* right = lv_button_create(screen);
    lv_obj_set_size(right, 80, 150);
    lv_obj_align(right, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_set_style_bg_opa(right, LV_OPA_TRANSP, 0);
    lv_obj_t* r_lbl = lv_label_create(right);
    lv_label_set_text(r_lbl, LV_SYMBOL_RIGHT);
    lv_obj_center(r_lbl);

    lv_obj_add_event_cb(right, [](lv_event_t* e) {
        auto* nav = static_cast<AutonNavContext*>(lv_event_get_user_data(e));
        if (auton_list.empty()) return;
        current_auton_selection += nav->delta;
        if (current_auton_selection < 0) current_auton_selection = auton_list.size() - 1;
        if (current_auton_selection >= (int)auton_list.size()) current_auton_selection = 0;
        lv_label_set_text(nav->ui->name_label, auton_list[current_auton_selection].name);
        lv_label_set_text(nav->ui->desc_label, auton_list[current_auton_selection].description);
        selected_name_for_group(nav->ui->group) = auton_list[current_auton_selection].name;
    }, LV_EVENT_CLICKED, new AutonNavContext{ui_ctx, 1});

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
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "Driver Profiles");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    lv_obj_t* name_label = lv_label_create(screen);
    lv_label_set_text(name_label, profile_list[current_profile_selection].name);
    lv_obj_align(name_label, LV_ALIGN_CENTER, 0, -50);
    lv_obj_set_style_text_color(name_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);

    lv_obj_t* desc_label = lv_label_create(screen);
    lv_label_set_text(desc_label, profile_list[current_profile_selection].description);
    lv_label_set_long_mode(desc_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(desc_label, 200);
    lv_obj_set_style_text_align(desc_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(desc_label, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_style_text_color(desc_label, lv_color_hex(0xDCE4F2), LV_PART_MAIN);

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
