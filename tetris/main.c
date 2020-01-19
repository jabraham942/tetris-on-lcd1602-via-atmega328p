//-----------------------------------------------------------------------------
// tetris-on-lcd1602-via-atmega328p
//
// Github Link: https://github.com/jabraham942/tetris-on-lcd1602-via-atmega328p
//
// Author: Josh Abraham
//
// Target MCU: ATmega328P
// ---------------------------------------------------------------------------



#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
#include <stdio.h>

#include "LCD1602.h" //Contains functions to communicate with LCD1602 display
#include "Tetris.h"  //Contains functions which controls Tetris data structures and logic


//---------------------------------------
// Function: Tetris
//
// Description: Create empty tetris_state and begin playing tetris by loading random tetromino blocks till top of display is reached
//
// Input: None
//
// Output: None
//---------------------------------------
void Tetris() {
	srand(time(NULL));
	uint8_t tetris_state[6][19] = 
	{{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,     0x00, 0x00,0x00,  },
	 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,     0x00, 0x00,0x00,  },
	 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,     0x00, 0x00,0x00,  },
	 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,     0x00, 0x00,0x00   },
	
	 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,     0x00, 0x00,0x00,  },
	 {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00,0x00,     0x00, 0x00,0x00   }};
	
	while(1) {
		
		load_random_tetromino(19,tetris_state);

		if ( (tetris_state[0][15] != 0x00) |
		(tetris_state[1][15] != 0x00) |
		(tetris_state[2][15] != 0x00) |
		(tetris_state[3][15] != 0x00) ) {
			
			return;
		}

		
	}
}







int main(void)
{
 setup_AVR_ports(); // Setup Port B and C in Atmega328p
 setup_ADC(); //Setup ADC with initial settings
 LCD_init(); // initialize LCD controller
 create_tetris_characters(); //Add 4 custom characters to CGRAM to be used for displaying Tetromino blocks
 _delay_ms(500); // wait

 while(1){
	Tetris();		
 }
}