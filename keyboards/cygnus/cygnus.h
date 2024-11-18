#pragma once


enum layers {
    _QWERTY,
    _HD,
    _NUM,
    _SYM,
    _EXT
};

enum custom_keycodes { // Define custom keycodes for use in combos/macros
    HD_QU = SAFE_RANGE,  // Ensure it’s a unique keycode.
    // Combo Definitions
    HC_SCAP,
    HC_CW_TOGG,
    HC_GLMETS,
    HC_EXLM,
    HC_QUES,
    HC_PIPE,
    BSPCEV_EVERY,
    BSPCH_HERE,
    BSPCIT_IN_THE,
    DCOM,
    DFR,
    HC_CH,
    HC_SCH,
    HC_GH,
    HC_PH,
    HC_TH,
    HC_SH,
    HC_WH,
    UPDIR,
    SELW_R,
    SELW_L,
    SELLINE,
    HN_EQL, // _NUM = for shift macro
    HS_LABK, // _SYM { for shift macro
    HS_RABK, // _SYM } for shift macro
    // _NUM for mouse buttons   
    HN_HOME, 
    HN_END,
    HN_UP,
    HN_LEFT,
    HN_DOWN,
    HN_RGHT,
    // _EXT
    B_BRWS,
    F_BRWS,
    U_SCRL,
    D_SCRL,
    DOCST,
    DOCEND,
    ZM_IN,
    ZM_OUT,
    ZM_RST,
    WIN_L,
    WIN_R,
    DEFINE,
    HELP,  
    HD_UNDO,
    HD_REDO,
    HD_CUT,
    HD_COPY,
    HD_PSTE,
    HD_PSTM,
    UNAME,
};

// Hands Down alphas with hold-taps and custom codes
#define HD_A RSFT_T(KC_A)
#define HD_B KC_B
#define HD_C LCTL_T(KC_C)
#define HD_D KC_D
#define HD_E RGUI_T(KC_E)
#define HD_F KC_F
#define HD_G KC_G
#define HD_H RCTL_T(KC_H)
#define HD_I RALT_T(KC_I)
#define HD_J KC_J
#define HD_K KC_K
#define HD_L KC_L
#define HD_M KC_M
#define HD_N LGUI_T(KC_N)
#define HD_O KC_O
#define HD_P KC_P
#define HD_Q KC_Q
#define HD_R KC_R
#define HD_S LALT_T(KC_S)
#define HD_T LSFT_T(KC_T)
#define HD_U KC_U
#define HD_V KC_V
#define HD_W KC_W
#define HD_X KC_X
#define HD_Y KC_Y
#define HD_Z KC_Z
#define HD_LSFT LSFT_T(OSM(KC_LSFT))
#define HD_RSFT RSFT_T(OSM(KC_RSFT))
// Symbols
#define HD_DOT  KC_DOT  // .
#define HD_MINS KC_MINS // -
#define HD_PLUS KC_PLUS // +
#define HD_SCLN KC_SCLN // ;
#define HD_COLN KC_COLN // :
#define HD_DLR  KC_DLR // $
#define HD_ASTR KC_ASTR // *
#define HD_LABK KC_LABK // <
#define HD_RABK KC_RABK // >
#define HD_QUES KC_QUES // ?
#define HD_PERC KC_PERC // %
#define HD_EXLM KC_EXLM // !
#define HD_BSLS KC_BSLS // backslash
#define HD_PIPE KC_PIPE // |
#define HD_LPRN KC_LPRN // (
#define HD_RPRN KC_RPRN // )
#define HD_CIRC KC_CIRC // ^
#define HD_TILD KC_TILD // ~ 
#define HD_LCBR KC_LCBR // {
#define HD_RCBR KC_RCBR // }
#define HD_SLSH KC_SLSH
#define HD_AT   KC_AT
#define HD_EQL  KC_EQL
#define HD_LBRC KC_LBRC
#define HD_RBRC KC_RBRC
#define HD_HASH KC_HASH
#define HD_SPC  KC_SPC
#define HD_BSPC KC_BSPC
#define HD_ENT  KC_ENT
#define HD_QUOT KC_QUOT
#define HD_DQUO KC_DQUO
#define HD_ASTR KC_ASTR
#define HD_COMM KC_COMM

// Layer special keys
#define HN_4 RSFT_T(KC_4)
#define HN_5 RGUI_T(KC_5)
#define HN_6 RALT_T(KC_6)
#define HN_PLUS RCTL_T(KC_PLUS)
#define HN_7 KC_7 
#define HN_9 KC_9
#define HS_EQL RSFT_T(HN_EQL)
#define HS_COLN LSFT_T(HD_COLN)
#define HD_SCAP LGUI(LSFT(4))
#define HD_FSCAP LGUI(LSFT(3))

// Tap Dance keys
// Tap Dance definitions
enum {
    TD_ESC_LAYER_BOOT = 0,
    TD_DOT,
};
// #define TD_ESC TD(TD_ESC_LAYER_BOOT)
// #define TD_DOT TD(TD_DOT)