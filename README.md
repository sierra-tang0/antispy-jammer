# antispy-jammer
Simple ultrasonic antispy voice recording jammer based on ATTINY13 / ATTINY85 with amplifier PAM8403 module driving piezo ultrasonic transducers. It prevents from unauthorized human speech recording by hidden microphones and voice assistants.

The project is based on following concept presented here : https://sandlab.cs.uchicago.edu/jammer/  , 
here https://github.com/y-x-c/wearable-microphone-jamming  
and here  http://people.cs.uchicago.edu/~ravenben/publications/pdf/ultra-chi20.pdf , however my design is cost optimized and very simplified in comparison to the one above.

In my design I do not use programmable signal generator ( AD9833 ) here so the jamming capability may not be such good as original design (however if you want to play with original design relevant schematic and source code is also available down below). 
Instead of AD8933 I am using the simplest microcontroller ATMEGA ATTINY13A ( or ATTINY85 ) to create audio wave from predefined lookup table or using random value based  PWM techniques to modulate base 25kHZ signal (depending on source file). 

History:
At the beginning of this project ("main.c" and "main2.c" source files)  there was a set of resistors used to build 5-bit Digital to Analogue converter ( R-2R resistor ladder DAC : check here https://www.electronics-tutorials.ws/combination/r-2r-dac.html ) to create sinusoidal audio wave and audio amplification stage with 2 bipolar transistors (NPN+PNP bridge - class B amplifier) for driving piezzoelectric ultrasonic transducers : https://www.electronics-tutorials.ws/amplifier/amp_6.html (they introduce cross-over distortion  and have very small power). The Sinusoidal waveform parameters have been calculated using Libreoffice Calc / Microsoft Excell and can be changed to any other waveform if necessary.The C code was utilizing whole available PINs in ATTINY PORTB (PB0-PB4) to create DAC for sine wave or pulse wave depending on source code version.  I have uploaded this code here only for reference, maybe someone would like to play with it.

However after testing first prototype  it turned out that the jamming power is too low because center frequency 25kHz has to be FM modulated in random manner. Finally I had to use PAM8403 amplifier module ( see diagram with "enhanced" version) for 6 Watt output power and modify lookup table and code to construct square audio wave with pseudo white-noise bias that modulates center frequency of ultrasonic transducers. 
In this version the 25kHz frequency is randomly shifted in 0,4 kHz offsets within 23-27 kHz range and that gives awesome results in jamming (up to 4-5 meters of jamming). 

Files "main4.c"/"main5.c"  are using Digital to Analog Converter for FM 25kHz signal modulation only , while files "main7.c"/"main8.c" are using random PWM and amplitude modulation to generate the noise (in test main7.c/main8.c work better than main4.c/main5.c files). 
If using files with AM modulation please tune signal gain using potentiometer not to get it distorted.

The device can be powered from 2,7V - 5,5 V power source ( it can operate even directly from LiPol 3,7V battery, but remember the higher the voltage - the more output power you get ). The microcontroller ATTINY13 has its fuses set to operate with 9,6MHz internal clock while ATTINY85 has fuses set to operate on 16MHz clock. 

UPDATE 20.02.2022 
I have managed to re-create original design with only : ATTINY85 chip + AD9833 signal generator + PAM8403 audio amplifier and set of transducers.
The code "main6.c" and the script "compileattiny6" is prepared for this purpose, the diagram will be "arduino-mic-supresor-ultrasonic-v2-ATTINY85.png". 
Also relevant INO scripts and schematic of this re-created design are available for any other Arduino supporting SPI serial bus connectivity. For ARDUINO version please use diagrams "arduino-mic-supresor-ultrasonic-v2.png" and "arduino-mic-supresor-ultrasonic-v2-pro-mini.png"

UPDATE 27.01.2023 
The difference to original programming is that together with random FREQUENCY shifting I am also using random PHASE shifting which gives better effectivenes for high-end phones despite the signal is a little bit audible..  This is only valid for versions which use  AD9833 signal generator.

UPDATE 12.04.2023 
The code for Digispark/ATTINY85 + AD9833 + TPA3116/PAM8403 was changed to support INFRASOUND 5-25Hz random FM modulation over 25KHz ULTRASOUND carrier. This gives the best result for jamming iPhones and on the high-end phones and the signal is barely audible by human ear when demodulated on obstacles. However such approach cannot be introduced into the design which does not use AD9833 due to speed limitation of the ATTINY85 chip. If you want to jam iPhones I would suggest to use design : Digispark + AD9833 + TPA3116 + at least 12 transducers


IF YOU WANT TO INCREASE JAMMING CAPABILITY USE BETTER AUDIO AMPLIFIER LIKE TPA3116 WHICH GIVES 50WATT OF AUDIO POWER AND MORE TRANSDUCERS LIKE 50 PER AUDIO CHANNEL ! 
---

Available versions :

- AVR-GCC versions for direct chip upload - ATTINY13 and ATTINY85 chips :
In ATTINY85 version the internal clock has to be reconfigured to PLL clock, no DIV8 to 16MHz frequency ( AVRDUDE fuses : -U lfuse:w:0xf1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m  ) like for Digisparkdevice.  Also number of NOP commands is finetuned in the source code to fit exactly ~25kHZ frequency. USBASP and AVRDUDE have to be used for ATTINY chip programming. This is not Arduino code, however some bigger chips like ATMEGA 328P ( the one from ARDUINO ) also can be used.

Please use schematic "antispy-jammer-enhanced-schematic.png" and following combinations of source code and compilation script :

for ATTINY85 : main4.c   +  compileattiny85v2 , main7.c   +  compileattiny85v3 

for ATTINY13 : main5.c   +  compileattinyv2 , main8.c  +   compileattinyv3

If you want to play with original design using AD9833 signal generator, please use combination of:
main6.c   +  compileattiny6  and schematic "arduino-mic-supresor-ultrasonic-v2-ATTINY85.png"
 
 
ARDUINO VERSIONS :

- ARDUINO DIGISPARK version - there is separated "mic-jammer.ino" version which is composed of ARDUINO DIGISPARK (ATTINY85) connected to PAM8403 MODULE and 20 transducers. It also gives same high range of jamming capability.  For DIGISPARK version please use schematic "arduino-mic-supresor-ultrasonic.png" and Arduino script "mic-jammer.ino".  If you are having doubts how to connect and program Digispark board please follow this tutorial : http://digistump.com/wiki/digispark/tutorials/connecting
 
 - Re-created original version of Digispark with AD9833 signal generator - please use schematic "arduino-mic-supresor-ultrasonic-v2.png" and INO  script "mic-jammer-ad9833-digispark.ino" 
- Re-created original version of Arduino Nano/Mini/Pro with AD9833 signal generator - please use schematic "arduino-mic-supresor-ultrasonic-v2-pro-mini.png" and INO script "mic-jammer-ad9833.ino"

-------------------------------------------------------------------------------------

Component list :

- 1 x Microcontroller ATTINY13 / ATTINY13A - or ATTINY 85, DIGISPARK board or any other Arduino may be used instead of microcontroller
- 1 x 47 microfarad electrolytic capacitor ( it can be 1 - 100uF - for blocking distortions on power lines)
- 1 x 100 nanofarad capacitor (it can be 47nf - 470nF - for blocking distortions on power lines)
- set of 25kHZ (resonance frequency is important !)  ultrasonic piezo transducers like NU25C16T-1, 25kHz or equivalent
- some power source 3 - 5,5 Volt (it may be LiPol battery or set of 3xAA batteries)
- 1 x PAM4803 : 2 x 3Watt Amplifier module ( instead of 2 bipolar transistors) 
- 1 x 10K Ohm potentiometer ( or resistor divider ) may be put between ATTINY85/ARDUINO/AD9833 audio output pins and PAM8403 audio input pins 
- AD9833 signal generator board ( if you want to use original design)


-------------------------------------------------------------------------------------

AVR-GCC compilation and chip flashing :

"mainXX.c" source code is prepared in AVR-GCC and has to be uploaded with AVRDUDE to the ATTINY85/ATTINY13 chip.

If you have Linux based machine you would need to setup the environment by installing following packages : "gcc-avr", "binutils-avr" (or sometimes just "binutils"), "avr-libc", "avrdude". For Ubuntu / Linux Mint you may use following command : "sudo apt-get install gcc-avr binutils-avr binutils avr-libc gdb-avr avrdude"

If you have Windows machine, please follow this tutorial to install necessary packages : http://fab.cba.mit.edu/classes/863.16/doc/projects/ftsmin/windows_avr.html

If you do not know how to connect cables for ATMEL ATTINY chip programming please follow my tutorial here : https://www.youtube.com/watch?v=7klgyNzZ2TI


Compilation

- files "compileattiny" and "main.c" for ATTINY13/ATTINY13A chip  (internal 9,6 MHz clock )   - initial version please do not use it anymore !
- files "compileattiny85" and "main2.c" for ATTINY85 chip  (internal 16MHz PLL clock)  - initial version please do not use it anymore !

My design : 
- use "compileattinyv2" and "main5.c" files for ATTINY13/ATTINY13A chip - FM modulation only (internal 9,6 MHz clock )
- use "compileattinyv3" and "main8.c" files for ATTINY13/ATTINY13A chip - AM/PWM signal modulation (internal 9,6 MHz clock )
- use "compileattiny85v2" and "main4.c" files for ATTINY85 chip - FM modulation only  (internal 16MHz PLL clock)
- use "compileattiny85v3" and "main7.c" files for ATTINY85 chip  - AM/FM signal modulation (internal 16MHz PLL clock)
 

Original re-created design + enhancements for better jamming effectivenes  :
- use "compileattiny6" and "main6.c" files for ATTINY85 chip  (internal 16MHz PLL clock) + AD9833 signal generator

To compile the code for ATTINY use relevant "compileattinyXXX" script ( example : under linux "chmod +rx compileattiny && ./compileattiny ") for AVR-GCC environment compilation and flashing with AVRDUDE and USBASP cable.  

See the video showing how this device works :
- Old version : https://www.youtube.com/watch?v=YBQ7A4W0bTo   ( please do not use it , low jamming capability )
- New version for Arduino (and main4.c / main5.c for ATTINY) : https://youtu.be/JFtU2hQQ2vQ
- Re-created original version for Digispark : https://youtu.be/PcTkMWJb_Gs
- The newest version microphone jamming beast with TPA3116D2 audio amplifier board - jamms even iPhones : https://youtu.be/8RgoYlrgW4s



