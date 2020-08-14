#include "Arduino.h"
#include <SPI.h>
#include "characters.h"  //view and edit to add your own custom characters

#define lat 10 //latch though will be redister port controlled
#define PWMpin 6 // pin running brightness pwm
#define defaultBrightness 20
uint8_t brightness = defaultBrightness;

#define digitCnt 15 //count of digits to stream

boolean scroll = false;
boolean animate = false;
uint8_t animateOp = 1;
uint8_t charBuffer[digitCnt]; // input buffer for character to display, data taken from serial in from ESP8266

uint8_t scrollcharBuffer[60]; //holding space for scrolling text

//A function takes the characters and converts them to the interlaced streams bellow.
uint8_t interlaceA[digitCnt]; //data for row A
uint8_t interlaceB[digitCnt]; //data for row B
uint8_t interlaceC[digitCnt]; //data for row C
uint8_t interlaceD[digitCnt]; //data for row D

void encodeStream()
	{
		for (int i = 0; i < digitCnt; i++)
			{
				uint16_t character = returnMatch(charBuffer[i]);
				/*e.g the char 'A' returns 0b1110011010010011 which is 4 chunks of 4bits 1110, 0110, 1001, 0011.

				 I need to separate these four chunks and split them across interlace ABC and D, and add 1000 0100 0010 and 0001
				 to the end of each chunk as theyre deposited in the interlace arrays. */

				interlaceA[i] = (((character >> 8) & 0xF0) | (1 << 3)); //taking top 4 bits and setting bit 4 to 1 should give 11101000 for row a of character A
				interlaceB[i] = (((character >> 4) & 0xF0) | (1 << 2)); //taking top 4 bits and setting bit 4 to 1 should give 01100100 for row b of character A
				interlaceC[i] = (((character >> 0) & 0xF0) | (1 << 1)); //taking top 4 bits and setting bit 4 to 1 should give 10010010 for row c of character A
				interlaceD[i] = (((character << 4) & 0xF0) | (1 << 0)); //taking top 4 bits and setting bit 4 to 1 should give 00110001 for row d of  character A

			}

	}
/*
 * 							#	D3	#					#	A1	#
 * 				#		#							#							#		#
 * 				D			D						D					A				A
 * 				4					1				2			4						2
 * 				#							#		#		#							#
 * 							#	C1	#					#	A3	#
 * 				#							#		#		#							#
 * 				C					C				B				B					B
 * 				4			3						4						1			2
 * 				#		#							#							#		#
 * 							#	C2	#					#	B3	#
 */
#define a1 0b0000000000000010;
#define a2 0b1000000000000000;
#define a3 0b0100000000000000;
#define a4 0b0000010000000000;
#define a5 0b0000001000000000;
#define a6 0b0000000001000000;
#define a7 0b0000000000010000;
#define a8 0b0000000000000001;

uint8_t frame = 0;
uint16_t toEncode = 0;
void encodingAnimation(uint8_t digit, uint16_t data)
	{
		for (uint8_t i = 0; i < digitCnt; i++)
			{
				if (i == digit)
					{
						interlaceA[i] = (((data >> 8) & 0xF0) | (1 << 3)); //taking top 4 bits and setting bit 4 to 1 should give 11101000 for row a of character A
						interlaceB[i] = (((data >> 4) & 0xF0) | (1 << 2)); //taking top 4 bits and setting bit 4 to 1 should give 01100100 for row b of character A
						interlaceC[i] = (((data >> 0) & 0xF0) | (1 << 1)); //taking top 4 bits and setting bit 4 to 1 should give 10010010 for row c of character A
						interlaceD[i] = (((data << 4) & 0xF0) | (1 << 0)); //taking top 4 bits and setting bit 4 to 1 should give 00110001 for row d of  character A
					}
				else
					{
						interlaceA[i] = 0;
						interlaceB[i] = 0;
						interlaceC[i] = 0;
						interlaceD[i] = 0;
					}
			}

	}

#define frameDelay 20
void encodeAnimation()
	{
		brightness = defaultBrightness + 50;
		analogWrite(PWMpin, brightness);
		for (uint8_t i = (digitCnt * 2 + 1); i > 0; i--)
			{
				if (i % 2 == 0)
					{
						toEncode = a2
						;
						encodingAnimation((i / 2) - 1, toEncode);
					}
				else
					{
						toEncode = a1
						encodingAnimation((i / 2) - 1, toEncode);
					}
				delay(frameDelay);
			}

		for (uint8_t i = 0; i < 2; i++)
			{
				if (i < 2)
					{
						if (i % 2 == 0)
							{
								toEncode = a3

								encodingAnimation(0, toEncode);
							}
						else
							{
								toEncode = a4

								encodingAnimation(0, toEncode);
							}
					}
				delay(frameDelay);
			}

		for (uint8_t i = 0; i < (digitCnt * 2); i++)
			{

				if (i % 2 == 0)
					{
						toEncode = a5
						encodingAnimation(i / 2, toEncode);
					}
				else
					{
						toEncode = a6
						encodingAnimation(i / 2, toEncode);

					}

				delay(frameDelay);
			}

		for (uint8_t i = 0; i < 2; i++)
			{
				if (i < 2)
					{
						if (i % 2 == 0)
							{
								toEncode = a7

								encodingAnimation(digitCnt - 1, toEncode);
							}
						else
							{
								toEncode = a8

								encodingAnimation(digitCnt - 1, toEncode);
							}
					}
				delay(frameDelay);
			}
		brightness = defaultBrightness;
		analogWrite(PWMpin, brightness);
	}

uint8_t row = 0; //control for what row to draw, flipping to the next row each time timer triggers this function.
void shiftDisplay()
	{

		uint8_t d = 0; //d for digit
		//digitalWrite(lat, LOW);
		PORTB &= ~(1 << 2);

		switch (row)
			{
		case 1:
			for (d = 0; d < digitCnt; d++)
				{
					SPI.transfer(interlaceA[d]);
				}
			break;

		case 2:
			for (d = 0; d < digitCnt; d++)
				{
					SPI.transfer(interlaceB[d]);
				}
			break;

		case 3:
			for (d = 0; d < digitCnt; d++)
				{
					SPI.transfer(interlaceC[d]);
				}
			break;

		case 4:
			for (d = 0; d < digitCnt; d++)
				{
					SPI.transfer(interlaceD[d]);
				}
			row = 0;
			break;
			}
		//digitalWrite(lat, HIGH);
		PORTB |= (1 << 2);
		row++;

	}

void setup()
	{
		Serial.begin(115200);
		SPI.begin();
		SPI.setBitOrder(LSBFIRST);

		pinMode(lat, OUTPUT);		//latch

		pinMode(PWMpin, OUTPUT);//PWM brightness control will add control command in serial
		analogWrite(PWMpin, brightness);		//default to 20 (8bit)

		for (int i = 0; i < digitCnt; i++) // just to make sure display is empty on startup... Probably not needed.
			{
				charBuffer[i] = ' ';
				interlaceA[i] = 0;
				interlaceB[i] = 0;
				interlaceC[i] = 0;
				interlaceD[i] = 0;
			}

//////////////////////////////////////////////////////////////////////////////////
//setting display timer interrupt to keep a smooth display refresh
//The timer runs the display constantly.
		noInterrupts();// disable all interrupts
		TCCR1A = 0;
		TCCR1B = 0;

		TCCR1B &= ~(1 << WGM12); // WGm02 bit set to zero? for normal mode, to not clear on compare match? Is this right?
		/*WGM02:0=0 according to 328 datasheet, another example calls for WGM12...confusing*/
		TCCR1B &= ~(1 << CS12);    // 8 prescaler 2 ticks per uS
		TCCR1B |= (1 << CS11);
		TCCR1B &= ~(1 << CS10);

		TIMSK1 |= (1 << OCIE1A);  //Output compare eneabled
		OCR1A = 100; //10=500ns trigger
		TCNT1 = 0; //reset to zero

		interrupts();             // enable all interrupts
//////////////////////////////////////////////////////////////////////////////////
		String hw = "--=  Hello  =--";
		for (int i = 0; i < digitCnt; i++)
			{
				charBuffer[digitCnt - i - 1] = hw[i];
			}
		encodeStream();
		delay(1000);

		encodeStream();
		delay(2000);

		Serial.flush();    //clean out the serial junk esp8266 dishes out on startup
		delay(1000);

	}

//timer interrupt that runs the display
ISR(TIMER1_COMPA_vect)
	{
		shiftDisplay();
		TCNT1 = 0; //reset to zero
	}

uint8_t serialBuffer[65];
uint8_t serialBuffCount = 0;

bool newFrame = false;
bool packetReady = false;
uint8_t lastData = 0;
//following ACSII codes to keep some standard to follow.....
#define startFrame 0x02 //acsii code STX
#define endFrame 0x03 //acsii code ETX
#define ESC 0x1B //acsii code
#define ACK 0x06 //acsii code
#define NAK 0x15//acsii code

void resendSerial()
	{
		//dispDebug("BAD    ");
		//command resend last packet  why these bytes? just random ones that ill match in the other mcu to use as a resend (B.A.D)
		Serial.write(NAK);
	}
void confirmSerial()
	{
		//dispDebug("FAB    ");
		// confirm received packet no issues... Maybe will add a auto resend if no confirmation on other mcu F.A.B (thunderbirds... , cant do S.I.G for Captain Scarlet D: )
		Serial.write(ACK);
	}
void flush()
	{
		uint8_t chard = 0;
		while (Serial.available() > 0)
			{
				chard = Serial.peek();
				if (chard != startFrame)
					{
						Serial.read();
					}
				else
					{
						break;
					}
			}
		newFrame = false;
		serialBuffCount = 0;
	}

void getSerial()
	{

		/*
		 Serial structure  uint8_t chars[21]{startFrame,0xA0,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','\n',0,0,endFrame};
		 | Header Byte | Length of Packet including Header and 2 byte checksum, in integer (ir 10 bytes 20 40 50...)| Data | 2 byte Checksum |

		 Start Frame | Header Byte  |							 Data		 								|											 Checksum 												|  End Frame
		 Fixed digit display	=						0x02							0xA0								char Stream	+\n						sum of bytes added up bytes							0x03												Note:		Non scrolling fixed display, char stream should be limited to character count
		 Scrolling display   	=						0x02							0xB0								char Stream		+\n					sum of bytes added up bytes							0x03 											Note:		Scrolling display, char stream limited to 60 characters, can be increased by increasing serialBuffer global, but buffer must also include space for header length and checksum.
		 Special Animate			=						0x02							0xC0						Animation option	+\n			sum of bytes added up bytes							0x03												Note:		Triggers special scrited animation
		 Target Led								=						0x02							0xD0						2b Hex + Digit	+\n					sum of bytes added up bytes							0x03												Note: 		Targeted Led turn on, 2 byte data using Hex to address leds and dec to address digit e.g 0xA2 and 4 = Led A2 of Digit 4, 0xB4 + 23 = Led B4 of digit 23.

		 */

		uint8_t data = Serial.read();
//get serial data
		if (data == startFrame && lastData != ESC)
			{
				newFrame = true;
				packetReady = false;
				serialBuffCount = 0;
				serialBuffer[0] = data;
				lastData = data;

				//	dispDebug(" frame start ");

			}

		if (data == endFrame && lastData != ESC)
			{
				newFrame = false;
				packetReady = true;
				serialBuffer[serialBuffCount++] = data;
				lastData = data;

				//	dispDebug(" end frame ");
				// code to end the frame
			}

		//checking all states though maybe superfluous as the grabs for framing and esc is above...
		if (((data == endFrame && lastData == ESC)
		    || (data == startFrame && lastData == ESC)
		    || (data == ESC && lastData == ESC)
		    || (data != endFrame || data != startFrame || data != ESC))
		    && packetReady == false)
			{
				serialBuffer[serialBuffCount++] = data;
				lastData = data;
			}

		if (packetReady == true)
			{
				uint8_t command = serialBuffer[1];

				uint8_t length = 0;
				while (true) // work out length of message
					{
						if (serialBuffer[length] != '\n')
							{
								length++;
							}
						else
							{
								break;
							}
					}

				length = length - 2; //Length of message minus startFrame and Command byte
				uint8_t text[length] = { 0 };

				// error check//

				//head check
				if (serialBuffer[1] != 0xA0 && serialBuffer[1] != 0xB0
				    && serialBuffer[1] != 0xC0 && serialBuffer[1] != 0xD0) //expected headers, add any headers you like for extra commands
					{ //invalid head

						uint8_t head = serialBuffer[1];
						char buffer[15];
						sprintf(buffer, " Bad Head %x  ", head);
						dispDebug(buffer);

						flush();
						resendSerial();
						goto failEnd;
					}

				//checksum check
				//add up checksum this side
				uint16_t checksum = 0; // to calculate

				for (int c = 0; c < length + 3; c++) //Length + \n + start frame + command byte = +3
					{
						if (serialBuffer[c] != ESC) //ignore escape bytes in checksum, as ignored in the generation
							{
								checksum = checksum + serialBuffer[c];
							}

					}

				//Reconstitute received checksum two bytes for 16bit int.
				//Bug fix, before ESC wasnt handled in checksum bytes, now its captured and bytes offset.
				//not the tidest of code but its working so far.
				char buffer[15];

				uint8_t readStart = length + 3;

				uint8_t escOffset1 = 0;
				if (serialBuffer[readStart] == ESC)
					{
//					sprintf(buffer, " off1 %i  ", serialBuffer[readStart]);
//					dispDebug(buffer);
//					delay(500);

						escOffset1 = 1;
					}

				uint8_t escOffset2 = 0;
				if (serialBuffer[readStart + 1 + escOffset1] == ESC)
					{
//						sprintf(buffer, " off2 %i  ", serialBuffer[readStart+1+escOffset1]);
//						dispDebug(buffer);
//						delay(500);

						escOffset2 = 1;
					}

				uint8_t checkHIGH = serialBuffer[(readStart + escOffset1)];
				uint8_t checkLOW =
				    serialBuffer[(readStart + 1 + escOffset1 + escOffset2)];

//						sprintf(buffer, " cs1 %i  ", checkHIGH);
//						dispDebug(buffer);
//						delay(500);
//
//						sprintf(buffer, " cs2 %i  ",  checkLOW);
//						dispDebug(buffer);
//						delay(500);

				uint16_t checksumRe = checkHIGH << 8 | checkLOW << 0;
				//Compare received against this side added up checksum

				if (checksum != checksumRe)
					{
						uint16_t cs = checksumRe;

						sprintf(buffer, " cs %i  ", cs);
						dispDebug(buffer);
						delay(1000);

						//checksum failed resend packet
						flush();
						resendSerial();
						goto failEnd;
					}

//run data

				bool ignore = false;
				uint8_t targetByte = 0;

				for (uint8_t i = 0; i < length; i++)
					{ // +3 as thats the text portion of the packet
						if (serialBuffer[i + 3] == ESC
						    && (serialBuffer[i + 4] == startFrame
						        || serialBuffer[i + 4] == endFrame
						        || serialBuffer[i + 4] == ESC))
							{
								ignore = true;
							}
						else
							{
								ignore = false;
							}

						if (ignore == false)
							{
								text[targetByte] = serialBuffer[i + 2];
								targetByte++;
							}
					}

				if (serialBuffer[1] == 0xA0) 	//display fixed readout of text
					{
						scroll = false;
						animate = false;
						for (uint8_t i = 0; i < length; i++)
							{
								charBuffer[i] = text[length - 1 - i]; //transfering data to the display buffer for encoding.
							}
						encodeStream();
					}

				if (serialBuffer[1] == 0xB0) //scrolling text, code not written yet for this.... by all means finish it.
					{
						scroll = true;
						animate = false;
						for (uint8_t i = 0; i < length; i++)
							{
								charBuffer[i] = text[length - i]; //transfering data to the display buffer for encoding.
							}
						//set scrolling triggers and encode loops.......
					}

				if (serialBuffer[1] == 0xC0) 	//set animations, only one at the moment
					{
						scroll = false;
						animate = true;
						if (text[0] == "1")
							{
								animateOp = 1;
							}
					}

				if (serialBuffer[1] == 0xD0) //target led, code not written for this yet. But serial is fast enough that in theory you could run animations from host mcu
					{
// Write something interesting here that people might read!
					}

				failEnd: // if checks fail skips to here
				for (int i = 0; i < sizeof(serialBuffer); i++)
					{
						serialBuffer[i] = NULL;
					}
				packetReady = false;
			}
	}

void dispDebug(String msg)
	{
		for (int i = 0; i < digitCnt; i++)
			{
				charBuffer[digitCnt - i - 1] = msg[i];
			}
		encodeStream();
		delay(50);
	}
int cnt = 0;
void loop()
	{
		if (Serial.available() > 0)
			{
				getSerial();
			}
		else
			{
				if (animate == true)
					{
						encodeAnimation();
					}
			}

	}
