/*
 * Antispy ultrasonic device for preventing voice recording 
 * with ultrasonic waves generation based on ATTINY85,
 * ATTINY85 fuses have been set to internal PLL clock 16MHz 
 * with Resistor Ladder 5-bit Digital-to-Analog converter
 * and piezzo-electric driver
 * (C) Adam Loboda 2021, adam.loboda@wp.pl
 */

#include <stdint.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

// PIN - PORT ASSIGNMENT DEFINITION FOR 5-BIT DAC DEVICE
#define	OUTPUTPORT		PORTB
#define	OUTPUTPORTCTRL		DDRB

 
// created in Excell / Libreoffice - will randomly 
// swing transducer frequency around 25 kHZ with 0,4 kHz steps
// to achieve kind of white noise
const uint8_t  freqrandom[] PROGMEM = 
  {
5,10,0,6,2,6,2,2,2,6,3,4,3,7,6,10,
2,1,4,0,3,8,4,10,5,1,1,1,5,0,7,10,
9,10,9,2,0,10,6,3,0,7,8,4,10,1,2,
1,3,5,2,4,2,5,9,3,0,5,0,10,0,5,5,6,
6,10,5,2,7,3,8,4,5,5,3,0,0,4,8,10,
6,4,10,0,1,7,4,6,3,10,9,0,4,2,10,8,
8,6,3,1,10,9,10,4,1,7,3,10,0,6,3,1,
2,5,2,10,0,0,3,9,9,8,8,3,8,1,10,3,
5,7,4,10,5,2,0,7,5,2,6,7,7,10,9,2,
4,6,6,1,3,4,6,9,7,5,1,2,8,4,2,5,9,
5,1,3,1,8,4,10,5,4,6,8,7,10,9,6,5,
7,1,2,0,1,5,2,3,7,5,8,9,1,7,10,7,2,
5,10,6,7,6,2,0,8,8,5,1,7,8,7,5,9,8,
7,1,7,2,7,4,0,9,3,0,2,5,0,4,3,6,4,1,
10,7,5,0,8,8,9,1,0,8,8,7,2,8,0,6,7,
5,6,5,5,5,4,0,5,0,4,4,5,2,7,6,6,10,
4,7,5,1,7,7,10,6,2,0,0,1,6,3,0,2,7,
1,1,5,0,6,10,8,2,1,1,4,10,1,0,7,3,2,
5,6,1,6,2,1,3,2,6,8,10,2,7,9,1,9,6,0,
2,8,2,1,6,9,7,9,10,8,4,1,7,4,2,2,8,9,
9,3,0,7,3,8,0,7,2,10,8,3,4,9,4,0,0,0,
1,8,5,8,2,9,4,6,8,3,5,8,9,3,10,3,0,10,
7,3,10,3,6,7,6,4,4,7,4,2,5,3,4,9,1,5,
1,9,0,2,9,8,6,5,0,0,4,10,3,5,9,4,0,7,
10,1,7,0,10,4,0,3,9,7,6,7,4,8,6,5,1,7,
1,2,2,7,8,3,0,10,3,0,3,0,4,1,0,1,4,0,1,
10,7,5,2,3,10,9,2,0,9,7,2,2,2,2,5,8,10,
10,10,7,10,4,6,8,4,10,5,7,9,7,9,6,4,10,
4,0,8,7,7,9,2,4,5,10,9,4,6,2,10,1,10,2,
9,10,9,4,10,9,8,1,5,9,0,0,1,5,6,4,9,1,
9,10,10,8,3,4,2,2,0,3,7,10,4,4,8,2,8,7,
9,10,8,4,7,5,8,8,7,6,2,6,2,6,5,5,7,7,8,
1,4,9,7,6,10,10,7,8,2,5,3,10,4,4,3,5,10,
9,3,4,2,4,10,7,5,10,7,10,9,8,10,4,2,8,4,
8,4,0,10,0,5,4,4,6,7,2,9,8,2,3,4,5,7,8,7,
9,6,0,6,1,7,3,9,0,10,6,10,10,2,2,2,8,8,
8,7,9,0,2,9,10,8,0,6,6,4,1,7,1,8,4,0,4,
3,3,0,1,0,5,9,6,0,7,6,4,10,10,7,1,4,0,
10,2,7,9,4,9,7,9,8,2,9,3,2,10,9,8,6,9,
9,3,3,3,7,5,9,3,6,9,1,5,2,3,9,1,1,0,9,
4,8,4,9,4,4,0,3,2,4,0,4,4,1,0,10,2,2,
0,9,5,5,9,0,8,3,6,0,5,2,2,4,4,1,10,10,
0,8,5,4,10,5,4,9,0,2,7,0,4,1,1,6,0,0,
3,0,4,4,1,1,1,0,1,4,8,9,9,8,3,7,9,0,4,
4,1,8,9,1,2,0,8,9,5,10,3,7,7,1,1,9,0,7,
5,7,8,0,3,9,0,9,3,5,6,5,10,9,1,1,4,6,1,
10,7,10,8,3,10,7,4,3,10,0,2,10,10,9,2,
1,8,10,8,9,3,2,2,4,2,1,9,4,8,2,0,7,1,3,
2,0,1,3,8,0,4,0,9,2,1,5,0,5,8,4,9,9,0,
4,9,8,6,0,1,0,5,6,6,2,5,2,10,1,2,4,2,2,
3,7,1,7,3,8,5,5,7,10,6,10,6,0,8,0,5,6,8,
3,1,2,8,6,4,0,3,1,8,0,7,2,5,7,3,6,6,5,10,
5,0,6,8,9,6,7,7,0,5,3,6,10,3,7,9,5,8,3,1,
0,4,1,10,4,1,5,5,0,2,6,8,2,5,2,7,7,6,8,9,
0,10,1,4,7,3,5,6,3,8,7,0,10,1,9,4,7,1,8,
8,4,1,6,6,5,9,7,4,1,3,10,1,8,2,2,6,7,10,3    
};


// created in Excell / Libreoffice - will randomly 
// change amplitude of the pulse coded in 5 bit values
// to achieve kind of white noise
const uint8_t  amprandom[] PROGMEM = 
  {

11,1,4,10,30,10,4,17,11,18,28,10,25,12,10,28,15,9,28,
15,17,9,8,20,29,28,2,17,19,14,7,20,18,18,1,26,21,4,13,
26,19,29,21,28,12,17,31,17,20,26,27,7,6,14,23,29,7,28,
3,18,13,31,13,28,23,28,4,9,8,22,22,22,6,22,12,4,29,12,
14,5,18,24,8,20,26,7,21,5,8,22,26,18,22,2,29,9,19,19,6,
16,9,22,1,21,23,6,11,9,21,22,30,15,6,9,22,30,18,19,18,
9,21,15,12,27,3,29,14,9,17,11,18,25,14,5,2,20,5,4,19,26,
31,18,4,20,3,16,3,26,2,14,22,7,7,29,18,18,29,18,16,13,
30,20,9,27,27,15,1,1,27,21,20,1,3,3,18,3,7,4,25,15,22,
16,27,8,8,30,31,20,24,4,11,28,27,24,24,29,8,1,31,21,19,
8,27,1,12,10,24,3,6,5,22,29,8,4,11,2,29,7,27,30,4,14,16,
24,19,14,3,14,9,30,21,15,19,16,13,27,13,25,18,14,12,22,
22,26,14,28,21,6,23,1,14,23,12,21,7,23,15,1,3,18,19,13,
4,28,17,27,6,10,5,1,18,23,21,25,24,12,3,31,18,9,18,10,1,
9,13,31,6,24,12,5,8,23,7,26,18,5,5,21,5,10,16,5,28,2,10,
11,13,5,24,12,12,8,13,20,7,21,1,15,31,26,24,29,28,6,9,10,
22,14,12,13,23,13,29,9,26,7,28,27,1,23,8,23,2,28,19,21,
15,18,3,2,31,29,28,8,27,27,16,4,22,5,28,28,1,4,3,6,25,2,
31,18,25,28,6,5,21,11,11,24,6,17,1,25,24,1,20,6,12,13,4,
18,23,19,14,13,7,9,13,8,20,21,18,31,14,24,9,8,25,9,20,16,
22,15,21,1,12,31,22,23,5,10,9,27,19,20,12,26,17,10,2,27,
11,24,2,28,11,16,1,25,14,10,29,1,2,12,19,2,4,30,7,18,1,7,
26,14,7,29,19,15,2,24,10,4,15,19,12,22,17,2,5,9,15,10,24,
22,3,28,28,9,26,5,11,24,13,22,12,1,20,29,22,13,23,28,11,
16,14,11,14,31,23,18,26,5,8,25,9,4,31,18,31,27,19,20,5,11,
8,18,15,20,2,20,16,13,19,29,2,10,29,25,9,12,29,20,1,5,27,
8,1,12,7,28,29,23,14,18,21,10,8,12,15,7,20,12,5,17,19,13,
24,1,28,8,22,10,16,27,18,24,9,25,14,22,12,28,16,6,10,23,
24,16,16,19,31,8,3,13,31,5,21,23,21,1,17,6,18,15,20,28,26,
22,12,6,20,19,6,13,14,13,7,21,9,4,2,22,20,14,9,13,23,19,27,
2,1,4,18,12,12,13,1,11,11,8,18,16,7,28,3,31,8,12,1,29,13,
23,12,16,3,10,11,15,1,15,21,9,14,8,2,7,30,31,5,18,8,18,16,
1,30,14,2,12,18,19,12,18,21,24,19,11,9,28,31,3,31,6,24,2,28,
4,4,17,21,11,13,15,6,6,18,28,10,17,5,8,30,28,6,27,30,14,20,
19,21,16,26,7,17,24,13,13,18,27,12,13,31,21,8,17,21,26,9,17,
15,26,29,29,14,13,4,6,31,22,26,10,6,16,12,15,21,14,4,5,18,5,
2,3,7,23,10,29,8,24,17,28,30,11,14,16,11,31,25,4,6,5,19,21,
17,24,29,23,17,12,7,22,7,11,12,30,15,26,4,6,31,6,20,26,25,
20,14,27,25,1,17,8,1,1,10,23,21,2,29,20,24,11,7,16,5,28,28,
4,28,1,21,29,24,17,18,29,24,18,17,7,20,29,28,10,16,10,6,11,
14,23,7,31,13,2,9,22,9,26,17,14,28,24,6,30,15,20,19,2,27,23,
24,7,28,14,10,25,31,8,17,10,11,16,31,28,16,8,9,1,27,28,19,
11,1,1,3,24,20,1,26,18,20,21,6,17,9,23,27,4,16,20,31,12,13,
26,2,4,27,22,15,4,16,1,4,17,6,23,27,9,25,15,8,3,7,29,14,21,
12,31,28,30,26,2,16,18,21,20,6,14,17,10,16,8,10,21,30,8,14,
30,25,24,17,10,9,10,19,25,19,15,28,18,11,21,23,25,4,14,30,31,
17,18,10,30,10,1,9,18,24,25,25,9,22,12,23,1,17,23,27,23,29,
22,2,18,17,24,26,19,16,16,4,2,13,19
};


int main(void)
{
        uint16_t sequence;
        uint8_t pulsewidth, i;


        // SET OUTPUT PIN ON ALL OUTPUT PORTS
        
        OUTPUTPORTCTRL = 0b00011111;  //set all 5 LOWER bit pins as output
        OUTPUTPORT     = 0b00000000;  //set all 5 LOWER bit pins to ZERO

       //* neverending loop */
       while (1) 
       { 

          // generating WAVE on BIT0-BIT5 DAC outputs
          // 48 = 27,2 kHZ, added values (0-10) to drop randomly within 23-27kHz frequency
          // random values are taken from PROGMEM lookup table, step is 0,4 kHZ

          for(sequence=0; sequence<1000; sequence++)
           {
           // calculate pulse width , 46 is offset
           pulsewidth = pgm_read_byte(freqrandom + sequence) + 46 ;

           // send HIGH VOLTAGE - this time square wave
           // PB0 may serve as GND for PAM8403 module audio input
           // OUTPUTPORT = 0b00000111;
           
           // sending random value of AMPLITUDE to DAC on resistors to achieve AM modulation 
           OUTPUTPORT = pgm_read_byte(amprandom + sequence); 

           
           // now delay to achieve desired frequency
           for(i=0; i<pulsewidth; i++)
             {
              asm volatile (
           "    nop	\n" 
                );
              };
           //

           // send LOW VOLTAGE - this time square wave
           OUTPUTPORT = 0; 
           // now delay to achieve desired frequency
           for(i=0; i<pulsewidth; i++)
             {
              asm volatile (
           "    nop	\n" 
                );
              };
           //
           };

      
        }; // neverending loop 

};  // end of MAIN



