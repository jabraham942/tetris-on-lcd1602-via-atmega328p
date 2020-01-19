#ifndef _LCD1602_H_
#define _LCD1602_H_

//LCD PORT to Atmega328p Port mapping
#define RS 0 // R/S Pin for LCD
#define ENABLE 1 // Enable Pin for LCD
#define D4 2 // Data Pin 4 for LCD
#define D5 3 // Data Pin 5 for LCD
#define D6 4 // Data Pin 6 for LCD
#define D7 5 // Data Pin 7 for LCD


//LCD Commands
#define CURSOR_DISABLED 0x0C
#define SHIFT_RIGHT 0x06
#define CONTROLLER_INIT 0x33
#define CLEAR_DISPLAY 0x01
#define FOUR_BIT_INPUT 0x32
#define CURSOR_SET 0x80
#define FIVExEIGHT_CHAR_SIZE 0x28



#define enable_bit(x,y)   x |=  (1 << y)
#define disable_bit(x,y) x &= ~(1 << y)




//---------------------------------------
// Function: setup_AVR_ports
//
// Description: Initialize Ports on Atmega328p
//
// Input: None
// Output: None
//
//---------------------------------------
void setup_AVR_ports()
{
	
	PORTC |= ((1 << PORTC0) | (1 << PORTC1)); //Turn on Pull-up resistor for PC0 and PC1
	DDRC = 0x00; // Configure Ports C0 and C1 as input ports
	 
	DDRB = 0x3F; // Configure Ports B5 - B0 as output ports

 
 
}





//---------------------------------------
// Function: setup_ADC
//
// Description: Initialize ADC on Atmega328p
//
// Input: None
// Output: None
//
//---------------------------------------
void setup_ADC()
{

	DIDR0 = 0x03; // Disable Digital input for C0 and C1
		
	ADCSRA |=(1<ADPS2)|(1<ADPS1) |(1<<ADPS0) | (1<<ADEN); //Enable ADC and use prescaler = 128
	ADMUX |=(1<<REFS0);//Using AVcc as Reference Voltage
	
}





//---------------------------------------
// Function: pulse_enable_pin
// 
// Description: Pulse the Enable pin for 50 us
//
// Input: None
// Output: None
//
//---------------------------------------
void pulse_enable_pin()
{
	enable_bit(PORTB,ENABLE); 
	 
	_delay_us(50); 
	 
	disable_bit(PORTB,ENABLE); 
}





//---------------------------------------
// Function: send_half_byte
//
// Description: Set Top 4 Data bits and Pulse Enable Pin
//
// Input: uint8_t 
// Output: None
//
//---------------------------------------
void send_half_byte(uint8_t input_byte)
{
	PORTB &= 0xC3; // Set B5 - B2 to zero
	 
	if ((1 << 7) & input_byte) {
		enable_bit(PORTB,D7);
	}
	 
	if ((1 << 6) & input_byte) {
		enable_bit(PORTB,D6);
	}
	 
	if ((1 << 5) & input_byte) {
		enable_bit(PORTB,D5);
	}
	 
	if ((1 << 4) & input_byte) {
		enable_bit(PORTB,D4);
	}
	 
	pulse_enable_pin(); 
}




//---------------------------------------
// Function: send_full_byte
//
// Description: Send byte in four bit segments
//
// Input: uint8_t
// Output: None
//
//---------------------------------------
void send_full_byte (uint8_t input_byte)
{
	send_half_byte(input_byte); 
	send_half_byte(input_byte<<4); 

}





//---------------------------------------
// Function: LCD_command
// 
// Description: Send Command to LCD Controller
//
// Input: uint8_t
// Output: None
//
//---------------------------------------
void LCD_command (uint8_t cmd)
{
	disable_bit(PORTB,RS); // Set Command Mode
	send_full_byte(cmd); // Send Command to LCD
}








//---------------------------------------
// Function: LCD_data
//
// Description: Send Byte to LCD Controller to be interpreted as ASCII Character
//
// Input: uint8_t
// Output: None
//
//---------------------------------------
void LCD_data (uint8_t input_byte)
{
	enable_bit(PORTB,RS); // Set Data Mode
	send_full_byte(input_byte); // Send Data to LCD
}







//---------------------------------------
// Function: LCD_init
// 
// Description: Initialize LCD Controller
//
// Input: None
// Output: None
//
//---------------------------------------
void LCD_init()
{
	
	LCD_command(CONTROLLER_INIT); // Controller Init
	LCD_command(CURSOR_DISABLED); // Cursor Disabled
	LCD_command(SHIFT_RIGHT); // Shift Right
	LCD_command(FOUR_BIT_INPUT); // Input Mode = 4-bit
	LCD_command(FIVExEIGHT_CHAR_SIZE); // 5x8 character size, 2 line display
	LCD_command(CLEAR_DISPLAY); // Clear Display
	 
	_delay_ms(5);
 

}





//---------------------------------------
// Function: LCD_clear
//
// Description: Clear LCD Display
//
// Input: None
// Output: None
//
//---------------------------------------
void LCD_clear() 
{
	LCD_command(CLEAR_DISPLAY); //LCD Clear Command
	 
	_delay_ms(5);
}







//---------------------------------------
// Function: LCD_set_cursor
//
// Description: Set Cursor at x character on y line
//
// Input: uint8_t x, 
//        uint8_t y
// Output: None
//
//---------------------------------------
void LCD_set_cursor(uint8_t x, uint8_t y) 
{
	uint8_t address = 0 + x + CURSOR_SET; // Line 0 --> 0 offset
	if (y == 1) {
		address += 0x40;  // Line 1 --> 0x40 offset
	}
	LCD_command(address); // update cursor with x,y position
}






//---------------------------------------
// Function: LCD_save_custom_character
//
// Description: Save 8 bytes from custchar array at cgramaddress location to be used as custom character for LCD display
//
// Input: uint8_t cgramaddress, 
//        uint8_t custchar[]
// Output: None
//
//---------------------------------------
void LCD_save_custom_character(uint8_t cgramaddress, uint8_t custchar[]) {
	cgramaddress &= 0x7F;
	cgramaddress |= 0x40;
	LCD_command(cgramaddress);
	_delay_ms(60);
	for(int i = 0; i < 8; i++) {
		
		LCD_data(custchar[i]);
		
	}
	
}



#endif // _LCD1602_H_