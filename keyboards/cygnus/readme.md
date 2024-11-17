# cygnus

![cygnus](imgur.com image replace me!)

*A short description of the keyboard/project*

* Keyboard Maintainer: [Jackson Williams](https://github.com/williamsjag)
* Hardware Supported: rp2040, individual PCBs
* Hardware Availability: *Links to where you can find this hardware*

Compile example for this keyboard (after setting up your build environment and moving cygnus folder into qmk_firmware/keyboards):

    `qmk compile -kb cygnus -km default`

Flashing example for this keyboard:

    drag to usb storage of controller in boot mode

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

rp2040-zero doesn't have a RUN pin so bootloader is handled with a tapdance on the Escape key- keyboard is reset after tapping 5 times within the tapping term

