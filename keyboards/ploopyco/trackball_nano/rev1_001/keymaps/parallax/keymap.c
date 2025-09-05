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
#include "hires_dragscroll.h"

// Dummy
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{ KC_NO }}};

#define MOTION_DELAY 20
#define IDLE_DELAY   3000

typedef enum {
    STATE_IDLE,
    STATE_PENDING,
    STATE_ACTIVE,
    STATE_STOPPING
} motion_state_t;

static motion_state_t state = STATE_IDLE;
static uint32_t state_timer = 0;

bool led_update_user(led_t led_state) {
    if(led_state.scroll_lock && !is_hires_dragscroll_on()) {
        hires_dragscroll_on();
    } else if (!led_state.scroll_lock && is_hires_dragscroll_on()) {
        hires_dragscroll_off();
    }

    // Sync our motion state to NumLock
    if (led_state.num_lock) {
        // Host reports NumLock ON → treat as ACTIVE
        if (state != STATE_ACTIVE) {
            state = STATE_ACTIVE;
        }
    } else {
        // Host reports NumLock OFF → treat as IDLE
        if (state != STATE_IDLE) {
            state = STATE_IDLE;
        }
    }

    return true;
}

static inline bool is_numlock_on(void) {
    return host_keyboard_led_state().num_lock;
}

static inline bool is_scrolllock_on(void) {
    return host_keyboard_led_state().scroll_lock;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    int dx = mouse_report.x;
    int dy = mouse_report.y;

    switch (state) {
        case STATE_IDLE:
            if (dx || dy) {
                state = STATE_PENDING;
                state_timer = timer_read();
            }
            break;

        case STATE_PENDING:
            if (!(dx || dy)) {
                state = STATE_IDLE; // false alarm
            } else if (timer_elapsed(state_timer) > MOTION_DELAY) {
                // Sustained motion → request NumLock ON
                if (!is_numlock_on()) {
                    tap_code(KC_NUM);
                }
                state = STATE_ACTIVE;
            }
            break;

        case STATE_ACTIVE:
            if (!(dx || dy)) {
                state = STATE_STOPPING;
                state_timer = timer_read();
            }
            break;

        case STATE_STOPPING:
            if (dx || dy) {
                state = STATE_ACTIVE; // resumed
            } else if (!is_scrolllock_on() && timer_elapsed(state_timer) > IDLE_DELAY) {
                // Idle confirmed → request NumLock OFF
                if (is_numlock_on()) {
                    tap_code(KC_NUM);
                }
                state = STATE_IDLE;
            }
            break;
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
