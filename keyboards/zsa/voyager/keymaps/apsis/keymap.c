#include "config.h"
#include QMK_KEYBOARD_H

#include "users/apsis/apsis.h"

#ifdef RGB_MATRIX_ENABLE
extern rgb_config_t rgb_matrix_config;

void keyboard_post_init_user(void) {
    rgb_matrix_enable();
}

static void set_layer_hsv(uint8_t hue, uint8_t sat) {
    HSV hsv = {
        .h = hue,
        .s = sat,
        .v = rgb_matrix_config.hsv.v,
    };
    RGB rgb = hsv_to_rgb(hsv);

    rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
}

static void set_mse_layer_hsv(uint8_t hue, uint8_t sat) {
    HSV hsv = {
        .h = hue,
        .s = sat,
        .v = rgb_matrix_config.hsv.v,
    };
    RGB rgb = hsv_to_rgb(hsv);

    for (int i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        if (i >= 26) {
            rgb_matrix_set_color(i, 0, 0, 0);
        } else {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
}

bool rgb_matrix_indicators_user(void) {
    if (keyboard_config.disable_layer_led) {
        return false;
    }

    switch (biton32(layer_state)) {
    case DEF:
        set_layer_hsv(18, 255);
        break;
    case NAV:
        set_layer_hsv(8, 255);
        break;
    case SYM:
        set_layer_hsv(150, 200);
        break;
    case NUM:
        set_layer_hsv(38, 255);
        break;
    case EXT:
        set_layer_hsv(0, 255);
        break;
    case MSE:
        set_mse_layer_hsv(0, 0);
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
