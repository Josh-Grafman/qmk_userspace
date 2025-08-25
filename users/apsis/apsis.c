#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"

#include "g/keymap_combo.h"

#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TABL LCTL(KC_PGUP)
#define TABR LCTL(KC_PGDN)
#define SPCL A(G(KC_LEFT))
#define SPC_R A(G(KC_RGHT))
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

#define UNDO  LCTL(KC_Z)
#define CUT   LCTL(KC_X)
#define COPY  LCTL(KC_C)
#define PASTE LCTL(KC_V)
#define BKWRD LCTL(KC_BACKSPACE)

enum layers {
    BSE,
    NAV,
    SYM,
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
const key_override_t comm_semi_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_SCLN); // S-, -> ;
const key_override_t dot_coln_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLN);   // S-. -> :
const key_override_t slsh_unds_override = ko_make_basic(MOD_MASK_SHIFT, KC_SLSH, KC_UNDS);    // S-/ -> _
// TODO: add dash -> em dash

const key_override_t *key_overrides[] = {
	&ques_exlm_override,
    &comm_semi_override,
    &dot_coln_override,
    &slsh_unds_override
};

/*
wishlist

- media keys (back play skip)
- vol up/down
- ins
- TABL/TABR
- MEH/HYPER
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BSE] = LAYOUT_thirtyfour(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,            KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_D,            KC_H,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,            KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_QUES,
                                       KC_LSFT, LA_NAV,  LA_SYM,  KC_SPC
    ),

    [NAV] = LAYOUT_thirtyfour(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         KC_PGUP, HOME,    KC_UP,   END,     XXXXXXX,
        OS_GUI,  OS_ALT,  OS_SHFT, OS_CTRL, XXXXXXX,         KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,
        UNDO,    CUT,     COPY,    PASTE,   QK_REP,          XXXXXXX, BKWRD,   KC_DEL,  XXXXXXX, XXXXXXX,
                                       _______, _______, _______, _______
    ),

    [SYM] = LAYOUT_thirtyfour(
        KC_BSLS, KC_PIPE, KC_UNDS, KC_AMPR, KC_PERC,         KC_PLUS, KC_MINS, KC_EQL,  KC_ASTR, KC_SLSH,
        KC_LABK, KC_COLN, KC_LCBR, KC_LPRN, KC_LBRC,         KC_DLR,  OS_CTRL, OS_SHFT, OS_ALT,  OS_GUI,
        KC_RABK, KC_SCLN, KC_RCBR, KC_RPRN, KC_RBRC,         XXXXXXX, KC_AT,   KC_HASH, KC_CIRC, KC_TILD,
                                       _______, _______, _______, _______
    ),

    [NUM] = LAYOUT_thirtyfour(
        KC_7,    KC_5,    KC_3,    KC_1,    KC_9,            KC_8,    KC_0,    KC_2,    KC_4,    KC_6,
        OS_GUI,  OS_ALT,  OS_SHFT, OS_CTRL, KC_F11,          KC_F10,  OS_CTRL, OS_SHFT, OS_ALT,  OS_GUI,
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
