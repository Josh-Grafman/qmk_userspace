#pragma once

#ifndef LAYER_ENUM_H
#define LAYER_ENUM_H

enum layers {
    DEF,
    NAV,
    SYM,
    NUM,
    EXT,
    MSE,
};

#endif /* LAYER_ENUM_H */

enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
    OS_FN,
    SC_HOLD,
    XC_UNDS,
    SW_WIN,  // Switch to next window         (cmd-tab)
};
