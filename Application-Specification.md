# Etch A Sketch Specification
## Introduction
The Etch A Sketch is a simple-to-use digital drawing board that allows the user to turn knobs to draw lines on an LCD screen and shake the board in order to erase their drawing. While the board is still plugged in, the user can keep drawing and erasing their creation for as long as they please. The materials required to properly operate the Etch A Sketch are as follows: </br>
* L152RE NUCLEO Board
* mbed shield with a Navigation switch, potentiometers, RGB LED, accelerometer, and LCD to plug on top of the NUCLEO board
* USB to USB 2.0 mini-B connector
* Laptop or personal computer
</br>
The drawing board uses a L152RE NUCLEO microcontroller with an mbed shield plugged in on top. The board will be wired to the user’s laptop or PC with the USB to USB 2.0 mini-B wire and will output the user-controlled drawings with the LCD. To properly operate the Etch A Sketch program, the user will power up their NUCLEO board and interact with the potentiometers, accelerometer, 5-way Navigation Switch, and the LCD. 

## Tolerances
At this time, there are no tolerances to be applied to this specification.

## Operation
* Apply power to the NUCLEO board through the USB serial connection to a PC or laptop.
* On bootup, the LCD screen will be blank.
* The RGB LED will be red on bootup to indicate it is in “pencil up” mode. This means that turning the potentiometers will move a cursor on the screen without drawing a line, allowing the user to move to a point on the LCD screen in which they want to begin drawing at.
* The user will push the navigation switch to the right to enter “pencil down” mode, indicated by the RGB LED turning green. This mode allows the user to begin drawing lines on the LCD screen by turning the potentiometers. Pushing the navigation switch to the left will put it back into “pencil up” mode.
* To move the pencil horizontally, the user can turn the left knob (potentiometer) clockwise to move right and counterclockwise to move left.
* To move the pencil vertically, the user can turn the right knob clockwise to move up and counterclockwise to move down.
* To adjust the width of the drawing line, the user can press up on the navigation switch to increase the line width from 1 pixel (default) to 2 pixels. Pressing down on the navigation switch will decrease the width back to 1 pixel (these are the only two options).
* To turn the screen into negative mode, the user can press the center navigation switch button. This turns all black pixels on the LCD screen into white pixels and vice versa.
* To clear the LCD screen, permanently erasing all the drawn pixels, the user must pick up and shake the NUCLEO board for a couple seconds.
