#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"

#include "g/keymap_combo.h"

#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TABL G(S(KC_LBRC))
#define TABR G(S(KC_RBRC))
#define SPCL A(G(KC_LEFT))
#define SPC_R A(G(KC_RGHT))
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

#define UNDO  LCTL(KC_Z)
#define CUT   LCTL(KC_X)
#define COPY  LCTL(KC_C)
#define PASTE LCTL(KC_V)

enum layers {
    BSE,
    SYM,
    NAV,
    NUM,
};

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_GUI,

    SW_WIN,  // Switch to next window         (cmd-tab)
};

const key_override_t ques_exlm_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUES, KC_EXLM); // S-? -> !

const key_override_t *key_overrides[] = {
	&ques_exlm_override
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BSE] = LAYOUT_thirtyfour(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,            KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_D,            KC_H,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,            KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_QUES,
                                       KC_LSFT, LA_NAV,  LA_SYM,  KC_SPC
    ),

    [SYM] = LAYOUT_thirtyfour(
        KC_ESC,  KC_LBRC, KC_LCBR, KC_LPRN, KC_TILD,         KC_CIRC, KC_RPRN, KC_RCBR, KC_RBRC, KC_GRV,
        KC_MINS, KC_ASTR, KC_EQL,  KC_UNDS, KC_DLR,          KC_HASH, OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_PLUS, KC_PIPE, KC_AT,   KC_BSLS, KC_PERC,         XXXXXXX, KC_AMPR, KC_SCLN, KC_COLN, KC_EXLM,
                                       _______, _______, _______, _______
    ),

    [NAV] = LAYOUT_thirtyfour(
        KC_VOLD, SW_WIN,  TABL,    TABR,    KC_VOLU,         QK_BOOT, HOME,    KC_UP,   END,     KC_DEL,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_NO,           KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, KC_BSPC,
        UNDO,    CUT,     COPY,    PASTE,   KC_MPLY,         XXXXXXX, KC_PGDN, KC_PGUP, KC_NO,   KC_ENT,
                                       _______, _______, _______, _______
    ),

    [NUM] = LAYOUT_thirtyfour(
        KC_7,    KC_5,    KC_3,    KC_1,    KC_9,            KC_8,    KC_0,    KC_2,    KC_4,    KC_6,
        OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_F11,          KC_F10,  OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,
        KC_F7,   KC_F5,   KC_F3,   KC_F1,   KC_F9,           KC_F8,   KC_F12,  KC_F2,   KC_F4,   KC_F6,
                                       _______, _______, _______, _______
    ),
  };

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_gui_state = os_up_unqueued;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LGUI, KC_TAB, SW_WIN,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_gui_state, KC_LCMD, OS_GUI,
        keycode, record
    );

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}
