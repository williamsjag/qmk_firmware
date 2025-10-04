/*
This is the c configuration file for the keymap

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

see <http://www.gnu.org/licenses/>.
*/

#pragma once

/* Select hand configuration */
#define MASTER_LEFT

//Serial settings
#define USE_SERIAL
#define SOFT_SERIAL_PIN GP1
#define SERIAL_USE_MULTI_TRANSACTION
#define SERIAL_PIO_USE_PIO1

#define TAPPING_TOGGLE 2
#define TAPPING_TERM 250
#define TAPPING_TERM_PER_KEY #define TAPPING_TERM_PER_KEY 1


/* Home row mods */
#define TAP_CODE_DELAY 10
#define TAP_HOLD_CAPS_DELAY 10
#define PERMISSIVE_HOLD
#define CHORDAL_HOLD

#define CAPS_WORD_INVERT_ON_SHIFT

/* Don't apply custom shift keys on layer 0 (QWERTY) */
#define CUSTOM_SHIFT_KEYS_LAYER_MASK ~(1 << 1)
// Don't apply custom shift keys with command
#define CUSTOM_SHIFT_KEYS_NEGMODS MOD_BIT(KC_LGUI)

// #define ACHORDION_STREAK

#define COMBO_TERM_PER_COMBO 1
#define COMBO_MUST_PRESS_IN_ORDER_PER_COMBO
#define COMBO_MUST_TAP_PER_COMBO

