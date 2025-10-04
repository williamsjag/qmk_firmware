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
            SEND_STRING("HD Vibranium");
            wait_ms(500);
            for (int i = 0; i < 12; i++) {
                tap_code(KC_BSPC);
            }
        } else {
            // If Layer 1 is inactive, switch to Layer 1
            layer_invert(1);
            SEND_STRING("QWERTY");
            wait_ms(500);
            for (int i = 0; i < 6; i++) {
                tap_code(KC_BSPC);
            }
        }
    }
}



// Define Tap Dance Actions
tap_dance_action_t tap_dance_actions[] = {
    [TD_ESC_LAYER_BOOT] = ACTION_TAP_DANCE_FN(dance_esc_layer_boot),
    // [TD_DOT] = ACTION_TAP_DANCE_FN(sentence_end),
 };

// Set longer tapping term for tap dances
 uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
         case HR_5: // reset buttons
            return 3000;
         case HR_6:
            return 3000;
        default:
            return TAPPING_TERM;
    }
}

// Not strictly a tap dance, but related
// Helper for implementing tap vs. long-press keys for simple keycodes if I ever implement any. Given a tap-hold
// key event, replaces the hold function with `long_press_keycode`.
 /*static bool process_tap_or_long_press_key(*/
 /*    keyrecord_t* record, uint16_t long_press_keycode) {*/
 /*  if (record->tap.count == 0) {  // Key is being held.*/
 /*    if (record->event.pressed) {*/
 /*      tap_code16(long_press_keycode);*/
 /*    }*/
 /*    return false;  // Skip default handling.*/
 /*  }*/
 /*  return true;  // Continue default handling.*/
 /*}*/

///////////////////////////////////////////////////////////////////////////////
// Custom shift keys (https://getreuer.info/posts/keyboards/custom-shift-keys)
///////////////////////////////////////////////////////////////////////////////

// Define table of shift modifications
const custom_shift_key_t custom_shift_keys[] = {
    {HD_MINS, HD_PLUS}, // Shift - is +
    {HD_COMM, HD_SCLN}, // Shift , is ;
    {HD_HASH, HD_DLR}, // Shift # is $
    {EOS, HD_COLN}, // Shift . is :
    {HD_DOT, HD_COLN}, // Shift . is :
    {HD_SLSH, HD_ASTR}, // Shift / is *
    {HD_DQUO, HD_LBRC}, // Shift " is [
    {HD_QUOT, HD_RBRC}, // Shift ' is ]
    {HN_HOME, MS_BTN1}, // Shift Home on _NUM is L click
    {HN_END, MS_BTN2}, // Shift End on _NUM is R click
    {U_SCRL, L_SCRL}, // Shift up scroll is left scroll
    {D_SCRL, R_SCRL}, // Shift down scroll is right scroll
    {HD_UNDO, HD_REDO},
};

uint8_t NUM_CUSTOM_SHIFT_KEYS =
    sizeof(custom_shift_keys) / sizeof(custom_shift_key_t);

///////////////////////////////////////////////////////////////////////////////
// Combos (https://docs.qmk.fm/features/combo)
///////////////////////////////////////////////////////////////////////////////

enum combos {
    TA_CWORD,
    EXLM,
    QUES,
    AE_PIPE,
    AE_PIPE2,
    AI_SCAP,
    GLMETS,
    ST_CH,
    CSN_SCH,
    MG_GH,
    PF_PH,
    TN_TH,
    SN_SH,
    TSC_TCH,
    WX_WH,
    UO_COM,
    YOU_GML,
    OY_FR,
    AH_AU,
    UH_UA,
    EH_EO,
    OH_OE,
    COMBO_LENGTH
};

// RH Punctuation and function
#define HD_pipe_keys      HD_A, HD_E // Type "|"
#define HS_pipe_keys      HS_SCLN, HN_EQL // Type "|"
#define HD_Screencap_keys HD_A, HD_I // Capture screen
#define HD_wcap_keys      HD_T, HD_A // Toggle caps word
#define HD_ques_keys      HD_SLSH, HD_DQUO // ?
#define HD_exlm_keys      EOS, HD_SLSH // !
#define HD_guilmet_keys HD_DQUO, HD_QUOT // « | »
// LH h digraph combos - all use original key + neighbor
#define HD_Th_keys HD_T, HD_N // Type "th"
#define HD_Sh_keys HD_T, HD_C // Type "sh"
#define HD_Ch_keys HD_N, HD_C // Type "ch"
#define HD_Wh_keys HD_W, HD_M // Type "wh"
#define HD_Ph_keys HD_P, HD_L // Type "ph"
#define HD_Gh_keys HD_G, HD_M // Type "gh"
#define HD_Sch_keys HD_S, HD_C, HD_N // Type "sch"
#define HD_Tch_keys HD_C, HD_N, HD_T // Type "tch"
// Common words
#define dcom_keys    HD_U,    HD_O // Type ".com"
#define dfr_keys     HD_O,    HD_Y // Type ".fr"
#define gmail_keys   HD_Y, HD_O, HD_U // Type "@gmail.com"
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
const uint16_t PROGMEM Pipe_combo2[]      = {HS_pipe_keys, COMBO_END}; // |
const uint16_t PROGMEM Screencap_combo[] = {HD_Screencap_keys, COMBO_END}; // Screen Capture
const uint16_t PROGMEM Guilmet_combo[]   = {HD_guilmet_keys, COMBO_END}; // « | »
// H digraph combos (these are mnemonically arranged- first letter + neighbor)
const uint16_t PROGMEM H_Ch_combo[]  = {HD_Ch_keys, COMBO_END}; // TYPE "ch"
const uint16_t PROGMEM H_Sch_combo[] = {HD_Sch_keys, COMBO_END}; // TYPE "Sch"
const uint16_t PROGMEM H_Gh_combo[]  = {HD_Gh_keys, COMBO_END}; // TYPE "gh"
const uint16_t PROGMEM H_Ph_combo[]  = {HD_Ph_keys, COMBO_END}; // TYPE "ph"
const uint16_t PROGMEM H_Th_combo[]  = {HD_Th_keys, COMBO_END}; // TYPE "th"
const uint16_t PROGMEM H_Sh_combo[]  = {HD_Sh_keys, COMBO_END}; // TYPE "sh"
const uint16_t PROGMEM H_Wh_combo[]  = {HD_Wh_keys, COMBO_END}; // TYPE "wh"
const uint16_t PROGMEM H_Tch_combo[] = {HD_Tch_keys, COMBO_END}; // TYPE "Sch"

// Common word combos
const uint16_t PROGMEM DCOM_COMBO[]  = {dcom_keys, COMBO_END};
const uint16_t PROGMEM DFR_COMBO[]   = {dfr_keys, COMBO_END};
const uint16_t PROGMEM GMAIL_COMBO[] = {gmail_keys, COMBO_END};
// "Adaptive keys"
const uint16_t PROGMEM AU_COMBO[]   = {au_keys,  COMBO_END};
const uint16_t PROGMEM UA_COMBO[]   = {ua_keys,  COMBO_END};
const uint16_t PROGMEM EO_COMBO[]   = {eo_keys,  COMBO_END};
const uint16_t PROGMEM OE_COMBO[]   = {oe_keys,  COMBO_END};

combo_t key_combos[] = {
    [TA_CWORD] = COMBO(Caps_word_combo, HC_CW_TOGG),
    [EXLM]     = COMBO(Hexlm_combo, HC_EXLM), // !
    [QUES]     = COMBO(Hques_combo, HC_QUES), // ?
    [AE_PIPE]  = COMBO(Pipe_combo, HC_PIPE), // |
    [AE_PIPE2] = COMBO(Pipe_combo2, HC_PIPE), // |
    [AI_SCAP]  = COMBO(Screencap_combo, HC_SCAP),
    [GLMETS]   = COMBO(Guilmet_combo, HC_GLMETS), // « | »
    // H bigrams
    [ST_CH]    = COMBO(H_Ch_combo, HC_CH),
    [CSN_SCH]  = COMBO(H_Sch_combo, HC_SCH),
    [MG_GH]    = COMBO(H_Gh_combo, HC_GH),
    [PF_PH]    = COMBO(H_Ph_combo, HC_PH),
    [TN_TH]    = COMBO(H_Th_combo, HC_TH),
    [SN_SH]    = COMBO(H_Sh_combo, HC_SH),
    [WX_WH]    = COMBO(H_Wh_combo, HC_WH),
    [TSC_TCH]  = COMBO(H_Tch_combo, HC_TCH),
    // Common word combos
    [UO_COM]   = COMBO(DCOM_COMBO, DCOM),
    [OY_FR]    = COMBO(DFR_COMBO, DFR),
    [YOU_GML]  = COMBO(GMAIL_COMBO, GMAIL),
    // "Adaptive keys"
    [AH_AU]    = COMBO(AU_COMBO, HC_AU),
    [UH_UA]    = COMBO(UA_COMBO, HC_UA),
    [EH_EO]    = COMBO(EO_COMBO, HC_EO),
    [OH_OE]    = COMBO(OE_COMBO, HC_OE),
};

// Custom combo timing
uint16_t get_combo_term(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case AE_PIPE:
        case AE_PIPE2:
        case AI_SCAP:
            return 20;
        case AH_AU:
        case UH_UA:
        case EH_EO:
        case OH_OE:
            return 200;
        default:
            return COMBO_TERM;
    }
}

bool get_combo_must_press_in_order(uint16_t combo_index, combo_t *combo) {
    switch (combo_index) {
        case AH_AU:
        case UH_UA:
        case EH_EO:
        case OH_OE:
            return true;
        default:
            return false;
    }
}

bool get_combo_must_tap(uint16_t combo_index, combo_t *combo) {
    // Make *all* combos, that have Mod-Tap/Layer-Tap/Momentary keys in its chord, to be tap-only
    uint16_t key;
    uint8_t idx = 0;
    while ((key = pgm_read_word(&combo->keys[idx])) != COMBO_END) {
        switch (key) {
            case QK_MOD_TAP...QK_MOD_TAP_MAX:
            case QK_LAYER_TAP...QK_LAYER_TAP_MAX:
            case QK_MOMENTARY...QK_MOMENTARY_MAX:
                return true;
        }
        idx += 1;
    }
    return false;

}
///////////////////////////////////////////////////////////////////////////////
// Achordion (https://getreuer.info/posts/keyboards/achordion)
///////////////////////////////////////////////////////////////////////////////

// Define shift and gui as eager for accordion
// bool achordion_eager_mod(uint8_t mod) {
//   switch (mod) {
//     case MOD_LSFT:
//     case MOD_RSFT:
//     case MOD_LGUI:
//     case MOD_RGUI:
//       return true;  // Eagerly apply Shift and GUI mods.
//
//     default:
//       return false;
//   }
// }
// // Set shift to a shorter streak timer
// uint16_t achordion_streak_chord_timeout(
//     uint16_t tap_hold_keycode, uint16_t next_keycode) {
//   uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(tap_hold_keycode));
//   if ((mod & MOD_LSFT) != 0) {
//     return 100;  // A shorter streak timeout for Shift mod-tap keys.
//   } else {
//     if ((mod & MOD_RSFT) != 0) {
//         return 100;
//     } else {
//     return 200;  // A longer timeout otherwise.
//     }
//   }
// }

///////////////////////////////////////////////////////////////////////////////
// CAPS WORD (https://docs.qmk.fm/features/caps_word)
///////////////////////////////////////////////////////////////////////////////

// customization for caps word if necessary
// bool caps_word_press_user(uint16_t keycode) {
//     switch (keycode) {
//         // Keycodes that continue Caps Word, with shift applied.
//         case KC_A ... KC_Z:
//         case KC_MINS:
//             add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
//             return true;

//         // Keycodes that continue Caps Word, without shifting.
//         case KC_1 ... KC_0:
//         case KC_BSPC:
//         case KC_DEL:
//         case KC_UNDS:
//             return true;

//         default:
//             return false;  // Deactivate Caps Word.
//     }
// }

///////////////////////////////////////////////////////////////////////////////
// process_record_user
///////////////////////////////////////////////////////////////////////////////

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Save mod state for
    uint8_t mod_state = get_mods();
    uint8_t oneshot_mod_state = get_oneshot_mods();

    // Process achordion to determine holds
    // if (!process_achordion(keycode, record)) { return false; }

    // Process custom shift keys
    if (!process_custom_shift_keys(keycode, record)) {
        return false;
    }

    switch (keycode) {

        // define combo actions
        case HC_SCAP:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    tap_code16(HD_FSCAP);
                } else {
                     tap_code16(HD_SCAP);
                }
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
        case HC_TCH:
            if (record->event.pressed) {
                if (is_caps_word_on()) {
                    // Send uppercase letters using the Shift modifier
                    tap_code16(S(KC_T));
                    tap_code16(S(KC_C));
                    tap_code16(S(KC_H));
                } else {
                    // Send lowercase letters
                    tap_code16(KC_T);
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
                    tap_code16(S(KC_S));
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
                // reset button
        case GMAIL:
            if (record->event.pressed) {
                SEND_STRING("@gmail.com");
            }
            return false;
        case HR_5: // Hold to reset keyboard
            if (record->tap.count > 0) {    // Key is being tapped.
                if (record->event.pressed) {
                    tap_code16(KC_5);
                }
            } else {
               reset_keyboard();
            }
            return false;
        case HR_6: // Hold to reset keyboard
            if (record->tap.count > 0) {    // Key is being tapped.
                if (record->event.pressed) {
                    tap_code16(KC_6);
                }
            } else {
                reset_keyboard();
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
        case EOS:  // Period on tap, Period/␣/OSM_SHIFT on long press.
            if (record->tap.count > 0) {    // Key is being tapped.
                if (record->event.pressed) {
                    tap_code16(HD_DOT);
                }
            // handle long press if held
            } else { // Key is being long pressed.
                if (record->event.pressed) {
                    if (!(get_mods() & MOD_MASK_SHIFT)) {
                        tap_code(HD_DOT);
                        tap_code(KC_SPC);
                        /* Internal code of OSM(MOD_LSFT) */
                        add_oneshot_mods(MOD_BIT(KC_LEFT_SHIFT));
                    } else {
                        tap_code16(KC_COLN);
                    }
                }
            }
            return false;
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
        case RNAME:
            if (record->event.pressed) {
                if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                    unregister_mods(MOD_MASK_SHIFT);
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    SEND_STRING("Jackson Williams");
                    register_mods(mod_state);
                }
                else {
                    SEND_STRING("Jackson");
                }
            }
            return false;
        case HS_RPRN:
            if (record->tap.count && record->event.pressed) {      // Check if the key was tapped (not held for shift)
                tap_code16(KC_RPRN); // Send ';' when tapped
            }
            return false; // Skip further processing
        case HN_PLUS:
            if (record->event.pressed) {
                if (record->tap.count) {
                    tap_code16(KC_PLUS);
                } else {
                    // Register LALT when the key is held
                    register_mods(MOD_BIT(KC_RCTL));
                }
            } else {
                // Unregister LALT when the key is released
                unregister_mods(MOD_BIT(KC_RCTL));
            }
            return false; // Skip further processing of this key
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
        case HS_LPRN:
            if (record->event.pressed && record->tap.count) {
                tap_code16(KC_LPRN);
            }
            return false;
        case HD_UNAME: // I wish I knew why this is necessary
            if (record->event.pressed) {
                if (record->tap.count) {
                    if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                        unregister_mods(MOD_MASK_SHIFT);
                        del_oneshot_mods(MOD_MASK_SHIFT);
                        SEND_STRING("williamsjag@gmail.com");
                        register_mods(mod_state);
                    } else {
                        SEND_STRING("williamsjag");
                    }
                } else {
                    // Register LALT when the key is held
                    register_mods(MOD_BIT(KC_RCTL));
                }
            } else {
                // Unregister LALT when the key is released
                unregister_mods(MOD_BIT(KC_RCTL));
            }
            return false; // Skip further processing of this key
        case HD_AMPR: // I wish I knew why this is necessary
            if (record->event.pressed) {
                if (record->tap.count) {
                    tap_code16(KC_AMPR);
                } else {
                    // Register LALT when the key is held
                    register_mods(MOD_BIT(KC_RALT));
                }
            } else {
                // Unregister LALT when the key is released
                unregister_mods(MOD_BIT(KC_RALT));
            }
            return false; // Skip further processing of this key
        case HS_LABK:
            if (record->event.pressed) {
                if (record->tap.count) {
                    if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                        unregister_mods(MOD_MASK_SHIFT);
                        del_oneshot_mods(MOD_MASK_SHIFT);
                        SEND_STRING("<=");
                        register_mods(mod_state);
                    }
                    else {
                        SEND_STRING("<");
                    }
                    return false;
                } else {
                    // Register LALT when the key is held
                    register_mods(MOD_BIT(KC_LCTL));
                }
            } else {
                // Unregister LALT when the key is released
                unregister_mods(MOD_BIT(KC_LCTL));
            }
            return false; // Skip further processing of this key
        case HS_RABK:
            if (record->event.pressed) {
                if (record->tap.count) {
                    if (mod_state & MOD_MASK_SHIFT || oneshot_mod_state & MOD_MASK_SHIFT) {
                        unregister_mods(MOD_MASK_SHIFT);
                        del_oneshot_mods(MOD_MASK_SHIFT);
                        SEND_STRING(">=");
                        register_mods(mod_state);
                    }
                    else {
                        SEND_STRING(">");
                    }
                    return false;
                } else {
                    // Register LALT when the key is held
                    register_mods(MOD_BIT(KC_LALT));
                }
            } else {
                // Unregister LALT when the key is released
                unregister_mods(MOD_BIT(KC_LALT));
            }
            return false; // Skip further processing of this key
        case KC_TAB:
            if ((mod_state & MOD_MASK_GUI) && (mod_state & MOD_MASK_ALT)) {
                unregister_mods(mod_state);
                tap_code16(G(KC_GRV));
                return false;
            }
            return true;
    }
    return true;
}

#include "gpio.h" // Include GPIO functions

// void matrix_scan_user(void) {
//     achordion_task();
// }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_HD] = LAYOUT(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      TD(TD_ESC_LAYER_BOOT), KC_1, KC_2, KC_3, KC_4, HR_5,                        HR_6,    KC_7,    KC_8,    KC_9,    KC_0,  TD(TD_ESC_LAYER_BOOT),
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,   HD_X,    HD_W,    HD_M,    HD_G,    HD_J,                        HD_HASH,  EOS,     HD_SLSH, HD_DQUO, HD_QUOT, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      Q_QU,     HD_S,    HD_C,    HD_N,    HD_T,    HD_K,                        HD_COMM,  HD_A,    HD_E,    HD_I,    HD_H,   HD_Z,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HD_LSFT,  HD_F,    HD_P,    HD_L,    HD_D,    HD_V,                        HD_MINS,  HD_U,    HD_O,    HD_Y,    HD_B,   HD_RSFT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_BSPC,  HD_R,  HD_TAB,      HD_ESC, KC_SPC,  KC_ENT
                                      //`--------------------------'  `--------------------------'
  ),

    [_QWERTY] = LAYOUT(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      TD(TD_ESC_LAYER_BOOT), KC_1, KC_2, KC_3, KC_4, HR_5,                         HR_6,    KC_7,    KC_8,   KC_9,   KC_0, TD(TD_ESC_LAYER_BOOT),
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,   KC_O,   KC_P,    KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,   KC_L,   KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,   KC_DOT, KC_SLSH, KC_ESC,
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
      KC_ESC,  KC_NO,   KC_NO,   HD_QUOT, HD_DQUO, KC_NO,                        HD_EURO, HD_DLR,  HD_PND,   HD_YEN,  KC_NO,  KC_DEL,
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB,  KC_GRV,  HD_CIRC, HD_LBRC, HD_RBRC, HD_HASH,                      UPDIR,   HD_QUES, HD_SLSH,  HD_PERC, EMOJI,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO,   HS_LABK, HS_RABK, HS_LPRN, HS_RPRN, HD_UNDS,                      HD_COLN, HS_SCLN, HN_EQL,   HD_AMPR, HD_UNAME, KC_ENT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO,   KC_NO,   HD_EXLM, HD_LCBR, HD_RCBR, HD_COMM,                      KC_DOT,  HD_TILD, HD_MINS,  HD_AT,   RNAME,  KC_NO,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_BSLS, HD_SPC, MO(_EXT),     KC_TRNS,  KC_NO,   KC_NO
                                      //`--------------------------'  `--------------------------'
  ),

    [_EXT] = LAYOUT(
 //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_ESC, WIN_L3,  WIN_L2,  WIN_C,   WIN_R2,  WIN_R3,                        MUTE,   MUS_PRE,  PLAY,    MUS_NEX, V_DOWN,  V_UP,
  //,--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_ESC, WIN_L3,  WIN_L2,  WIN_C,   WIN_R2,  WIN_R3,                        DOCST,   MS_BTN1, MS_UP,   MS_BTN2,  DEFINE,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_TAB, B_TAB_P, B_TAB_N, B_BRWS,  F_BRWS,  GPT,                           DOCEND,  MS_LEFT, MS_DOWN, MS_RGHT, TSLATE,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      HR_5,   SELLINE, SELW_L,  SELWD,   SELW_R,  SEL_A,                         PWORD,   ZM_IN,   ZM_RST,  ZM_OUT,  HELP,    HR_6,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                           KC_NO,   KC_NO,  KC_TRNS,    KC_TRNS, KC_NO,   KC_NO
                                      //`--------------------------'  `--------------------------'
    ),

};

    const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM =
  LAYOUT(
  'L', 'L', 'L', 'L', 'L', 'L',             'R', 'R', 'R', 'R', 'R', 'R',
  'L', 'L', 'L', 'L', 'L', 'L',             'R', 'R', 'R', 'R', 'R', 'R',
  'L', 'L', 'L', 'L', 'L', 'L',             'R', 'R', 'R', 'R', 'R', 'R',
  'L', 'L', 'L', 'L', 'L', 'L',             'R', 'R', 'R', 'R', 'R', 'R',
                        '*', '*', '*',   '*', '*', '*'
  );
