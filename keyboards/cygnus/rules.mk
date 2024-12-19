SPLIT_KEYBOARD = yes
# FIRMWARE_FORMAT = uf2
MCU = RP2040
BOOTLOADER = rp2040
# SPLIT_KEYBOARD = yes
# BOARD = GENERIC_RP_RP2040

SERIAL_DRIVER = vendor # needed for half-duplex with TRRS+rp2040
WS2812_DRIVER = vendor


# Space savings
AUDIO_ENABLE = no
MUSIC_ENABLE = no
RGBLIGHT_ENABLE = no
LEADER_ENABLE = no
NKRO_ENABLE = no 
CONSOLE_ENABLE = yes
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
AUTO_SHIFT_ENABLE = no

# enabled features
MOUSEKEY_ENABLE = yes
TAP_DANCE_ENABLE = yes
EXTRAKEY_ENABLE = yes
COMBO_ENABLE = yes
CAPS_WORD_ENABLE = yes
UNICODE_ENABLE = yes
NKRO_ENABLE = yes

LTO_ENABLE = no # must be disabled for rp2040 builds

SRC += features/custom_shift_keys.c
SRC += features/achordion.c
