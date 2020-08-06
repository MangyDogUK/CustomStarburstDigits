/*
 * characters.h
 *
 *  Created on: 5 Aug 2020
 *      Author: Mangy_Dog
 */

#ifndef CHARACTERS_H_
#define CHARACTERS_H_
//
/*
 *Diagram of the leds according to ROW and LED of the 4x4 matrix on 74HC595 8bit shiftreg
 *The display function will cycle the 4 rows in 4 bit chunks of the character defines bellow using persistence of vision to display the whole character.
 *
 *I honestly cant remember why I didnt do a proper 1234 pattern, where each corner had the same led order as the last, might have been for ease of pcb routing.
 *
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
 *
 *
 */
///***********  |  A  |   B  |   C  |  D   |*/
////#define nul 0b0000'0000'0000'0000; //null empty blank nothing to see here!
//upper case
#define uA 0b1110010010010011;
#define uB 0b0010011011010111;
#define uC 0b1000001001010011;
#define uD 0b0000101001011001;
#define uE 0b1010001011010011;
#define uF 0b1010000010010011;
#define uG 0b1010011001010011;
#define uH 0b0110010010010001;
#define uI 0b1000001101000110;
#define uJ 0b1000000101000110;
#define uK 0b0001100010010001;
#define uL 0b0000001001010001;
#define uM 0b0101010000011001;
#define uN 0b0100110000011001;
#define uO 0b1100011001010011;
#define uP 0b1110000010010011;
#define uQ 0b1100111001010011;
#define uR 0b1110100010010011;
#define uS 0b1010011011000011;
#define uT 0b1000000100000110;
#define uU 0b0100011001010001;
#define uV 0b0001000000110001;
#define uW 0b0100011101010101;
#define uX 0b0001100000101000;
#define uY 0b0001000100001000;
#define uZ 0b1001001001100010;

#define space 0b0000000000000000;

//lower case
#define la 0b0000001111010000;
#define lb 0b0000000111010001;
#define lc 0b0000000011010000;
#define ld 0b0000000111010100;
#define le 0b0000000011010111;
#define lf 0b0000000010010011;
#define lg 0b0000000111000111;
#define lh 0b0000000110010001;
#define li 0b0000000000010001;
#define lj 0b0000000101000100;
#define lk 0b0001100100000100;
#define ll 0b0000001100000100;
#define lm 0b0010010110010000;
#define ln 0b0000000110010000;
#define lo 0b0000000111010000;
#define lp 0b0000000010010111;
#define lq 0b0000000110000111;
#define lr 0b0000000010010000;
#define ls 0b0000000111000011;
#define lt 0b0000000011010001;
#define lu 0b0000000101010000;
#define lv 0b0000000000110000;
#define lw 0b0000011101010000;
#define lx  0b0001100000101000;
#define ly 0b0000000111000101;
#define lz 0b0000000011100000;

//numbers
#define n0 0b1101011001110011;
#define n1 0b0000001101000110;
#define n2 0b1110001011010010;
#define n3 0b1110011011000010;
#define n4 0b0010000110000101;
#define n5 0b1010011011000011;
#define n6 0b1010011011010011;
#define n7 0b1001000000100010;
#define n8 0b1110011011010011;
#define n9 0b1110011011000011;

//symbols
#define error 0b1110011111010111;
#define si 0b0000000011000000;
#define sEq 0b0010001011000000;
#define sMin 0b0010000010000000;
//
//When ever you add a character define, add it to the returnMatch function bellow so it would get converted at runtime to the display encoder.
uint16_t returnMatch(char chara)
	{
		//converting true character data to 16 bit led encoded data from the defines at the top.
		//Feel free to add custom character encodes for any symbol you'll like...
		switch (chara)
			{
		case 'A':

			return uA
			break;

		case 'B':

			return uB
			break;

		case 'C':

			return uC
			break;

		case 'D':

			return uD
			break;

		case 'E':

			return uE
			break;

		case 'F':

			return uF
			break;

		case 'G':

			return uG
			break;

		case 'H':

			return uH
			break;

		case 'I':

			return uI
			break;

		case 'J':

			return uJ
			break;

		case 'K':

			return uK
			break;

		case 'L':

			return uL
			break;

		case 'M':

			return uM
			break;

		case 'N':

			return uN
			break;

		case 'O':

			return uO
			break;

		case 'P':

			return uP
			break;

		case 'Q':

			return uQ
			break;

		case 'R':

			return uR
			break;

		case 'S':

			return uS
			break;

		case 'T':

			return uT
			break;

		case 'U':

			return uU
			break;

		case 'V':

			return uV
			break;

		case 'W':

			return uW
			break;

		case 'X':

			return uX
			break;
		case 'Y':

			return uY
			break;

		case 'Z':

			return uZ
			break;

			////
		case 'a':

			return la
			break;

		case 'b':

			return lb
			break;

		case 'c':

			return lc
			break;

		case 'd':

			return ld
			break;

		case 'e':

			return le
			break;

		case 'f':

			return lf
			break;

		case 'g':

			return lg
			break;

		case 'h':

			return lh
			break;

		case 'i':

			return li
			break;

		case 'j':

			return lj
			break;

		case 'k':

			return lk
			break;

		case 'l':

			return ll
			break;

		case 'm':

			return lm
			break;

		case 'n':

			return ln
			break;

		case 'o':

			return lo
			break;

		case 'p':

			return lp
			break;

		case 'q':

			return lq
			break;

		case 'r':

			return lr
			break;

		case 's':

			return ls
			break;

		case 't':

			return lt
			break;

		case 'u':

			return lu
			break;

		case 'v':

			return lv
			break;

		case 'w':

			return lw
			break;

		case 'x':

			return lx
			break;
		case 'y':

			return ly
			break;

		case 'z':

			return lz
			break;

/////
		case ' ':
			return space
			break;

		case '0':
			return n0
			break;

		case '1':
			return n1
			break;

		case '2':
			return n2
			break;

		case '3':
			return n3
			break;

		case '4':
			return n4
			break;

		case '5':
			return n5
			break;

		case '6':
			return n6
			break;

		case '7':
			return n7
			break;

		case '8':
			return n8
			break;

		case '9':
			return n9
			break;
//symbols

		case ':':
			return si
			break;

		case '=':
			return sEq
			break;

		case '-':
			return sMin
			break;

		default:
			return error
			break;

			}
	}

#endif /* CHARACTERS_H_ */
