#include QMK_KEYBOARD_H
#include "macros.h"

enum layer_names {
    _BASE = 0,
    _NUM,
    _SYM,
    _NAV,
    _FUNC,
    _META
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // done!
  [_BASE] = LAYOUT(
    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                      KC_J,    KC_L,    KC_U,    KC_Y,    QQOT,
    KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                      KC_H,    KC_N,    KC_E,    KC_I,    KC_O,
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_NO,  KC_NO,    KC_K,    KC_M,    CMADSH,  PEREX,   SLUND,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   TT(_SYM),TB_CRL, RT_SHF,   NV_SPC,  KC_NO,   KC_NO,   KC_NO,   KC_NO),

  // done!
  [_NUM] = LAYOUT(
    KC_NO,   KC_PERC, KC_ASTR, KC_SLASH,KC_NO,                     KC_NO,   KC_7,    KC_8,    KC_9,    KC_NO,
    KC_ESC,  KC_EQ,   KC_PLUS, KC_MINUS,KC_NO,                     KC_0,    KC_1,    KC_2,    KC_3,    KC_BSPC,
    KC_NO,   KC_DOT,  KC_COMMA,KC_DLR,  KC_NO,   _____,  _____,    KC_NO,   KC_4,    KC_5,    KC_6,    KC_NO,
    _____,   _____,   _____,   _____,   _____,   _____,  _____,    NV_BSE, _____,   _____,   _____,   _____ ),


  [_SYM] = LAYOUT(
    KC_PERC, KC_DLR,  LABKSL,  KC_RABK, KC_PIPE,                   KC_ASTR, KC_LPRN, KC_RPRN, KC_LBRC, KC_RBRC,
    KC_ESC,  KC_EXLM, KC_PLUS, KC_EQ,   KC_COLN,                   KC_SCLN, KC_LCBR, KC_RCBR, KC_GRAVE,KC_BSPC,
    KC_CIRC, KC_TILD, KC_HASH, KC_AMPR, KC_BSLS, _____,  _____,    KC_AT,   MOJI,_____,_____,  _____,
    _____,   _____,   _____,   _____,   _____,   _____,  _____,    NV_BSE, _____,   _____,   _____,   _____ ),

  [_NAV] = LAYOUT(
    QK_LLCK, TO(_NUM),TO(_FUNC),TO(_META),KC_NO,                   KC_PGUP, TAB_L,    KC_UP,   TAB_R,   LALT(KC_F4),
    KC_ESC,  L_ALT,   L_SHFT,  L_GUI,   LCTL_LSFT_K,               KC_PGDN, KC_LEFT,  KC_DOWN, KC_RIGHT,KC_BSPC,
    KC_DEL,  CUT,     COPY,    PASTE,   KC_BSLS, _____,  _____,    KC_NO,   KC_HOME,  KC_END,  KC_NO,   LSFT(KC_ENTER),
    _____,   _____,   _____,   _____,   _____,   _____,  _____,    TO(_BASE),_____,   _____,   _____,   _____ ),

  [_FUNC] = LAYOUT(
    BOOT,    KC_MUTE, KC_VOLD, KC_VOLU, QK_RBT,                     KC_NO,KC_NO,KC_NO,KC_NO,SLEEP,
    KC_ESC,  KC_PREVT,KC_PLAY, KC_NEXTT,SSHOT,                     KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,
    KC_NO,   KC_NO,   KC_BDN,  KC_BUP,  KC_NO,   _____,  _____,    KC_NO,   KC_NO,KC_NO,KC_NO,KC_NO, 
    _____,   _____,   _____,   _____,   _____,   _____,  _____,    NV_BSE,_____,   _____,   _____,   _____ ),

  [_META] = LAYOUT(
    KC_NO,    KC_NO, KC_NO, KC_NO, KC_NO,                     KC_NO,TAB_L,VS_UP,TAB_R,KC_NO,
    KC_NO,  KC_NO,KC_NO, KC_NO,KC_NO,                     KC_NO,VS_L,VS_DN,VS_R,KC_NO,
    KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,   _____,  _____,    KC_NO,KC_NO,KC_NO,KC_NO,KC_NO, 
    _____,   _____,   _____,   _____,   _____,   TB_CMD,  _____,    NV_BSE,_____,   _____,   _____,   _____ )
};

enum combos {
  FP_SHFT
};

const uint16_t PROGMEM fp_combo[] = {KC_F, KC_P, COMBO_END};

combo_t key_combos[] = {
  [FP_SHFT] = COMBO(fp_combo, KC_LSFT)
};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  os_variant_t os = detected_host_os();
  switch(keycode)
   {
      case QQOT:
         if (record->tap.count && record->event.pressed)
         {
            tap_code16(KC_QUOTE); /* ' on tap */
         }
         else if (record->event.pressed)
         {
            tap_code16(KC_QUES); /* ? on hold */
         }
         return false;
      case CMADSH:
        if (record->tap.count && record->event.pressed)
          {
              tap_code16(KC_COMMA); /* , on tap */
          }
          else if (record->event.pressed)
          {
              tap_code16(KC_MINUS); /* - on hold */
          }
          return false;
      case PEREX:
        if (record->tap.count && record->event.pressed)
          {
              tap_code16(KC_DOT); /* . on tap */
          }
          else if (record->event.pressed)
          {
              tap_code16(KC_EXLM); /* ! on hold */
          }
          return false;
      case SLUND:
        if (record->tap.count && record->event.pressed)
          {
              tap_code16(KC_SLASH); /* / on tap */
          }
          else if (record->event.pressed)
          {
              tap_code16(KC_UNDS); /* _ on hold */
          }
          return false;
      case LABKSL:
        if (record->tap.count && record->event.pressed)
          {
              tap_code16(KC_LABK); /* < on tap */
          }
          else if (record->event.pressed)
          {
              tap_code16(KC_SLASH); /* / on hold */
          }
          return false;
      case NV_BSE:
        if (record->tap.count && record->event.pressed)
          {
              layer_move(_BASE); /* to _BASE on tap */
          }
          else if (record->event.pressed)
          {
              layer_on(_NAV); /* temp _NAV on hold */
          }
          else
          {
              if(!is_layer_locked(_NAV)) {
                  layer_off(_NAV);  /* release _NAV */
              }
          }
          return false;
      case TAB_L:
        if (record->tap.count && record->event.pressed)
          {
            if(os == OS_MACOS) {
              tap_code16(C(S(KC_TAB)));
            } else {
              tap_code16(C(KC_PGUP));
            }
          }
          return false;
      case TAB_R:
        if (record->tap.count && record->event.pressed)
          {
            if(os == OS_MACOS) {
              tap_code16(C(KC_TAB));
            } else {
              tap_code16(LCTL(KC_PGDN));
            }
          }
          return false;
      case TB_CRL:
        // turn into tab/cmd on Mac
        if (os == OS_MACOS) {
          if (record->tap.count && record->event.pressed) {
            tap_code16(KC_TAB);
          } else if (record->event.pressed) {
            register_code(KC_LGUI);
          } else if(!record->tap.count) {
            unregister_code(KC_LGUI);
          }
          return false;
        }
      case MOJI:
        if (os == OS_MACOS) {
          if (record->tap.count && record->event.pressed) {
            tap_code16(C(LGUI(KC_SPACE)));
          }
        }
      case L_GUI:
        if (os == OS_MACOS) {
          if (record->event.pressed) {
            register_code(KC_LCTL);
          } else if(!record->tap.count) {
            unregister_code(KC_LCTL);
          }
          return false;
        }
      case VS_UP:
        if (os == OS_MACOS) {
          if (record->event.pressed) {
            tap_code16(C(LGUI(KC_UP)));
          }
          return false;
        }
   }
   return true;
}
