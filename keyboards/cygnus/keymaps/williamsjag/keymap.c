// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "cygnus.h"
#include "features/custom_shift_keys.h"
#include "features/achordion.h"


#define ____ KC_TRNS


// Custom keycode definitions found in cygnus.h

///////////////////////////////////////////////////////////////////////////////
// Tap Dance (https://docs.qmk.fm/features/tap_dance)
///////////////////////////////////////////////////////////////////////////////


// Define esc_layer_boot
void dance_esc_layer_boot(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        // Single tap: Send KC_ESC
        tap_code(KC_ESC);
    } else if (state->count == 3) {
        // Triple tap: Toggle Layer 1 and type text
        if (layer_state_is(1)) {
            // If Layer 1 is already active, switch to Layer 0
            layer_invert(1);
            SEND_STRING("QWERTY");
            wait_ms(500);
            for (int i = 0; i < 6; i++) {
                tap_code(KC_BSPC);
            }
        } else {
            // If Layer 1 is inactive, switch to Layer 1
            layer_invert(1);
            SEND_STRING("HD Vibranium");
            wait_ms(500);
            for (int i = 0; i < 12; i++) {
                tap_code(KC_BSPC);
            }
        }
    } else if (state->count == 5) {
        // Five taps: Send QK_BOOT (reboot to bootloader)
        reset_keyboard();
    }
}

static void sentence_end(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        
        // Tapping TD_DOT produces
        // ". <one-shot-shift>" i.e. dot, space and capitalize next letter.
        // This helps to quickly end a sentence and begin another one
        // without having to hit shift.
        case 1:
            /* Check that Shift is inactive */
            if (!(get_mods() & MOD_MASK_SHIFT)) {
                tap_code(KC_DOT);
                tap_code(KC_SPC);
                /* Internal code of OSM(MOD_LSFT) */
                add_oneshot_mods(MOD_BIT(KC_LEFT_SHIFT));

            } else {
                // send normal (shifted) keycode
                tap_code(KC_DOT);
            }
            break;
        case 2:
            // Type a single dot
            tap_code(KC_DOT);
            break;
        // Three dots ellipsis.
        case 3:
            // type a dot
            tap_code(KC_DOT);
            // add a second dot
            tap_code(KC_DOT);
            // tap the third dot
            tap_code(KC_DOT);
            break;
    }
};

void sentence_end_finished (tap_dance_state_t *state, void *user_data) {
}

// Define Tap Dance Actions
tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_LAYER_BOOT] = ACTION_TAP_DANCE_FN(dance_esc_layer_boot),
    [TD_DOT] = ACTION_TAP_DANCE_FN(sentence_end),
};

// Not strictly a tap dance, but related
// Helper for implementing tap vs. long-press keys if I ever implement any. Given a tap-hold
// key event, replaces the hold function with `long_press_keycode`.
// static bool process_tap_or_long_press_key(
//     keyrecord_t* record, uint16_t long_press_keycode) {
//   if (record->tap.count == 0) {  // Key is being held.
//     if (record->event.pressed) {
//       tap_code16(long_press_keycode);
//     }
//     return false;  // Skip default handling.
//   }
//   return true;  // Continue default handling.
//}

///////////////////////////////////////////////////////////////////////////////
// Caps Word (https://docs.qmk.fm/features/caps_word)
///////////////////////////////////////////////////////////////////////////////

bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
    case KC_MINS:
    case Q_QU:
    case HC_AU:
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_UNDS:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}

///////////////////////////////////////////////////////////////////////////////
// Hands Down Adaptive Keys (https://sites.google.com/alanreiser.com/handsdown/home/hands-down-neu)
///////////////////////////////////////////////////////////////////////////////

// Adaptive graveyard
// MAGIC HASH

//         case HD_HASH: // Magic HD_HASH
//             switch (prior_keycode) {
//                 case HD_D: // "does"
//                     SEND_STRING("oes");
//                     return_state = false;
//                     break;
//                 case HD_F: // "for"
//                     SEND_STRING("or");
//                     return_state = false;
//                     break;
//                 case HD_H: // "have"
//                     SEND_STRING("ave");
//                     return_state = false;
//                     break;
//                 case HD_J: // "just"
//                     SEND_STRING("ust");
//                     return_state = false;
//                     break;
//                 case HD_K: // "know"
//                     SEND_STRING("now");
//                     return_state = false;
//                     break;
//                 case HD_M: // "ment"
//                     SEND_STRING("ent");
//                     return_state = false;
//                     break;
//                 case HD_S: // "sion"
//                     SEND_STRING("ion");
//                     return_state = false;
//                     break;
//                 case HD_T: // "tion"
//                     SEND_STRING("ion");
//                     return_state = false;
//                     break;
//                 case HD_W: // "williams"
//                     SEND_STRING("illiams");
//                     return_state = false;
//                     break;
//                 case HD_SPC: // "and"
//                     SEND_STRING("and");
//                     return_state = false;
//                     break;


///////////////////////////////////////////////////////////////////////////////
// Custom shift keys (https://getreuer.info/posts/keyboards/custom-shift-keys)
///////////////////////////////////////////////////////////////////////////////

// Define table of shift modifications
const custom_shift_key_t custom_shift_keys[] = {
    {HD_MINS, HD_PLUS}, // Shift - is +
    {HD_COMM, HD_SCLN}, // Shift , is ;
    {HD_HASH, HD_DLR}, // Shift # is $
    {TD(TD_DOT), HD_COLN}, // Shift . is :
    {HD_SLSH, HD_ASTR}, // Shift / is *
    {HD_DQUO, HD_LBRC}, // Shift " is [
    {HD_QUOT, HD_RBRC}, // Shift ' is ]
    {HN_HOME, MS_BTN1}, // Shift Home on _NUM is L click
    {HN_END, MS_BTN2}, // Shift End on _NUM is R click
    {HN_RGHT, MS_RGHT}, // Shift arrown os _NUM is mouse movement
    {HN_LEFT, MS_LEFT},
    {HN_UP, MS_UP},
    {HN_DOWN, MS_DOWN}, 
};

uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

///////////////////////////////////////////////////////////////////////////////
// Combos (https://docs.qmk.fm/features/combo)
///////////////////////////////////////////////////////////////////////////////

// Punctuation and function
#define HD_pipe_keys      HD_A, HD_E // Type "|"
#define HD_Screencap_keys HD_A, HD_I // Capture screen
#define HD_wcap_keys      HD_T, HD_A // Toggle caps word
#define HD_ques_keys      HD_SLSH, HD_DQUO // ?
#define HD_exlm_keys      TD(TD_DOT), HD_SLSH // !
#define HD_guilmet_keys HD_DQUO, HD_QUOT // « | »

// h digraph combos - all use original key + neighbor
#define HD_Th_keys HD_T, HD_N // Type "th"
#define HD_Sh_keys HD_S, HD_N // Type "sh"
#define HD_Ch_keys HD_C, HD_S // Type "ch"
#define HD_Wh_keys HD_W, HD_X // Type "wh"
#define HD_Ph_keys HD_P, HD_F // Type "ph"
#define HD_Gh_keys HD_G, HD_M // Type "gh"
#define HD_Sch_keys HD_S, HD_C, HD_N // Type "sch"

// Common words
#define every_keys  KC_BSPC, HD_E // Type "every"
#define here_keys   KC_BSPC, HD_H // Type "here"
#define in_the_keys KC_BSPC, HD_I, HD_T // Type "in the"
#define dcom_keys    HD_U,    HD_O // Type ".com"
#define dfr_keys     HD_O,    HD_Y // Type ".fr"

// "Adaptive keys""
#define au_keys      HD_A, HD_H // Type "au"
#define ua_keys      HD_U, HD_H // Type "ua"
#define eo_keys      HD_E, HD_H // Type "eo"
#define oe_keys      HD_O, HD_H // Type "oe"

// These definitions based on Hands Down Neu & variations
const uint16_t PROGMEM Caps_word_combo[] = {HD_wcap_keys, COMBO_END}; // Toggle Caps Word
const uint16_t PROGMEM Hexlm_combo[]     = {HD_exlm_keys, COMBO_END}; // !
const uint16_t PROGMEM Hques_combo[]     = {HD_ques_keys, COMBO_END}; // ?
const uint16_t PROGMEM Pipe_combo[]      = {HD_pipe_keys, COMBO_END}; // |
const uint16_t PROGMEM Screencap_combo[] = {HD_Screencap_keys, COMBO_END}; // Screen Capture
const uint16_t PROGMEM Guilmet_combo[]   = {HD_guilmet_keys, COMBO_END}; // « | »
// H digraph combos (these are mnemonically arranged- first letter + neighbor)
const uint16_t PROGMEM H_Th_combo[]  = {HD_Th_keys, COMBO_END}; // TYPE "th"
const uint16_t PROGMEM H_Ch_combo[]  = {HD_Ch_keys, COMBO_END}; // TYPE "ch"
const uint16_t PROGMEM H_Wh_combo[]  = {HD_Wh_keys, COMBO_END}; // TYPE "wh"
const uint16_t PROGMEM H_Sh_combo[]  = {HD_Sh_keys, COMBO_END}; // TYPE "sh"
const uint16_t PROGMEM H_Ph_combo[]  = {HD_Ph_keys, COMBO_END}; // TYPE "ph"
const uint16_t PROGMEM H_Gh_combo[]  = {HD_Gh_keys, COMBO_END}; // TYPE "gh"
const uint16_t PROGMEM H_Sch_combo[] = {HD_Sch_keys, COMBO_END}; // TYPE "Sch"
// Common word combos
const uint16_t PROGMEM BSPC_E_COMBO[]   = {every_keys,  COMBO_END};
const uint16_t PROGMEM BSPC_H_COMBO[]   = {here_keys,   COMBO_END};
const uint16_t PROGMEM BSPC_I_COMBO[]   = {in_the_keys, COMBO_END};
const uint16_t PROGMEM DCOM_COMBO[]   = {dcom_keys, COMBO_END};
const uint16_t PROGMEM DFR_COMBO[]   = {dfr_keys, COMBO_END};
// "Adaptive keys"
const uint16_t PROGMEM AU_COMBO[]   = {au_keys,  COMBO_END};
const uint16_t PROGMEM UA_COMBO[]   = {ua_keys,  COMBO_END};
const uint16_t PROGMEM EO_COMBO[]   = {eo_keys,  COMBO_END};
const uint16_t PROGMEM OE_COMBO[]   = {oe_keys,  COMBO_END};

combo_t key_combos[] = {
    COMBO(Screencap_combo, HC_SCAP),
    COMBO(Caps_word_combo, HC_CW_TOGG),
    COMBO(Hexlm_combo, HC_EXLM), // !
    COMBO(Hques_combo, HC_QUES), // ?
    COMBO(Pipe_combo, HC_PIPE), // |
    COMBO(Guilmet_combo, HC_GLMETS), // « | »
    // H bigrams
    COMBO(H_Ch_combo, HC_CH),
    COMBO(H_Sch_combo, HC_SCH),
    COMBO(H_Gh_combo, HC_GH),
    COMBO(H_Ph_combo, HC_PH),
    COMBO(H_Th_combo, HC_TH),
    COMBO(H_Sh_combo, HC_SH),
    COMBO(H_Wh_combo, HC_WH),
    // Common word combos
    COMBO(BSPC_E_COMBO, BSPCEV_EVERY),
    COMBO(BSPC_H_COMBO, BSPCH_HERE),
    COMBO(BSPC_I_COMBO, BSPCIT_IN_THE),
    COMBO(DCOM_COMBO, DCOM),
    COMBO(DFR_COMBO, DFR),
    // "Adaptive keys"
    COMBO(AU_COMBO, HC_AU),
    COMBO(UA_COMBO, HC_UA),
    COMBO(EO_COMBO, HC_EO),
    COMBO(OE_COMBO, HC_OE),
};



// Custom combo timing
uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch (index) {
        case HC_PIPE:
            return 30;
        case HC_SCAP:
            return 30;
    }
    return COMBO_TERM;
}

///////////////////////////////////////////////////////////////////////////////
// Achordion (https://getreuer.info/posts/keyboards/achordion)
///////////////////////////////////////////////////////////////////////////////
// Define shift and gui as eager for accordion
bool achordion_eager_mod(uint8_t mod) {
  switch (mod) {
    case MOD_LSFT:
    case MOD_RSFT:
    case MOD_LGUI:
    case MOD_RGUI:
      return true;  // Eagerly apply Shift and GUI mods.

    default:
      return false;
  }
}
// Set shift to a shorter streak timer
uint16_t achordion_streak_chord_timeout(
    uint16_t tap_hold_keycode, uint16_t next_keycode) {
  uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(tap_hold_keycode));
  if ((mod & MOD_LSFT) != 0) {
    return 100;  // A shorter streak timeout for Shift mod-tap keys.
  } else {
    if ((mod & MOD_RSFT) != 0) {
        return 100;
    } else {
    return 200;  // A longer timeout otherwise.
    }
  }
}


///////////////////////////////////////////////////////////////////////////////
// process_record_user
///////////////////////////////////////////////////////////////////////////////

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Save mod state for later use
    uint8_t mod_state = get_mods();
    uint8_t oneshot_mod_state = get_oneshot_mods();

    // Process achordion to determine holds   
    if (!process_achordion(keycode, record)) { return false; }

    // Process custom shift keys
    if (!process_custom_shift_keys(keycode, record)) { 
        return false; 
    }
   
    switch (keycode) {
         // define combos
        case HC_SCAP:
            if (record->event.pressed) {
                tap_code16(HD_SCAP);
            }
            return false;
        case HC_CW_TOGG:
            if (record->event.pressed) {
                caps_word_on();
            }
            return false;
        case HC_EXLM:
            if (record->event.pressed) {
                tap_code16(KC_EXLM);
            }
            return false;
        case HC_QUES:
            if (record->event.pressed) {
                tap_code16(KC_QUES);
            }
            return false;
        case HC_PIPE:
            if (record->event.pressed) {
                tap_code16(KC_PIPE);
            }
            return false;
        case HC_GLMETS:
            if (record->event.pressed) {
                tap_code16(HD_LGMT);  // «
                tap_code16(KC_SPACE);
                tap_code16(KC_SPACE);
                tap_code16(HD_RGMT);  // »
                tap_code16(KC_LEFT);
                tap_code16(KC_LEFT);
            }
            return false;        
        case HC_CH:
            if (record->event.pressed) {
                // Check if Caps Word is active
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_C));
                    tap_code16(S(KC_H));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_C);
                    tap_code16(KC_H);
                }
            }
            return false;
        case HC_SCH:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_S));
                    tap_code16(S(KC_C));
                    tap_code16(S(KC_H));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_S);
                    tap_code16(KC_C);
                    tap_code16(KC_H);
                }
            }
            return false;
        case HC_GH:
            if (record->event.pressed) {
               if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_G));
                    tap_code16(S(KC_H));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_G);
                    tap_code16(KC_H);
                }
            }
            return false;
        case HC_PH:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_P));
                    tap_code16(S(KC_H));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_P);
                    tap_code16(KC_H);
                }
            }
            return false;
        case HC_TH:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_T));
                    tap_code16(S(KC_H));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_T);
                    tap_code16(KC_H);
                }
            }
            return false;
        case HC_SH:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_C));
                    tap_code16(S(KC_H));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_S);
                    tap_code16(KC_H);
                }
            }
            return false;
        case HC_WH:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_W));
                    tap_code16(S(KC_H));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_W);
                    tap_code16(KC_H);
                }
            }
            return false;

        case BSPCEV_EVERY:
            if (record->event.pressed) {
                SEND_STRING("every");
            }
            return false;

        case BSPCH_HERE:
            if (record->event.pressed) {
                SEND_STRING("here");
            }
            return false;

        case BSPCIT_IN_THE:
            if (record->event.pressed) {
                SEND_STRING("in the");
            }
            return false;
        case DCOM:
            if (record->event.pressed) {
                SEND_STRING(".com");
            }
            return false;
        case DFR:
            if (record->event.pressed) {
                SEND_STRING(".fr");
            }
            return false;
        case Q_QU:
            if (record->tap.count > 0) {    // Key is being tapped.
                if (record->event.pressed) {
                    if (is_caps_word_on()) {
                        // Send uppercase letters using the Shift modifier
                        tap_code16(S(KC_Q));
                        tap_code16(S(KC_U));
                    } else {
                        // Send lowercase letters
                        tap_code16(KC_Q);
                        tap_code16(KC_U);
                    }
                }
            } else { // Key is being held.
                if (record->event.pressed) {
                    if (is_caps_word_on()) {
                        // Send uppercase letters using the Shift modifier
                        tap_code16(S(KC_Q));
                    } else {
                    // Send lowercase letters
                    tap_code16(KC_Q);
                    }
                }
            }
            return false;  // Skip default handling.
        // "Adaptive keys"
        case HC_AU:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_A));
                    tap_code16(S(KC_U));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_A);
                    tap_code16(KC_U);
                }
            }
            return false;
        case HC_UA:
            if (record->event.pressed) {
               if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_U));
                    tap_code16(S(KC_A));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_U);
                    tap_code16(KC_A);
                }
            }
            return false;
        case HC_EO:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_E));
                    tap_code16(S(KC_O));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_E);
                    tap_code16(KC_O);
                }
            }
            return false;
        case HC_OE:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_O));
                    tap_code16(S(KC_E));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_O);
                    tap_code16(KC_E);
                }
            }
            return false;
        case HN_EQL:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    SEND_STRING("!=");
                    register_mods(mod_state);
                }
                else {
                    tap_code16(KC_EQL);
                }
            }
            return false;
        case UPDIR:  // Types ../ to go up a directory on the shell.
            if (record->event.pressed) {
                SEND_STRING("../");
            }
            return false;
        case SELLINE:  // Selects the current line.
            if (record->event.pressed) {
                SEND_STRING(SS_LCTL("a" SS_LSFT("e")));
            }
            return false;
        case SELWD:  // Selects the current word.
            if (record->event.pressed) {
                tap_code16(LALT(KC_LEFT));
                tap_code16(LALT(LSFT(KC_RIGHT)));
            }
            return false;
        case HS_LABK:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    SEND_STRING("<=");
                    register_mods(mod_state);
                }
                else {
                    SEND_STRING("<");
                }
            }
            return false;
        case HS_RABK:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    SEND_STRING(">=");
                    register_mods(mod_state);
                }
                else {
                    SEND_STRING(">");
                }
            }
            return false;
        case UNAME:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    SEND_STRING("williamsjag@gmail.com");
                    register_mods(mod_state);
                }
                else {
                    SEND_STRING("williamsjag");
                }
            }
            return false;
        case HD_EURO:
            if (record->event.pressed) {
                tap_code16(LALT(LSFT(KC_2)));
            }
            return false;
        case HD_PND:
            if (record->event.pressed) {
                tap_code16(LALT(KC_3));
            }
            return false;
        case HD_YEN:
            if (record->event.pressed) {
                tap_code16(LALT(KC_Y));
            }
            return false;
        case WIN_L3:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    tap_code16(WIN_L23);
                    register_mods(mod_state);
                }
                else {
                    tap_code16(WIN_L3);
                }
            }
            return false;
        case WIN_R3:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    tap_code16(WIN_R23);
                    register_mods(mod_state);
                }
                else {
                    tap_code16(WIN_R3);
                }
            }
            return false;
    }
    return true;
}

#include "gpio.h" // Include GPIO functions

void matrix_scan_user(void) {
    achordion_task();
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      TD(TD_ESC_LAYER_BOOT), KC_1, KC_2, KC_3, KC_4, KC_5,                         KC_6,    KC_7,    KC_8,   KC_9,   KC_0, TD(TD_ESC_LAYER_BOOT),
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,   KC_O,   KC_P,    KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,   KC_L,   KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,   KC_DOT, KC_SLSH, KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, MO(_HD),  KC_SPC,    KC_ENT, MO(_NUM),  KC_RALT
                                      //`--------------------------'  `--------------------------'

  ),

    [_HD] = LAYOUT(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      TD(TD_ESC_LAYER_BOOT), KC_1, KC_2, KC_3, KC_4, KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  TD(TD_ESC_LAYER_BOOT), 
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,   HD_W,    HD_X,    HD_M,    HD_G,    HD_J,                      HD_HASH, TD(TD_DOT), HD_SLSH, HD_DQUO, HD_QUOT, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      Q_QU,     HD_C,    HD_S,    HD_N,    HD_T,    HD_K,                        HD_COMM,  HD_A,    HD_E,    HD_I,    HD_H,   HD_Z, 
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_LSFT,  HD_P,    HD_F,    HD_L,    HD_D,    HD_V,                        HD_MINS,  HD_U,    HD_O,    HD_Y,    HD_B,   HD_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_BSPC,  HD_R,  MO(_NUM),   MO(_SYM), KC_SPC,  KC_ENT
                                      //`--------------------------'  `--------------------------'
  ),

    [_NUM] = LAYOUT(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_NO,   SELLINE, SELW_L,  SELWD,   SELW_R,  SEL_A,                        HD_CIRC,  HD_HASH, HD_PERC, HD_DLR, HD_COLN, HD_EXLM, 
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,  KC_PGUP, HN_HOME, KC_UP,   HN_END,  D_SCRL,                       HD_SLSH,  HN_7,   KC_8,    HN_9,   KC_MINS,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO,   KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, U_SCRL,                       HD_ASTR,  HN_4,   HN_5,    HN_6,   HN_PLUS,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_LSFT, HD_UNDO, HD_CUT,  HD_COPY, HD_PSTE, HD_PSTM,                      HD_COMM,  KC_1,   KC_2,    KC_3,   HN_EQL,   HD_RSFT, 
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_NO,   KC_NO,   KC_TRNS,   MO(_EXT),  KC_0,   HD_DOT
                                      //`--------------------------'  `--------------------------'
  ),

    [_SYM] = LAYOUT(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC, KC_GRV,  HD_LPRN, HD_RPRN, HD_SCLN, HD_COMM,                      KC_NO,  HD_EURO,  HD_PND,  HD_YEN,  KC_NO,   KC_NO, 
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_EXLM, HD_LCBR, HD_LBRC, HD_RBRC, HD_RCBR, HD_QUES,                      UPDIR,   KC_BSPC, KC_TAB,  HD_AT,   KC_NO,   KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_HASH, HD_CIRC, HN_EQL,  HD_UNDS, HD_DLR,  HD_ASTR,                      HD_COLN, KC_RSFT, UNAME,   KC_AMPR, KC_ENT,  KC_NO,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_TILD, HS_LABK, HD_PIPE, HD_MINS, HS_RABK, HD_BSLS,                      HD_DOT,  KC_DEL,  HD_BTAB, HD_PERC, KC_RSFT, KC_NO, 
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          HD_SLSH, HD_SPC, MO(_EXT),     KC_TRNS,  KC_NO,   KC_NO
                                      //`--------------------------'  `--------------------------'
  ),

    [_EXT] = LAYOUT(
 //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC,  CAP_PG,  WIN_L3,  WIN_L2,  WIN_R2,  WIN_R3,                       MUTE,   MUS_PRE,  PLAY,    MUS_NEX, V_DOWN,  V_UP,
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB, B_TAB_P, B_TAB_N, B_BRWS,  F_BRWS,  KC_PGUP,                       DOCST,   HN_HOME, HN_UP,   HN_END,  DEFINE,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      GPT,    KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_PGDN,                       DOCEND,  HN_LEFT, HN_DOWN, HN_RGHT, TSLATE,  KC_DEL, 
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     HD_REDO, HD_UNDO, HD_CUT,  HD_COPY, HD_PSTE, HD_PSTM,                       PWORD,   ZM_IN,   ZM_RST,  ZM_OUT,  HELP,    HD_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_NO,   KC_NO,  KC_TRNS,    KC_TRNS, KC_NO,   KC_NO
                                      //`--------------------------'  `--------------------------' 
    )
};