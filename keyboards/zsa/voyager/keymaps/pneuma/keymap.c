// Copyright 2023 ZSA Technology Labs, Inc <@zsa>
// Copyright 2023 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#define MSE_LED caps_lock

static bool initialized = false;
static bool mse_enabled = false;

static led_t initial_led_state;

enum layer_names {
    _BASE = 0,
    _NUM,
    _NAV,
    _MSE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
        CW_TOGG, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
        SFT_T(KC_BSPC),KC_A, KC_S, KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, RSFT_T(KC_QUOT),
        KC_LGUI, ALT_T(KC_Z),KC_X, KC_C,    KC_V,    KC_B,                         KC_N,    KC_M,    KC_COMMA,KC_DOT,  RALT_T(KC_SLSH), KC_RCTL,
                                LT(1,KC_ENT), CTL_T(KC_TAB),           SFT_T(KC_BSPC), LT(2,KC_SPC)
    ),
    [_NUM] = LAYOUT(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
        KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                      KC_7,    KC_8,    KC_9,    KC_MINS, KC_SLSH, KC_F12,
        _______, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,                      KC_4,    KC_5,    KC_6,    KC_PLUS, KC_ASTR, KC_BSPC,
        _______, _______, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR,                      KC_1,    KC_2,    KC_3,    KC_DOT,  KC_EQL,  KC_ENT,
                                                     _______, _______,    _______, KC_0
    ),
    [_NAV] = LAYOUT(
        RM_TOGG, QK_KB,   RM_NEXT, RGB_M_P, RM_VALD, RM_VALU,                      _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, KC_VOLD, KC_VOLU, KC_MUTE, _______,                      KC_PGUP, KC_HOME, KC_UP,   KC_END,  _______, _______,
        _______, KC_MPRV, KC_MNXT, KC_MSTP, KC_MPLY, _______,                      KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,
        _______, _______, _______, _______, _______, _______,                      _______, C(S(KC_TAB)), C(KC_TAB), _______, _______, _______,
                                                     _______, _______,    _______, _______
    ),
    [_MSE] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, MS_LEFT, MS_RGHT,                      _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                      _______, _______, _______, _______, _______, _______,
                                                     TO(_BASE), _______,  _______, _______
    )
};

uint32_t initialization_timeout(uint32_t trigger_time, void *cb_arg) {
    initial_led_state = host_keyboard_led_state();
    initialized = true;
    return 0; // Don't repeat
}

bool led_update_user(led_t led_state) {
    if (!initialized)
        return true;

    if (led_state.MSE_LED != initial_led_state.MSE_LED) {
        layer_move(_MSE);
        mse_enabled = true;
    } else {
        mse_enabled = false;
    }
    return true;
}
