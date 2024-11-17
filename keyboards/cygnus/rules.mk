SPLIT_KEYBOARD = yes
# FIRMWARE_FORMAT = uf2
BOOTLOADER = rp2040
RD = GENERIC_RP_RP2040
SERIAL_DRIVER = vendor # needed for half-duplex with TRRS+rp2040


# Space savings
AUDIO_ENABLE = no
MUSIC_ENABLE = no
RGBLIGHT_ENABLE = no
LEADER_ENABLE = no
UNICODE_ENABLE = no
NKRO_ENABLE = no 
CONSOLE_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
AUTO_SHIFT_ENABLE = no

MOUSEKEY_ENABLE = yes
TAP_DANCE_ENABLE = yes
EXTRAKEY_ENABLE = yes
COMBO_ENABLE = yes
CAPS_WORD_ENABLE = yes

LTO_ENABLE = no # change if not enough space on board - incompatible with macro and function

SRC += features/custom_shift_keys.c
SRC += features/achordion.c
