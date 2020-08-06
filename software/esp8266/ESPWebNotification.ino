/***************************************************************************
 This program uses a mix mash of various guides found online for using common online API's
 for getting data such as the time and sub stats.

 Swap the credentials out for your own........

 Data handling and encoding is custom designed for the display. And any edits you make to the
 code here must be reflected in the display hardware control  program on the arduino.

 I will try to keep this human readable and relatively easy to follow......... I say I'll try cant guarantee it...!

 ****************************************************************************/
#include "Arduino.h"
//The setup function is called once at startup of the sketch
#include <ESP8266WiFi.h>
#include <time.h>


const char* ssid = "Dog-Fluff-Everywhere!"; // place your SSID here
const char* password = "love-sex-secret-god-password-hack_the_planet"; //Place your password here!

#include <WiFiClientSecure.h>
//https://github.com/witnessmenow/arduino-youtube-api
#include <YoutubeApi.h>
#include <ArduinoJson.h>
//////////////////////////////////////////////////////////////////////////////////////////
#define API_KEY "KeysKEYS-EVERYWHERE"  // your google apps API Token
#define CHANNEL_ID "YOURE ID NOT MINE" // makes up the url of channel
//////////////////////////////////////////////////////////////////////////////////////////

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);



long subs = 0;






#define bufferSize 65




class displayDigits
	{
//special serial bytes following ACSII codes to keep some standard to follow.....
#define startFrame 0x02 //acsii code STX
#define endFrame 0x03 //acsii code ETX
#define ESC 0x1B //acsii code
#define ACK 0x06 //acsii code
#define NAK 0x15//acsii code

private:
	char buffer[bufferSize]; //65 being arbitrary 60 characters plus  special bytes ... Raise it if you like, but be sure to raise it in the arduino sketch as well
	bool scrolling = false;
	uint8_t digitCnt;

	void checksum() // calculates checsum and injects it into the packet
		{
			uint16_t checkSum = 0;
			int c = 0;
			while (true)
				{
					if (buffer[c] != '\n')
						{
							checkSum = checkSum + buffer[c];
							c++;
						}
					else
						{
							checkSum = checkSum + buffer[c];
							break;
						}
				}
			buffer[c + 1] = checkSum >> 8;
			buffer[c + 2] |= checkSum << 0 & 0xff;
		}

public:

	displayDigits(uint8_t digits)
		{
			digitCnt = digits;
		}

	void setText(bool scroll, char *text)
		{
			scrolling = scroll;

			buffer[0] = startFrame;

			int c = 0;
			if (scrolling == false)
				{
					buffer[1] = 0xA0;
				}
			else
				{
					buffer[1] = 0xB0;
				}
			while (true)
				{
					if (buffer[c] != '\n')
						{
							buffer[2 + c] = text[c];
							c++;
						}
					else
						{
							buffer[2 + c] = text[c];
							break;
						}
				}

		//checksum(); // calculates checsum and injects it into the packet

			buffer[c + 3] = endFrame;

		}
	void setAnimation(char ani)
		{
			buffer[0]=startFrame;
			buffer[1] = 0xC0;
			buffer[2]=ani;
			buffer[3]='\n';
			buffer[6] = endFrame;
		}

	void write(void)
		{
			uint16_t checkSum = 0;

			int c = 0;
			while (true)
				{
					if (buffer[c] != '\n')
						{
							checkSum = checkSum + buffer[c];
							c++;
						}
					else
						{
						checkSum = checkSum + buffer[c];
							break;
						}
				}
			buffer[c + 1] = checkSum >> 8;
			buffer[c + 2] = checkSum << 0;

			uint8_t offset = 0;
			for (int i = 2; i <(c + 3 + offset); i++) //inject ESC in message and checksum if special bytes were used
				{
					if ((buffer[i] == startFrame && buffer[i - 1] != ESC)
					    || (buffer[i] == endFrame && buffer[i - 1] != ESC))
							//|| (buffer[i] == ESC && buffer[i - 1] != ESC))

						{
							for (int s = (c + 3+offset); s >=i; s--)
								{
									buffer[s+1] = buffer[s];
								}
								offset++;
						buffer[i] = ESC;
						}

				}

			for (int i = 0; i <(c + 4 + offset); i++)
				{
					Serial.write(buffer[i]);
				}
		}
	};



int timezone = 0;
int dst = 0;

displayDigits myDigits = displayDigits(15); //setting the number of digits in chain

void setup()
	{

		Serial.begin(115200);

		Serial.setDebugOutput(0);//a way to force off the debug output, causing issues. Still not working though.....

		delay(100);
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, password);


		delay(1000);
		myDigits.setAnimation('1');
		myDigits.write();
		delay(1000);

		while (WiFi.status() != WL_CONNECTED)
			{

				delay(1000);
			}
		 client.setInsecure();


		configTime(1 * 3600, 0, "pool.ntp.org", "time.nist.gov");

		while (!time(nullptr))
			{

				delay(1000);
			}

	}
unsigned long timeBetweenRequests = 2400000;
unsigned long nextRunTime;
uint16_t subCount=0;

void loop()
	{
		 if (millis() > nextRunTime)  {
		    if(api.getChannelStatistics(CHANNEL_ID))
		    {
		    		subCount=api.channelStats.subscriberCount;
		    }
		    nextRunTime = millis() + timeBetweenRequests;
		  }


		time_t rawtime;
		struct tm *timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		char timeChar[6];
		strftime(timeChar, sizeof(timeChar), "%H:%M", timeinfo);

		char buffer[bufferSize];
		sprintf(buffer,"%s  Sub %i\n",timeChar,subCount);//end in \n new line otherwise program will get stuck in a constant loop

		myDigits.setText(false, buffer);
		myDigits.write();

		delay(500);
	}

