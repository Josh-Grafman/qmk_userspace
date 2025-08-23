/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "ploopyco.h"

// Dummy
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{ KC_NO }}};

static bool set_mse_layer = false;

void keyboard_post_init_user(void) {
    led_t led_state = host_keyboard_led_state();
    set_mse_layer = led_state.num_lock;
}

bool led_update_user(led_t led_state) {
    set_mse_layer = led_state.num_lock;
    set_drag_scroll(led_state.scroll_lock);
    return true;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    const bool toggle_threshold = 3;
    if (((mouse_report.x >= toggle_threshold || mouse_report.x < -toggle_threshold) || (mouse_report.y >= toggle_threshold || mouse_report.y < -toggle_threshold)) && !set_mse_layer) {
        tap_code(KC_NUM);
        set_mse_layer = true;
    }
    return mouse_report;
}

void suspend_power_down_user(void) {
    // Switch off sensor + LED making trackball unable to wake host
    adns5050_power_down();
}

void suspend_wakeup_init_user(void) {
    adns5050_init();
}