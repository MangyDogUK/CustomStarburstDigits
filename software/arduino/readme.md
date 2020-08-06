The digits are driven by an atmega328p, I used a arduino pro mini.
The shift registers are connected to SPI hardware that constantly driven by the hardware timer. 
The data is sent in an interlaced 4 row cycle... and currently the sweet spot seems to be data being sent at a 500ns interval, which is more than fast enough for vision persistance without any flicker or phase issues with PWM dimming.


