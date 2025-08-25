COMBO_ENABLE = yes
VPATH += keyboards/gboards

INTROSPECTION_KEYMAP_C = apsis.c
SRC += oneshot.c
SRC += swapper.c

KEY_OVERRIDE_ENABLE = yes
CAPS_WORD_ENABLE = yes
REPEAT_KEY_ENABLE = yes