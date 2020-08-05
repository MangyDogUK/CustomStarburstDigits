Custom Modular Starburst Digit
 
A Custom designed modular Starburst Digit, inspired by the displays on the Hornet fighter jet. 
Taking the original concept of the displays from the 1970s, that used tungsten bulbs, and used fibreoptic light pipes to light the segments of the display.
This project takes the visual concept and modernises it, using LEDs and shift registers to create a modular daisy-chainable display system.

At the time of writing this, the included software is functional and has only a few minor bugs and not all planned features are written. 

The digits use an arduino (atmega328p) to drive them, and basic functions are working as they should. Currently not written, is scrolling text and targeted led control through serial.

The digits receive data through SPI hardware and a hardware timer constantly drives the display, printing out a saved buffer. The software updates that buffer and in turn the display starts displaying that data.

Currently I'm realsing this under the Creative Commons License. 
Basic gist, feel free to use this and modify any designs for personal use. That could be making your own displays or even to use in your own built flightsims.
What you cant do is commercialise this project or derivative of this project in any way.

If you wish to commercialise this, to sell or make units to sell contact me and we can arrange a license fee, as well as more in-depth support.
