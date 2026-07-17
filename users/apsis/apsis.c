#include QMK_KEYBOARD_H

#include "apsis.h"
#include "oneshot.h"
#include "oneshot_fn.h"
#include "swapper.h"

#define FWD    G(KC_RBRC)
#define BACK   G(KC_LBRC)
#define TABL   LCTL(KC_PGUP)
#define TABR   LCTL(KC_PGDN)
#define SPCL   A(G(KC_LEFT))
#define SPC_R  A(G(KC_RGHT))
#define SSHOT  G(S(KC_S))
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)
#define LA_EXT MO(EXT)

#define UNDO  LCTL(KC_Z)
#define CUT   LCTL(KC_X)
#define COPY  LCTL(KC_C)
#define PASTE LCTL(KC_V)
#define BKWRD LCTL(KC_BACKSPACE)
#define QUIT  A(KC_F4)

#include "g/keymap_combo.h"

const key_override_t ques_exlm_override = ko_make_basic(MOD_MASK_SHIFT, KC_QUES, KC_EXLM); // S ? -> !
const key_override_t comm_semi_override = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_SCLN); // S , -> ;
const key_override_t dot_coln_override = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLN);   // S . -> :
const key_override_t dash_slsh_override = ko_make_basic(MOD_MASK_SHIFT, KC_MINS, KC_SLSH); // S - -> /

const key_override_t *key_overrides[] = {
    &ques_exlm_override,
    &comm_semi_override,
    &dot_coln_override,
    &dash_slsh_override,
};

/*
wishlist

- ins
- TABL/TABR
- MEH/HYPER
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT_thirtyfour(
        KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,            KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,
        KC_A,    KC_R,    KC_S,    KC_T,    KC_D,            KC_H,    KC_N,    KC_E,    KC_I,    KC_O,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,            KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_QUES,
                                       LA_NAV,  KC_LSFT, KC_SPC,  LA_SYM
    ),

    [NAV] = LAYOUT_thirtyfour(
        LA_EXT,  XXXXXXX, XXXXXXX, BKWRD,   XXXXXXX,         KC_PGUP, XXXXXXX, KC_UP,   XXXXXXX, QUIT,
        OS_GUI,  OS_ALT,  OS_SHFT, OS_CTRL, XXXXXXX,         KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,
        UNDO,    CUT,     COPY,    PASTE,   XXXXXXX,         XXXXXXX, KC_HOME, KC_END,  XXXXXXX, XXXXXXX,
                                       _______, _______, _______, _______
    ),

    [SYM] = LAYOUT_thirtyfour(
        KC_BSLS, KC_LBRC, KC_LCBR, KC_LPRN, KC_LABK,         KC_RABK, KC_RPRN, KC_RCBR, KC_RBRC, KC_SLSH,
        KC_MINS, KC_ASTR, KC_PLUS, KC_EQL,  KC_DLR,          KC_HASH, OS_CTRL, OS_SHFT, OS_ALT,  OS_GUI,
        XXXXXXX, XXXXXXX, KC_PERC, KC_PIPE, KC_AT,           KC_GRV,  XXXXXXX, KC_AMPR, KC_CIRC, KC_TILD,
                                       _______, _______, _______, _______
    ),

    [NUM] = LAYOUT_thirtyfour(
        XXXXXXX, KC_EQL,  KC_SLSH, KC_ASTR, XXXXXXX,         XXXXXXX, KC_7,    KC_8,    KC_9,    XXXXXXX,
        OS_GUI,  OS_ALT,  OS_SHFT, OS_CTRL, OS_FN,           KC_0,    KC_1,    KC_2,    KC_3,    XXXXXXX,
        XXXXXXX, KC_DOT,  KC_MINS, KC_PLUS, XXXXXXX,         XXXXXXX, KC_4,    KC_5,    KC_6,    XXXXXXX,
                                       _______, _______, _______, _______
    ),

    [EXT] = LAYOUT_thirtyfour(
        XXXXXXX, KC_MUTE, KC_VOLD, KC_VOLU, KC_SLEP,         XXXXXXX, KC_MPRV, KC_MNXT, KC_MPLY, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, SSHOT,           XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX, KC_BRIU, XXXXXXX, XXXXXXX, XXXXXXX,
                                       LA_NAV,  KC_LSFT,  KC_SPC,  LA_SYM
    ),

    [MSE] = LAYOUT_thirtyfour(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, KC_SCRL, MS_BTN1, MS_BTN2,         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        UNDO,    CUT,     COPY,    PASTE,   XXXXXXX,         XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                       KC_NUM,  _______, XXXXXXX, KC_NUM
    ),
  };

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case LA_EXT:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case LA_EXT:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
    case OS_FN:
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
bool os_fn_pending = false;

uint16_t oneshot_fn_press_user(uint16_t keycode) {
    switch (keycode) {
    case KC_1:
        return KC_F1;
    case KC_2:
        return KC_F2;
    case KC_3:
        return KC_F3;
    case KC_4:
        return KC_F4;
    case KC_5:
        return KC_F5;
    case KC_6:
        return KC_F6;
    case KC_7:
        return KC_F7;
    case KC_8:
        return KC_F8;
    case KC_9:
        return KC_F9;
    case KC_EQL:
        return KC_F10;
    case KC_SLSH:
        return KC_F11;
    case KC_ASTR:
        return KC_F12;
    default:
        return KC_NO;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == XC_UNDS) {
        if (record->event.pressed) {
            uint8_t mods = get_mods();
            if (mods & MOD_MASK_SHIFT) {
                unregister_mods(mods & MOD_MASK_SHIFT);
                send_unicode_string("\xE2\x80\x94");
                register_mods(mods & MOD_MASK_SHIFT);
            } else {
                tap_code16(KC_UNDS);
            }
        }
        return false;
    }

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

    if (!update_oneshot_fn(&os_fn_pending, OS_FN, keycode, record)) {
        return false;
    }

    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    os_fn_pending = false;
    return update_tri_layer_state(state, SYM, NAV, NUM);
}

