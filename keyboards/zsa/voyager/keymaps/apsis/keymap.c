#include "config.h"
#include QMK_KEYBOARD_H

#include "users/apsis/apsis.h"

#ifdef RGB_MATRIX_ENABLE
extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

static void set_layer_color(uint8_t red, uint8_t green, uint8_t blue) {
    float scale = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
    rgb_matrix_set_color_all(scale * red, scale * green, scale * blue);
}

bool rgb_matrix_indicators_user(void) {
    if (keyboard_config.disable_layer_led) {
        return false;
    }

    switch (biton32(layer_state)) {
    case DEF:
        set_layer_color(0xFB, 0x9B, 0x60);
        break;
    case NAV:
        set_layer_color(0xFF, 0x73, 0x64);
        break;
    case SYM:
        set_layer_color(0x6F, 0x97, 0xD8);
        break;
    case NUM:
        set_layer_color(0xFC, 0xD6, 0x62);
        break;
    case EXT:
        set_layer_color(0xD9, 0x55, 0x55);
        break;
    case MSE:
        set_layer_color(0x9A, 0x9A, 0x9A);
        break;
    default:
        if (rgb_matrix_get_flags() == LED_FLAG_NONE) {
            rgb_matrix_set_color_all(0, 0, 0);
        }
        break;
    }

    return true;
}
#endif

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
