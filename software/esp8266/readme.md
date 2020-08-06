Data gathering and writing to display controller....
I used guides for time and youtube apis, but digit writing code is fully custom.
I will likely convert this class into a easily transportable library at some point.
the digits control class should work with any MCU.


One current known bug which I've yet to figure out... In some combinations of text being sent (in my case time and sub count) I get a bad checksum. Whats actually happening is a 0x02 frame start byte is being sent just before the 0x03 frame end byte in the packet.This throws off the checksum on the control MCU side as it expects the last two bytes before the frame end byte to be the checksum value. 

This doesnt happen all the time, and its certainly not programmed to do this. I will try to fix this when I have more free time to do so.
