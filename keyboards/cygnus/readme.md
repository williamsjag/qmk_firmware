# Cygnus 4x6

![cygnus](imgur.com image replace me!)

This is my build of juhakaup's excellent [Cygnus](https://github.com/juhakaup/keyboards/tree/main/Cygnus%20v1.0) keyboard with build notes accommodating a few significant differences (100% resin print, rp2040zero controller, 4x6 layout), as well as my personal keymap using [Hands Down Vibranium](https://sites.google.com/alanreiser.com/handsdown/home/hands-down-neu) and several of its complemementary features, explained below. This is a keymap intended for MacOS, is you are planning it with using another OS, changes will need to be made to defined modifier/key pairs, as well as to the home row mods.

* Keyboard Maintainer: [Jackson Williams](https://github.com/williamsjag)
* Hardware Supported: rp2040, individual PCBs
* Hardware Availability: *Links to where you can find this hardware*

Compile example for this keyboard (after setting up your build environment and moving cygnus folder into qmk_firmware/keyboards):

    `qmk compile -kb cygnus -km default`

Flashing example for this keyboard:

    drag to usb storage of controller in boot mode

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).


## Differences from Cygnus v1.0

### Resin Printed Case

I chose to print the case using resin instead of a thermoplastic in order to avoid layer lines. Because it's a thermosetting plastic you can't use heat-set inserts without damaging the material. Instead, I used Injection Molding insert nuts (not exactly the best option but I really struggled to find a real press-in insert that fit both the screws and the holes).

        Injection Molding Brass Knurled Thread Inserts Nuts       [Aliexpress](https://www.aliexpress.com/item/1005004001886100.html?spm=a2g0o.order_list.order_list_main.41.46f51802OmCO8M)
        Length 5mm, M3x OD4mm

I softened the plastic of the case holes by warming them up 10-15 seconds with a hair dryer, then pressed the inserts in place using a cold soldering iron. It takes some effort to get them in; a few times I had to reheat the area for a few seconds once the nut was wedged in the hole, and then they slid in pretty easily.

I also sealed the outside of the case and keycaps with 3 layers of clear coat. I've read conflicting reports on the skin-safety of cured resin, but figured better safe than sorry, plus it is provides a small layer of scratch protection and helps with yellowing for the keycaps.

Because of the brittleness of resin prints, I used the blow dryer treatment before screwing in the PCBs out of an abundance of caution.

### RP2040-zero

I chose this controller mainly because I didn't want to be concerned about the size of my firmware. As a bonus it's cheaper.

        2x RP2040-zero Controller                                   [Aliexpress](https://www.aliexpress.com/item/1005004281549886.html?spm=a2g0o.order_list.order_list_main.29.46f51802OmCO8M)

This doesn't change much apart from the fact that due to its small size, there’s no RUN pin to attach a reset button. Instead, I wired the reset switch to the ground pin (shared with the TRRS socket), and GP29, then set up GP29 to reset the keyboard when held 2 seconds in qmk:

```c

#include "gpio.h" // Include GPIO functions

#define RESET_BUTTON_PIN GP29
#define RESET_HOLD_TIME 2000 // Time in milliseconds

void keyboard_pre_init_user(void) {
    setPinInputHigh(RESET_BUTTON_PIN); // Configure the pin as input with pull-up
}

void matrix_scan_user(void) {
    static bool button_pressed = false;
    static uint32_t button_press_time = 0;

    // Check if the button is pressed
    if (!readPin(RESET_BUTTON_PIN)) { // Active low
        if (!button_pressed) {
            button_pressed = true;
            button_press_time = timer_read(); // Record the current time
        } else if (timer_elapsed(button_press_time) >= RESET_HOLD_TIME) {
            reset_keyboard(); // Trigger the reset after 2 seconds
        }
    } else {
        // Reset state if the button is released
        button_pressed = false;
        button_press_time = 0;
    }
}

My keymap is also equipped with a tapdance key that resets the keyboard on 5 taps.

```

Notice also the controller configuration and formatting of pinouts in info.json: 
```json
    "processor": "RP2040",
    "bootloader": "rp2040",
    "split": {
        "soft_serial_pin": "GP1" 
    },
    "diode_direction": "COL2ROW",
    "matrix_pins": {
        "cols": ["GP3", "GP4", "GP5", "GP6", "GP7", "GP8"],
        "rows": ["GP28", "GP27", "GP26", "GP15", "GP14"]
    },
```
I wired the rows top to bottom, and the columns outside->in according to the [juhakaup's diagram](https://github.com/juhakaup/keyboards/blob/main/Cygnus%20v1.0/img/4x6wiring.jpg)

Other rp2040-zero specific qmk setup includes the following lines in rules.mk:
```
BOOTLOADER = rp2040
BOARD = GENERIC_RP_RP2040
SERIAL_DRIVER = vendor
```

### 4x6 Layout

Layout macro is defined in info.json, layers are enumerated in cygnus.h, and keys are mapped in keymap.c. keymaps/default/keymap.c is a single layer of basic QWERTY for those who would like to start from zero with a working layout. keymaps/williamsjag/keymap.c is my personal layout based on Hands Down Vibranium, explained in detail below.


## Layout Features

### Home Row Mods

Keys under the fingers in the hom row act as modifiers when held and as alpha keys when tapped.
For example the Mac screenshot command Shift(Command(4)) is executed with A(E(4))

### Adaptive Keys

Changes the output of the second-pressed key of specified key pairs when pressed in quick succession. Type the sequence slightly slower to avoid this adjustment. There are 2 kinds of adaptive keys in my layout: sequences that exist to avoid common SFBs (AH->AU), and "magic hash" which uses the uncommonly-typed-after-a-character '#' as a sort of auto-complete for common words and endings (H#->have)

Complete list:          AH  ->  AU              H#  ->  have
                        EH  ->  EO              F#  ->  for
                        UH  ->  UA              D#  ->  does
                        OH  ->  OE              K#  ->  know
                                                M#  ->  ment
                                                S#  ->  sion
                                                T#  ->  tion
                                                W#  ->  williams
                                                ␣#  ->  and


### Tap Dances

Allows one key send multiple different actions depending on how may times it is tapped in quick succession.

Complete list:          TD_ESC_LAYER_BOOT       1 tap       Escape
                                                3 taps      Toggle QWERTY and HD layers
                                                5 taps      Reset keyboard to Bootloader

                        TD_DOT                  1 tap       Period/Space/One-Shot Shift (end of sentence)
                                                2 taps      . (deletes space typed by first tap)
                                                3 taps      ...

### Custom Shift Keys

Changes the default behovior of certain keys when shifted

Complete list:          Shift - is +
                        Shift , is ;
                        Shift # is $
                        Shift . is :
                        Shift / is *
                        Shift " is [
                        Shift ' is ]
                        Shift 7 is ( (Num layer only)
                        Shift 9 is ) (Num layer only)

### Combos

Pressing two or more keys together to output something different. I use combos for some punctuation not present on the base layer, *h bigrams, and som common words and endings.

Complete list:          TN  ->  TH              ⌫E  ->  every
                        SN  ->  SH              ⌫H  ->  here
                        CS  ->  CH              ⌫I  ->  in the
                        WX  ->  WH              UO  ->  .com
                        PF  ->  PH              OY  ->  .fr
                        GM  ->  GH              ./  ->  !
                        CSN ->  SCH             /"  ->  ?
                        "'  ->  « | » (Pipe represents cursor position)
                        TA  ->  Toggle Caps Word
                        AI  ->  Screen capture drag box (full screen when shifted)
                        AE  ->  | (Substitute for compose key using Espanso- expansions listed below)

### Custom Keys

Several custom keys and their behaviors are defined in process_record_user in keymap.c.

Highlights:             HD_QU       types 'qu' when tapped and 'q' when held
                        HN_EQL      Shift = types != in the number layer
                        UPDIR       types '../'
                        SELW_R      Selects the word to the right of the cursor
                        SELW_L      Same but to the left
                        SELLINE     Selects the current line
                        HS_LABK     Shift < types '<=' in the symbol layer
                        HS_RABK     Shift > types '>=' in the symbol layer
                        Buttons for browser forward and back
                        Shifted Arrow keys in Num layer are mouse movements
                        Shifted Home and End in Num layer are left/right click
                        Buttons for undo/redo/cut/copy/paste/paste and match style
                        Buttons for mouse scroll wheel up and down
                        Buttons for zoom in/zoom out/reset zoom
                        Buttons for snap window left or right in Magnet
                       
### Achordion

Facilitates home row mods by preventing keys from firing with modifiers held with the same hand. Helps prevent mod misfires.

### A note on keycaps

Pictured are resin printed [DES Keycaps](https://github.com/pseudoku/PseudoMakeMeKeyCapProfiles). They are also for sale [here](https://www.asymplex.xyz/) by their designer. I wasn't sure how highly sculpted caps like this would feel in a keywell, but I have to say I love them.