groundstation
=============

A GNURadio-based ground station receiver for the Sprite spacecraft.

# Installation

    $ mkdir -p gr-sprite/build && cd gr-sprite/build
    $ cmake ..
    $ make
    $ make install

# Usage

Record I/Q samples using your favorite SDR at 437 MHz. There are GRC flowgraphs
available to do the same. For example, to use the rtl-sdr recorder:

    $ gnuradio-companion gr-sprite/examples/SpriteRecorder_RTL.grc

Then, decode Sprite transmissions with:

    $ gnuradio-companion gr-sprite/examples/SpriteReceiver2.grc

To decode the samples you recorded, disable the rtl-sdr block and enable the
File Source and Throttle.

# License

[Creative Commons Attribution-ShareAlike 4.0 International](https://creativecommons.org/licenses/by-sa/4.0/)
