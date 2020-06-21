**WARNING**
Use of this code or anything here is given without warranty for 
any purpose.  USE AT YOUR OWN RISK.


Update 6/21/2020: added adsget.c and modded Makefile to adapt.

06/21/2020: run "make clean simple" to produce ads_get
ads_get takes numbers on command line for address, ai line, and gain.
Will produce one read of ai line.  This is prep for use in PHP.

Short cut:
If you fork this repo to your own Raspberry Pi, you should 
be able to run "make clean default" in order to produce 
the current version of ads_read.  

Run this with "sudo ./ads_read" on Linux and you ought to get a 
stream from STD_IN on your terminal with the voltage of your
chosen AI line on the ADS1115.

Long story:
A C based package (more or less) to read analog values from
an ADS1115 A/D board.

I give original credit to Lewis Loflin at bristolwatch.com
as he was the one that deciphered the cut sheet for the 
ADS1115 register instructions.

I have altered the original C code to function-ize the
basic register calling and turn it into a returned voltage.

I also put the function into its own file so that one can
write code using the function without having to keep track
of the function itself.

The function definition is given below:

int16_t ads_read( int address, int ain, int gain );

The function depends upon a header file where I define
the needed binary sorted numbers that make up the 
configuration register.

The ADS1115 can be set up easily to be one of four I2C bus
addresses.  

The variable 'address' is set to be one of the following for
predefined values:
HEX48
HEX49
HEX4A
HEX4B

The function makes use of a bit setting for either 1-shot
read and shutdown operation or continuous read.  Loewis didn't
go into continuous read much and the 1-shot setting works well,
so I've set it permanently in the function.  

Configuration Register MSB values 
Define 1-shot read or continuous
ONESHOT_ON
ONESHOT_OFF

There are four analog inputs on the ADS1115 board.  The following
predefined values tell which one to read from.
AI0
AI1
AI2
AI3

There is also a gain setting for the AI and the following
predefined settings set that on a read cycle.  GAIN2 works
for most 3.3V and 5V inputs.

GAIN1 through GAIN8


Making a program that makes it easier to read and display (or return) 
a value and the ability of addressing multiple boards, 
means that one could have an I2C enabled device that could
easily read up to 16 0-3.3VDC or 0-5VDC sensors.

Things to do:
1. Currently working on a main program that reads function 
values from the command line and returns the value.
2. Plan to write program such that it can be used with PHP
to display an analog input value in a web page.
3. Not a programming task, but experiment with a way to 
use opto-isolators in the SDA/SCL lines so that the ADS1115
can be used at 5VDC but still operate on the Raspberry PI 3.3VDC
enabled I2C bus.
