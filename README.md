# liusificator

Fimware for arduino based massage robot

Works with two servos as manipulators, supports three basic modes

* neutral mode, device is powered but noop
* gauge mode, is used to set angle for servos (massage strength)
* operating mode

You choose mode by clicking buttons attached to pins 0, 2, 3 respectively (TODO, why pin 1 is not working in PULLUP_INPUT mode).

Selected mode is highlighted by LED, one attached to pin 8 for gauge mode and pin 9 for operating mode.

In gauge mode you choose angle of servos by changing resistance attached to signal pin A0.

Massage is what Liu really loves :)
