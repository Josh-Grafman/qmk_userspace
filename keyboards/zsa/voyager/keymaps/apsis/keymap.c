#include "config.h"
#include QMK_KEYBOARD_H

#ifndef LAYER_ENUM_H
#define LAYER_ENUM_H

enum layers {
    BSE,
    NAV,
    SYM,
    NUM,
    MSE,
};

#endif /* LAYER_ENUM_H */

bool led_update_user(led_t led_state) {
    static uint8_t num_state = 0;
    static uint8_t scrl_state = 0;
    scrl_state = led_state.scroll_lock;
    if (num_state != led_state.num_lock) {
        if (led_state.num_lock) {
            layer_on(MSE);
        } else {
            layer_off(MSE);
            if (scrl_state) {
                tap_code(KC_SCRL);
            }
        }
        num_state = led_state.num_lock;
    }
    return true;
}