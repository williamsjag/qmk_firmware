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
    /* SELW_R,
    SELW_L, */
    SELLINE, 
    HN_EQL, // _NUM = for shift macro
    HS_LABK, // _SYM < for shift macro
    HS_RABK, // _SYM > for shift macro
    // _NUM for mouse buttons   
    /* HN_HOME, 
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
    HD_PSTM, */
    UNAME,
    HD_EURO,
    HD_PND,
    HD_YEN,
    HD_SHARP,
    HD_NATUR,
    HD_FLAT,
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
#define HD_SLSH KC_SLSH // /
#define HD_AT   KC_AT   // @
#define HD_EQL  KC_EQL  // =
#define HD_LBRC KC_LBRC // [
#define HD_RBRC KC_RBRC // ]
#define HD_HASH KC_HASH // #
#define HD_SPC  KC_SPC  // ␣
#define HD_BSPC KC_BSPC // ⌫
#define HD_ENT  KC_ENT  // ↵
#define HD_QUOT KC_QUOT // '
#define HD_DQUO KC_DQUO // "
#define HD_ASTR KC_ASTR // *    
#define HD_COMM KC_COMM // ,
#define HD_UNDS KC_UNDS // _

// Layer special keys
#define HN_4      RSFT_T(KC_4)
#define HN_5      RGUI_T(KC_5)
#define HN_6      RALT_T(KC_6)
#define HN_PLUS   RCTL_T(KC_PLUS)
#define HN_7      KC_7 
#define HN_9      KC_9
#define HD_SCAP   LGUI(LSFT(4))
#define HD_FSCAP  LGUI(LSFT(3))
#define HD_BTAB   LSFT(KC_TAB)
#define UNI_TOG   LCTL(LALT(KC_SPC))
#define HELP      LGUI(LSFT(KC_QUES))
#define DEFINE    LGUI(LALT(KC_D))
#define WIN_R     LALT(LCTL(KC_RIGHT))
#define WIN_L     LALT(LCTL(KC_LEFT))
#define ZM_RST    LGUI(KC_0)
#define ZM_OUT    LGUI(KC_MINS)
#define ZM_IN     LGUI(KC_PLUS)
#define DOCEND    LGUI(KC_DOWN)
#define DOCST     LGUI(KC_UP)
#define U_SCRL    MS_WHLU
#define D_SCRL    MS_WHLD
#define F_BRWS    LGUI(KC_RBRC)
#define B_BRWS    LGUI(KC_LBRC)
#define HN_HOME   KC_HOME
#define HN_END    KC_END
#define HN_RGHT   KC_RGHT
#define HN_LEFT   KC_LEFT
#define HN_UP     KC_UP
#define HN_DOWN   KC_DOWN
#define HD_UNDO   LGUI(KC_Z)
#define HD_REDO   LGUI(LSFT(KC_Z))
#define HD_CUT    LGUI(KC_X)
#define HD_COPY   LGUI(KC_C)
#define HD_PSTE   LGUI(KC_V)
#define HD_PSTM   LSFT(LALT(LGUI(KC_V)))
#define SELW_L    LALT(LSFT(KC_LEFT))
#define SELW_R    LALT(LSFT(KC_RGHT))
#define SEL_A     LGUI(KC_A)
#define CAP_PG    LSFT(LGUI(KC_2))
#define WIN_L3    LCTL(LALT(KC_D))
#define WIN_R3    LCTL(LALT(KC_G))
#define WIN_L23   LCTL(LALT(KC_E))
#define WIN_R23   LCTL(LALT(KC_T))
#define WIN_L2    LCTL(LALT(KC_LEFT))
#define WIN_R2    LCTL(LALT(KC_RGHT))
#define MUTE      KC_F10
#define MUS_PRE   KC_F7
#define MUS_NEX   KC_F9
#define PLAY      KC_F8
#define V_DOWN    KC_F11
#define V_UP      KC_F12
#define TSLATE    LGUI(KC_1)
#define PWORD     LSFT(LGUI(KC_SPC))
#define GPT       LCTL(LALT(LGUI(KC_SPC)))
#define B_TAB_P   LGUI(LALT(KC_UP))
#define B_TAB_N   LGUI(LALT(KC_DOWN))

// Tap Dance keys
// Tap Dance definitions
enum {
    TD_ESC_LAYER_BOOT = 0,
    TD_DOT,
};
// #define TD_ESC TD(TD_ESC_LAYER_BOOT)
// #define TD_DOT TD(TD_DOT)