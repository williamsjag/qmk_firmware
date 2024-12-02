// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "cygnus.h"
#include "features/custom_shift_keys.h"
#include "features/achordion.h"


#define ____ KC_TRNS

uint16_t last_keycode;

// Custom keycode definitions found in cygnus.h

///////////////////////////////////////////////////////////////////////////////
// Tap Dance (https://docs.qmk.fm/features/tap_dance)
///////////////////////////////////////////////////////////////////////////////

// Helper function for keycode conversion
uint16_t ascii_to_keycode(char c) {
    if (c >= 'A' && c <= 'Z') {
        return (c - 'A') + KC_A;  // Convert uppercase letters
    } else if (c >= 'a' && c <= 'z') {
        return (c - 'a') + KC_A;  // Convert lowercase letters (QMK assumes shift is applied)
    } else if (c >= '0' && c <= '9') {
        return (c - '0') + KC_0;  // Convert numbers
    } else {
        // Add additional mappings here for other characters as needed
        switch (c) {
            case ' ':
                return KC_SPC;  // Space
            case '-':
                return KC_MINS; // Dash
            case '_':
                return S(KC_MINS); // Underscore
            // Add more symbols as necessary
            default:
                return KC_NO;  // Unsupported character
        }
    }
}

// Custom function to announce layer on change
void type_and_delete_text(const char* text) {
    // Type out the text
    for (const char* p = text; *p; p++) {
        // convert char to keycode
        uint16_t keycode = ascii_to_keycode(*p);
        if (keycode != KC_NO) {
            tap_code16(*p);
            wait_ms(15);
        }
    }
    // Wait 500 ms
    wait_ms(500);
    // Delete the text
    for (const char* p = text; *p; p++) {
        tap_code(KC_BSPC);
    }
}

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
            type_and_delete_text("QWERTY");
        } else {
            // If Layer 1 is inactive, switch to Layer 1
            layer_invert(1);
            type_and_delete_text("HD Vibranium");
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
            // Delete extra space on double tap
            tap_code(KC_BSPC);
            break;

        // Three dots ellipsis.
        case 3:
            // add a second dot
            tap_code(KC_DOT);
            // tap the third dot
            tap_code(KC_DOT);
            break;

        /* Old code before adding case 1
        send KC_DOT on every normal tap of TD_DOT
        default:
            tap_code(KC_DOT);
        */
    }
};

void sentence_end_finished (tap_dance_state_t *state, void *user_data) {
}


// Define Tap Dance Actions
tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_LAYER_BOOT] = ACTION_TAP_DANCE_FN(dance_esc_layer_boot),
    [TD_DOT] = ACTION_TAP_DANCE_FN_ADVANCED(sentence_end, sentence_end_finished, NULL),
};

// Not exactly a Tap Dance, but related
// Helper for implementing tap vs. long-press keys. Given a tap-hold
// key event, replaces the hold function with `long_press_keycode`.
static bool process_tap_or_long_press_key(
    keyrecord_t* record, uint16_t long_press_keycode) {
  if (record->tap.count == 0) {  // Key is being held.
    if (record->event.pressed) {
      tap_code16(long_press_keycode);
    }
    return false;  // Skip default handling.
  }
  return true;  // Continue default handling.
}

///////////////////////////////////////////////////////////////////////////////
// Hands Down Adaptive Keys (https://sites.google.com/alanreiser.com/handsdown/home/hands-down-neu)
///////////////////////////////////////////////////////////////////////////////

// define variables used
uint16_t preprior_keycode = KC_NO;
uint16_t prior_keycode = KC_NO;
uint16_t prior_keydown = 0; // timer of keydown for adaptive threshhold.
uint8_t  saved_mods;

// https://github.com/moutis/HandsDown/blob/main/handsdown/vx-adaptive.c
// https://github.com/moutis/HandsDown/blob/main/handsdown/adapt_h.c

bool process_adaptive_key(uint16_t keycode, const keyrecord_t *record) {
    bool return_state = true; // assume we don't do anything.
    
    // Check for adaptive context
    if (timer_elapsed(prior_keydown) > ADAPTIVE_TERM) { // outside adaptive threshhold
        prior_keycode = preprior_keycode = prior_keydown = 0; // turn off Adaptives.
        return true; // no adaptive conditions, so return.
    }

    // K, this could be adaptive, so process.
    saved_mods = get_mods();

    switch (keycode) { // process ignoring multi-function keys & shift state?
        case HD_H: // H precedes a vowel much more often than it follows (thanks, Ancient Greek!) so adaptive H is a sort of Magic Key
            switch (prior_keycode) {
                case HD_A:
                    tap_code(KC_U); // "AH" yields "AU" (7x more common)
                    return_state = false; // done.
                    break;
                case HD_U: //
                    tap_code(KC_A); // "UH" yields "UA" (126x more common)
                    return_state = false; // done.
                    break;
                case HD_E: // these EO/OE adaptives are of questionable value
                    tap_code(KC_O); // "EH" yields "EO" (1.75:1)
                    return_state = false; // done.
                    break;
                case HD_O:
                    tap_code(KC_E); // "OH" yields "OE" (almost 1:1, but eliminates an SFB?)
                    return_state = false; // done.
                    break;
            }
        case HD_HASH: // Magic HD_HASH
            switch (prior_keycode) {
                case HD_D: // "does"
                    SEND_STRING("oes");
                    return_state = false;
                    break;
                case HD_F: // "for"
                    SEND_STRING("or");
                    return_state = false;
                    break;
                case HD_H: // "have"
                    SEND_STRING("ave");
                    return_state = false;
                    break;
                case HD_J: // "just"
                    SEND_STRING("ust");
                    return_state = false;
                    break;
                case HD_K: // "know"
                    SEND_STRING("now");
                    return_state = false;
                    break;
                case HD_M: // "ment"
                    SEND_STRING("ent");
                    return_state = false;
                    break;
                case HD_S: // "sion"
                    SEND_STRING("ion");
                    return_state = false;
                    break;
                case HD_T: // "tion"
                    SEND_STRING("ion");
                    return_state = false;
                    break;
                case HD_W: // "williams"
                    SEND_STRING("illiams");
                    return_state = false;
                    break;
                case HD_SPC: // "and"
                    SEND_STRING("and");
                    return_state = false;
                    break;
            }
    }
    if (return_state) { // no adaptive processed, cancel state and pass it on.
        set_mods(saved_mods);
        prior_keycode = preprior_keycode = keycode = 0;
    }
    return return_state;
}

///////////////////////////////////////////////////////////////////////////////
// Custom shift keys (https://getreuer.info/posts/keyboards/custom-shift-keys)
///////////////////////////////////////////////////////////////////////////////

// Define table of shift modifications
const custom_shift_key_t custom_shift_keys[] = {
    {HD_MINS, HD_PLUS}, // Shift - is +
    {HD_COMM, HD_SCLN}, // Shift , is ;
    {HD_HASH, HD_DLR}, // Shift # is $
    {HD_DOT, HD_COLN}, // Shift . is :
    {HD_SLSH, HD_ASTR}, // Shift / is *
    {HD_DQUO, HD_LBRC}, // Shift " is [
    {HD_QUOT, HD_RBRC}, // Shift ' is ]
    {HN_7, HD_LPRN}, // Shift 7 on _NUM is (
    {HN_9, HD_RPRN}, // Shift 9 on _NUM is )
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
#define HD_exlm_keys      HD_DOT, HD_SLSH // !
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
};

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case HC_SCAP:
      if (pressed) {
        uint8_t mod_state = get_mods();
        uint8_t oneshot_mod_state = get_oneshot_mods();
        if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    tap_code16(HD_FSCAP);
                    register_mods(mod_state);
        }
        else {
            tap_code16(HD_SCAP);
        }
      }
      break;
    case HC_CW_TOGG:
      if (pressed) {
        tap_code16(CW_TOGG);
      }
      break;
    case HC_EXLM:
      if (pressed) {
        tap_code16(KC_EXLM);
      }
      break;
    case HC_QUES:
      if (pressed) {
        tap_code16(KC_QUES);
      }
      break;
    case HC_PIPE:
      if (pressed) {
        tap_code16(KC_PIPE);
      }
      break;
    case HC_GLMETS:
        if (pressed) {
            SEND_STRING("«  »");
            tap_code16(KC_LEFT);
            tap_code16(KC_LEFT);
        }
        break;        
    case HC_CH:
      if (pressed) {
        SEND_STRING("ch");
      }
      break;
    case HC_SCH:
      if (pressed) {
        SEND_STRING("sch");
      }
      break;
    case HC_GH:
      if (pressed) {
        SEND_STRING("gh");
      }
      break;
    case HC_PH:
      if (pressed) {
        SEND_STRING("ph");
      }
      break;
    case HC_TH:
      if (pressed) {
        SEND_STRING("th");
      }
      break;
    case HC_SH:
      if (pressed) {
        SEND_STRING("sh");
      }
      break;
    case HC_WH:
      if (pressed) {
        SEND_STRING("wh");
      }
      break;

    case BSPCEV_EVERY:
        if (pressed) {
            SEND_STRING("every");
        }
    break;

    case BSPCH_HERE:
        if (pressed) {
            SEND_STRING("here");
        }
    break;

    case BSPCIT_IN_THE:
        if (pressed) {
            SEND_STRING("in the");
        }
    break;
    case DCOM:
        if (pressed) {
            SEND_STRING(".com");
        }
    break;
    case DFR:
        if (pressed) {
            SEND_STRING(".fr");
        }
    break;
  }
}

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
    // Handle QU tap-hold
    switch (keycode) {
        case HD_QU:
            if (record->event.pressed) {
                SEND_STRING("qu");
            }
            return false;
        case HD_Q: // Qu on tap, Q on long press
            return process_tap_or_long_press_key(record, KC_Q);
        // shift = to != in _NUM
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
                tap_code16(UNI_TOG);
                tap_code16(UC(0x20AC));
                tap_code16(UNI_TOG);
            }
            return false;
        case HD_PND:
            if (record->event.pressed) {
                tap_code16(UNI_TOG);
                tap_code16(UC(0x00A3));
                tap_code16(UNI_TOG);
            }
            return false;
        case HD_YEN:
            if (record->event.pressed) {
                tap_code16(UNI_TOG);
                tap_code16(UC(0x00A5));
                tap_code16(UNI_TOG);
            }
            return false;
        case HD_NATUR:
            if (record->event.pressed) {
                tap_code16(UNI_TOG);
                tap_code16(UC(0x266E));
                tap_code16(UNI_TOG);
            }
            return false;
        case HD_FLAT:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    tap_code16(UNI_TOG);
                    tap_code16(UC(0xD834));
                    tap_code16(UC(0xDD2B));
                    tap_code16(UNI_TOG);
                    register_mods(mod_state);
                }
                tap_code16(UNI_TOG);
                tap_code16(UC(0x266D));
                tap_code16(UNI_TOG);
            }
            return false; 
        case HD_SHARP:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    tap_code16(UNI_TOG);
                    tap_code16(UC(0xD834));
                    tap_code16(UC(0xDD2A));
                    tap_code16(UNI_TOG);
                    register_mods(mod_state);
                }
                tap_code16(UNI_TOG);
                tap_code16(UC(0x266F));
                tap_code16(UNI_TOG);
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

    // process adaptive keys - move earlier ??
    if (!process_adaptive_key(keycode, record)) {
            prior_keydown = timer_read(); // (re)start prior_key timing
            preprior_keycode = prior_keycode; // look back 2 keystrokes?
            prior_keycode = keycode; // this keycode is stripped of mods+taps
            return false; // took care of that key
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
      HD_QU,    HD_C,    HD_S,    HD_N,    HD_T,    HD_K,                        HD_COMM,  HD_A,    HD_E,    HD_I,    HD_H,   HD_Z, 
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_LSFT,  HD_P,    HD_F,    HD_L,    HD_D,    HD_V,                        HD_MINS,  HD_U,    HD_O,    HD_Y,    HD_B,   HD_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_BSPC,  HD_R,  MO(_NUM),   MO(_SYM), KC_SPC,  KC_ENT
                                      //`--------------------------'  `--------------------------'
  ),

    [_NUM] = LAYOUT(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_NO,   KC_NO,   SELLINE, SELW_L,  SELW_R,  SEL_A,                        HD_CIRC,  HD_HASH, HD_PERC, HD_DLR, HD_COLN, HD_EXLM, 
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,  KC_PGUP, HN_HOME, HN_UP,   HN_END,  D_SCRL,                       HD_SLSH,  HN_7,   KC_8,    HN_9,   KC_MINS,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO,   KC_PGDN, HN_LEFT, HN_DOWN, HN_RGHT, U_SCRL,                       HD_ASTR,  HN_4,   HN_5,    HN_6,   HN_PLUS,  KC_DEL,
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
      HD_EXLM, HD_LCBR, HD_LBRC, HD_RBRC, HD_RCBR, HD_QUES,                      UPDIR,   KC_BSPC, KC_TAB,  HD_AT,   KC_NO,   HD_SHARP,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_HASH, HD_CIRC, HN_EQL,  HD_UNDS, HD_DLR,  HD_ASTR,                      HD_COLN, KC_RSFT, UNAME,   KC_AMPR, KC_ENT,  HD_NATUR,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_TILD, HS_LABK, HD_PIPE, HD_MINS, HS_RABK, HD_BSLS,                      HD_DOT,  KC_DEL,  HD_BTAB, HD_PERC, KC_RSFT, HD_FLAT, 
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
