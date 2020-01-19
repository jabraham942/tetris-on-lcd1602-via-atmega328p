//4 custom characters to load to LCD CGRAM
static uint8_t noFilled_char[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t topFilled_char[] = {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
static uint8_t bottomFilled_char[] = {0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};
static uint8_t allFilled_char[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,};
static int TETRIS_TICK_LENGTH = 500; //value in miliseconds


//Struct to hold tetromino location data
typedef struct tetromino_location {
	
	
		int orientation; //Current orientation of tetromino block (1-4)
		int center_x, center_y; // Center block x and y coordinate
		
		int c_b1_x[4], c_b1_y[4]; //Difference between center block and block 1 as a function of orientation
		int c_b2_x[4], c_b2_y[4]; //Difference between center block and block 2 as a function of orientation
		int c_b3_x[4], c_b3_y[4]; //Difference between center block and block 3 as a function of orientation
		
		int block1_x, block1_y; // Block 1 x and y coordinate
		int block2_x, block2_y; // Block 2 x and y coordinate
		int block3_x, block3_y; // Block 3 x and y coordinate
	
	} tetromino_location;
	
	
	
	
	
//---------------------------------------
// Function: create_tetris_characters
//
// Description: Save 4 custom characters in LCD Controller 
//	1. No bytes set
//  2. Top 4 bytes set
//  3. Bottom 4 bytes set
//  4. All 8 bytes set
//
// Input: None
// Output: None
//
//---------------------------------------
void create_tetris_characters() {
	LCD_save_custom_character(0x00, noFilled_char);
	LCD_save_custom_character(0x08, topFilled_char);
	LCD_save_custom_character(0x10, bottomFilled_char);
	LCD_save_custom_character(0x18, allFilled_char);

	
}








//---------------------------------------
// Function: valid_tetromino_location
//
// Description: Validate if the 4 blocks in tetromino is in valid location on LCD 1602 display
//
// Input: struct tetromino_location *t_loc_p, 
//        int columns
//        uint8_t tetris_state[][columns]
//
// Output: int
//		  -4 = At least one block location has a block already placed in tetris_state
//        -3 = At least one y coordinate is negative
//        -2 = At least one X coordinate is negative
//		  -1 = At least one X coordinate is greater than 3
//         0 = All 4 blocks in Tetromino are in a valid location
//
//---------------------------------------
int valid_tetromino_location(struct tetromino_location *t_loc_p, int columns,  uint8_t tetris_state[][columns]) {

	if((t_loc_p->center_x < 0) | (t_loc_p->block1_x < 0) | (t_loc_p->block2_x < 0) | (t_loc_p->block3_x < 0)) {
		return -1;
	}
	
	else if((t_loc_p->center_x > 3) | (t_loc_p->block1_x > 3) | (t_loc_p->block2_x > 3) | (t_loc_p->block3_x > 3)) {
		//printf("%d %d %d %d\n", t_loc_p->center_x, t_loc_p->block1_x, t_loc_p->block2_x, t_loc_p->block3_x);
		return -2;
	}
	
    else if((t_loc_p->center_y < 0) | (t_loc_p->block1_y < 0) | (t_loc_p->block2_x < 0) | (t_loc_p->block3_y < 0)) {
    	return -3;	
    }
	else if((tetris_state[t_loc_p->center_x][t_loc_p->center_y] != 0x00) |(tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] != 0x00) | (tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] != 0x00) | (tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] != 0x00)) {
	
		return -4;		
	}
	else {
		return 0;
	}
	
	
	
	
	
}

//---------------------------------------
// Function: update_tetromino_location_struct
//
// Description: Update Block 1-3's X,Y Coordinate based on Center block's X,Y Coordinate in tetromino_location struct pointer
//
// Input: struct tetromino_location *t_loc_p,
//
// Output: int
//		  -1 = At least one of tetromino's new block locations is outside bounds of display 
//         0 = All 4 blocks' new location in Tetromino are valid and have been updated
//
//---------------------------------------
int update_tetromino_location_struct(struct tetromino_location *t_loc_p) {
	if (((t_loc_p->center_x     +    t_loc_p->c_b1_x[t_loc_p->orientation]) > 3 ) |  ((t_loc_p->center_x     +    t_loc_p->c_b1_x[t_loc_p->orientation]) < 0)
	|  ((t_loc_p->center_x     +    t_loc_p->c_b2_x[t_loc_p->orientation]) > 3 ) |  ((t_loc_p->center_x     +    t_loc_p->c_b2_x[t_loc_p->orientation]) < 0)
	|  ((t_loc_p->center_x     +    t_loc_p->c_b3_x[t_loc_p->orientation]) > 3 ) |  ((t_loc_p->center_x     +    t_loc_p->c_b3_x[t_loc_p->orientation]) < 0)
	|  ((t_loc_p->center_y     +    t_loc_p->c_b1_y[t_loc_p->orientation]) > 18 ) |  ((t_loc_p->center_y     +    t_loc_p->c_b1_y[t_loc_p->orientation]) < 0)
	|  ((t_loc_p->center_y     +    t_loc_p->c_b2_y[t_loc_p->orientation]) > 18 ) |  ((t_loc_p->center_y     +    t_loc_p->c_b2_y[t_loc_p->orientation]) < 0)	
	|  ((t_loc_p->center_y     +    t_loc_p->c_b3_y[t_loc_p->orientation]) > 18 ) |  ((t_loc_p->center_y     +    t_loc_p->c_b3_y[t_loc_p->orientation]) < 0)) {
		return -1;
	}
	
	t_loc_p->block1_x    =    t_loc_p->center_x     +    t_loc_p->c_b1_x[t_loc_p->orientation];
	t_loc_p->block1_y    =    t_loc_p->center_y     +    t_loc_p->c_b1_y[t_loc_p->orientation];
	
	t_loc_p->block2_x    =    t_loc_p->center_x     +    t_loc_p->c_b2_x[t_loc_p->orientation];
	t_loc_p->block2_y    =    t_loc_p->center_y     +    t_loc_p->c_b2_y[t_loc_p->orientation];
	
	t_loc_p->block3_x    =    t_loc_p->center_x     +    t_loc_p->c_b3_x[t_loc_p->orientation];
	t_loc_p->block3_y    =    t_loc_p->center_y     +    t_loc_p->c_b3_y[t_loc_p->orientation];

	
	return 0;
}


//---------------------------------------
// Function: rotate_tetromino
//
// Description: Increment orientation and update the 4 tetromino blocks' location based on new orientation
//
// Input: struct tetromino_location *t_loc_p,
//		  int columns
//        uint8_t tetris_state[][columns]
//
// Output: int
//		  -3 = At least one Tetromino block is out of bounds after orientation increment
//		  -2 = At least one Tetromino block is out of bounds or colliding with existing block after orientation increment
//		  -1 = Tetromino has reached bottom edge of display already
//         0 = All 4 blocks' new location in Tetromino are valid and have been updated
//         TODO: Merge -3 and -2 EC conditions
//
//---------------------------------------
int rotate_tetromino(struct tetromino_location *t_loc_p, int columns,  uint8_t tetris_state[][columns]) {
	
		if ((t_loc_p->center_y == 0) | (t_loc_p->block1_y == 0) | (t_loc_p->block2_y == 0) | (t_loc_p->block3_y == 0)) {
			
			return -1;
		}
	
		tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x00;
		tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x00;
		tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x00;
		tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x00;
		
		t_loc_p->orientation = (t_loc_p->orientation + 3) % 4;

		int rc = update_tetromino_location_struct(t_loc_p);
		
		if (rc == -1) {

			t_loc_p->orientation = (t_loc_p->orientation + 1) % 4;
			
			tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x03;
			tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x03;
			tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x03;
			tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x03;
						
			return -3;
		}
		
		
		
		
		
		
		
		rc = valid_tetromino_location(t_loc_p, columns, tetris_state);
		
		if (rc == 0) {

			tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x03;
			tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x03;
			tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x03;
			tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x03;
		}
		else {

			t_loc_p->orientation = (t_loc_p->orientation + 1) % 4;
			update_tetromino_location_struct(t_loc_p);
			tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x03;
			tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x03;
			tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x03;
			tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x03;
			
			return -2;		
			
		}
	
	return 0;
}



//---------------------------------------
// Function: move_tetromino
//
// Description: Move the 4 tetromino blocks' location based on input direction
//
// Input: struct tetromino_location *t_loc_p,
//		  int columns
//        uint8_t tetris_state[][columns],
//		  int direction
//
// Output: int
//		  -3 = At least one Tetromino block is out of bounds after move
//		  -2 = At least one Tetromino block is out of bounds or colliding with existing block after move
//		  -1 = Tetromino has reached bottom edge of display already
//         0 = All 4 blocks' new location in Tetromino are valid and have been updated
//         TODO: Merge -3 and -2 EC conditions
//
//---------------------------------------
int move_tetromino(struct tetromino_location *t_loc_p, int columns,  uint8_t tetris_state[][columns], int direction) {
	//direction:
	// 0 == right(Increase x)
	// 1 == left (Decrease x)
	// 2 == down (Decrease y)

	if ((t_loc_p->center_y == 0) | (t_loc_p->block1_y == 0) | (t_loc_p->block2_y == 0) | (t_loc_p->block3_y == 0)) {
		
		return -1;
	}
	
	direction = direction % 3;
	
	tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x00;
	tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x00;
	tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x00;
	tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x00;
	
	if (direction == 0) {
		t_loc_p->center_x += 1;
	}
	else if(direction == 1) {
		t_loc_p->center_x -= 1;
		
	}
	else if(direction == 2) {
		
		t_loc_p->center_y -= 1;
	}

	int rc = update_tetromino_location_struct(t_loc_p);
	
	if (rc == -1) {
				if (direction == 0) {
					t_loc_p->center_x -= 1;
				}
				else if(direction == 1) {
					t_loc_p->center_x += 1;
					
				}
				else if(direction == 2) {
					
					t_loc_p->center_y += 1;
				}

				tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x03;
				tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x03;
				tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x03;
				tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x03;
				
				return -3;
				
	}
	
	
	
	
	
	
	
	
	rc = valid_tetromino_location(t_loc_p, columns, tetris_state);
	
	if (rc == 0) {
		tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x03;
		tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x03;
		tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x03;
		tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x03;
	}
	else {
		if (direction == 0) {
			t_loc_p->center_x -= 1;
		}
		else if(direction == 1) {
			t_loc_p->center_x += 1;
			
		}
		else if(direction == 2) {
			
			t_loc_p->center_y += 1;
		}

		update_tetromino_location_struct(t_loc_p);
		tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x03;
		tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x03;
		tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x03;
		tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x03;
		
		return -2;
		
	}
	
	return 0;
}










//---------------------------------------
// Function: joystick_update
//
// Description: Move the 4 tetromino blocks' location based Joystick state
//
// Input: struct tetromino_location *t_loc_p,
//		  int columns
//        uint8_t tetris_state[][columns],
//
// Output: None
//
//---------------------------------------
void joystick_update(struct tetromino_location *t_loc_p, int columns,  uint8_t tetris_state[][columns]) {
	
	
	int X_Val =0;
	int Y_Val =0;
	

	ADMUX = 0X41; // Set to Port C1
	ADCSRA |= (1<<ADSC);//Trigger conversion in ADC
	while ( !((1<<ADIF) & ADCSRA));//Spin until conversion in ADC is done
	X_Val = ADC;// Grab X value from ADC
	ADC=0;//Clear ADC registers
	ADMUX=0x40;//Set to Port C2
	

	ADCSRA |= (1<<ADSC);// Trigger conversion in ADC
	while ( !((1<<ADIF) & ADCSRA));// Spin until conversion in ADC is done
	Y_Val = ADC; //Grab Y value from ADC
	ADC=0;//Clear ADC registers


		// 0 == right(Increase x)
		// 1 == left (Decrease x)
		// 2 == down (Decrease y)
	if (X_Val<250) // Go Left

	{
		move_tetromino(t_loc_p, 19, tetris_state, 1);
	}

	if (X_Val>750) // Go Right

	{
		move_tetromino(t_loc_p, 19, tetris_state, 0);
	}

	if (Y_Val<250) // Go Down

	{
		move_tetromino(t_loc_p, 19, tetris_state, 2);
	}

	if (Y_Val>750) // Rotate

	{
		rotate_tetromino(t_loc_p, 19, tetris_state);
	}
	
}








//---------------------------------------
// Function: update_tetris_state
//
// Description: Update uint8_t 2-D array (tetris_state) based on tetromino_location struct pointer, t_loc_p
//
// Input: struct tetromino_location *t_loc_p,
//		  int columns
//        uint8_t tetris_state[][columns],
//
// Output: int
//		  -2 = Tetromino block has already reached bottom of display prior to function call
//		  -1 = At least one Tetromino block is in invalid location after move of all 4 blocks down Y-Axis by 1
//         0 = All 4 blocks' new location in Tetromino are valid and have been updated after move down Y-Axis by 1
//---------------------------------------
int update_tetris_state(struct tetromino_location *t_loc_p, int columns,  uint8_t tetris_state[][columns]) {
	
	
	if(( t_loc_p->center_y > 0) && (t_loc_p->block1_y > 0) && (t_loc_p->block2_y > 0) && (t_loc_p->block3_y) > 0) {
		 tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x00;
		 t_loc_p->center_y -= 1;
	}
	else {
			return -2;
	}
	
	
	
	tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x00;
	tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x00;
	tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x00;

	
	update_tetromino_location_struct(t_loc_p);
	int rc = valid_tetromino_location(t_loc_p, columns, tetris_state);
	if ( rc >= 0) {
	
		tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x03;
		tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x03;
		tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x03;
		tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x03;
	
	}
	
	else {
		
		t_loc_p->center_y += 1;
		update_tetromino_location_struct(t_loc_p);
		tetris_state[t_loc_p->center_x][t_loc_p->center_y] = 0x03;
	
		tetris_state[t_loc_p->block1_x][t_loc_p->block1_y] = 0x03;
	
		tetris_state[t_loc_p->block2_x][t_loc_p->block2_y] = 0x03;
	
		tetris_state[t_loc_p->block3_x][t_loc_p->block3_y] = 0x03;
		
		return -1;	
	}
	
	return 0;
	
}




//---------------------------------------
// Function: update_2_row_tetris_state
//
// Description: Read Rows 0-3 in tetris_state and write equivalent info in Rows 4-5 to match LCD1602 (2 row display)
//
// Input: int columns
//        uint8_t tetris_state[][columns],
//
// Output: None
//---------------------------------------
void update_2_row_tetris_state(int columns, uint8_t tetris_state[][columns]) { 
	
		for(int i= 0; i<2; i++) {
			
			for(int j = 0; j<19; j++) {
				
				if (tetris_state[2*i][j] == 0x00 && tetris_state[(2*i) + 1][j] == 0x00) {
					
					tetris_state[i+4][j] = 0x00;
					
				}
				else if (tetris_state[2*i][j] == 0x03 && tetris_state[(2*i) + 1][j] == 0x00) {
					
					tetris_state[i+4][j] = 0x01;
					
				}
				else if (tetris_state[2*i][j] == 0x00 && tetris_state[(2*i) + 1][j] == 0x03) {
					
					tetris_state[i+4][j] = 0x02;
					
				}
				else {
									
					tetris_state[i+4][j] = 0x03;
									
				}
				
			}
			
		}
	
}




//---------------------------------------
// Function: print_tetris_state_to_lcd
//
// Description: Print rows 4,5 in tetris_state to LCD 1602 display
//
// Input: int columns
//        uint8_t tetris_state[][columns],
//
// Output: None
//---------------------------------------
void print_tetris_state_to_lcd(int columns, uint8_t tetris_state[][columns]) {
	
	update_2_row_tetris_state(columns, tetris_state);
	
	
	for(int i =0; i<2; i++) {
		LCD_set_cursor(0,i);
		for(int j = 0; j<19; j++) {
				LCD_data(tetris_state[i+4][j]);
		}


	}
	
	

	
}


//---------------------------------------
// Function: remove_complete_rows
//
// Description: Remove completed horizontal rows from tetris_state and update accordingly
//
// Input: int columns,
//        uint8_t tetris_state[][columns]
//
// Output: None
//---------------------------------------
void remove_complete_rows(int columns, uint8_t tetris_state[][columns]) {
	
	int shift = 0;
	
	for(int i=0;i<17;i++) {
		
		if ((tetris_state[0][i] == 0x03) && (tetris_state[1][i] == 0x03) && (tetris_state[2][i] == 0x03) && (tetris_state[3][i] == 0x03) ) {
			
			shift++;
			
		}
		else {
			
			tetris_state[0][i - shift] = tetris_state[0][i];
			tetris_state[1][i - shift] = tetris_state[1][i];
			tetris_state[2][i - shift] = tetris_state[2][i];
			tetris_state[3][i - shift] = tetris_state[3][i];
		}
		
	}
	
}






//---------------------------------------
// Function: load_tetromino
//
// Description: Drop new tetromino block into display from Y=15 and descend till it lands on an existing block or the bottom of display
//
// Input: int columns,
//        uint8_t tetris_state[][columns],
//        struct tetromino_location *t_loc_p
//
// Output: None
//---------------------------------------
void load_tetromino(int columns, uint8_t tetris_state[][columns], struct tetromino_location *t_loc_p) {
	update_tetromino_location_struct(t_loc_p);
	
	for (int i=30;i>0;i--) {
		

		joystick_update(t_loc_p, 19, tetris_state);

		int rc = update_tetris_state(t_loc_p, 19, tetris_state);

		if (rc == 0) {

			print_tetris_state_to_lcd(19,tetris_state);
		}
		else{
			remove_complete_rows(19, tetris_state);
			print_tetris_state_to_lcd(19,tetris_state);
			return;
		}
		_delay_ms(TETRIS_TICK_LENGTH);

		
	}
	
}






//---------------------------------------
// Function: load_J_tetromino
//
// Description: Create struct tetromino_location pointer with specific data for J Tetromino and call load_tetromino function
//
// Input: int columns,
//        uint8_t tetris_state[][columns]
//
// Output: None
//---------------------------------------
void load_J_tetromino(int columns, uint8_t tetris_state[][columns]) {
	// Orientation = 0
	// [B1][B2]
	// [C]
	// [B3]
	
	// Orientation 2-4, add 90, 180 and 270 degree rotation counter clockwise respectively around center block
	
	// For B1-B3, these 3 arrays are the x/y difference between itself and the center block, based on the orientation 1-4
	
	
	struct tetromino_location t_loc = {
		rand() % 4, //orientation
		2, //entry_row
		16,			//entry_column
		
		{0, -1, 0, 1}, //c_b1_x
		{1, 0, -1, 0}, //c_b1_y
			
		{1, -1, -1, 1}, //c_b2_x
		{1, 1, -1, -1}, //c_b2_y
		
		{0, 1, 0, -1}, //c_b3_x
		{-1, 0, 1, 0}, //c_b3_y
			
		0,0, 0,0, 0,0};

	load_tetromino(columns, tetris_state, &t_loc);
	
}



//---------------------------------------
// Function: load_I_tetromino
//
// Description: Create struct tetromino_location pointer with specific data for I Tetromino and call load_tetromino function
//
// Input: int columns,
//        uint8_t tetris_state[][columns]
//
// Output: None
//---------------------------------------
void load_I_tetromino(int columns, uint8_t tetris_state[][columns]) {
	//LCD_data(0x02);//Bottom
	//LCD_data(0x02);//Bottom
	//LCD_data(0x02);//Bottom
	//LCD_data(0x02);//Bottom

		// Orientation = 0
		// [B1]
		// [C]
		// [B2]
		// [B3]
		
		// Orientation 2-4, add 90, 180 and 270 degree rotation counter clockwise respectively around center block
		
		// For B1-B3, these 3 arrays are the x/y difference between itself and the center block, based on the orientation 1-4
		
		
		
		struct tetromino_location t_loc = {
			0, //orientation
			2, //entry_row
			16,			//entry_column
			
			{0, -1, 0, 1}, //c_b1_x
			{1, 0, -1, 0}, //c_b1_y
			
			{0, 1, 0, -1}, //c_b2_x
			{-1, 0, 1, 0}, //c_b2_y
			
			{0, 2, 0, -2}, //c_b3_x
			{-2, 0, 2, 0}, //c_b3_y
			
		0,0, 0,0, 0,0};

		load_tetromino(columns, tetris_state, &t_loc);
	
}


//---------------------------------------
// Function: load_O_tetromino
//
// Description: Create struct tetromino_location pointer with specific data for O Tetromino and call load_tetromino function
//
// Input: int columns,
//        uint8_t tetris_state[][columns]
//
// Output: None
//---------------------------------------
void load_O_tetromino(int columns, uint8_t tetris_state[][columns]) {
	//LCD_data(0x03);//All
	//LCD_data(0x03);//All
	
		// Orientation = 0
		// [C ][B1]
		// [B2][B3]
		// 
		
		// Orientation 2-4, add 90, 180 and 270 degree rotation counter clockwise respectively around center block
		
		// For B1-B3, these 3 arrays are the x/y difference between itself and the center block, based on the orientation 1-4
		
		
		struct tetromino_location t_loc = {
			rand() % 4, //orientation
			2, //entry_row
			16,			//entry_column
			
			{1, 0, -1, 0}, //c_b1_x
			{0, 1, 0, -1}, //c_b1_y
			
			{0, 1, 0, -1}, //c_b2_x
			{-1, 0, 1, 0}, //c_b2_y
			
			{1, 1, -1, -1}, //c_b3_x
			{-1, 1, 1, -1}, //c_b3_y
			
		0,0, 0,0, 0,0};

		load_tetromino(columns, tetris_state, &t_loc);
	
	
}




//---------------------------------------
// Function: load_T_tetromino
//
// Description: Create struct tetromino_location pointer with specific data for T Tetromino and call load_tetromino function
//
// Input: int columns,
//        uint8_t tetris_state[][columns]
//
// Output: None
//---------------------------------------
void load_T_tetromino(int columns, uint8_t tetris_state[][columns]) {
	//LCD_data(0x02);//Bottom
	//LCD_data(0x03);//All
	//LCD_data(0x02);//Bottom
	
		// Orientation = 0
		// [B1][C ][B2]
		//     [B3]
		//
		
		// Orientation 2-4, add 90, 180 and 270 degree rotation counter clockwise respectively around center block
		
		// For B1-B3, these 3 arrays are the x/y difference between itself and the center block, based on the orientation 1-4
		
		
		struct tetromino_location t_loc = {
			rand() % 4, //orientation
			2, //entry_row
			16,			//entry_column
			
			{-1, 0, 1, 0}, //c_b1_x
			{0, -1, 0, 1}, //c_b1_y
			
			{1, 0, -1, 0}, //c_b2_x
			{0, 1, 0, -1}, //c_b2_y
			
			{0, 1, 0, -1}, //c_b3_x
			{-1, 0, 1, 0}, //c_b3_y
			
		0,0, 0,0, 0,0};

		load_tetromino(columns, tetris_state, &t_loc);
	
}



//---------------------------------------
// Function: load_S_tetromino
//
// Description: Create struct tetromino_location pointer with specific data for S Tetromino and call load_tetromino function
//
// Input: int columns,
//        uint8_t tetris_state[][columns]
//
// Output: None
//---------------------------------------
void load_S_tetromino(int columns, uint8_t tetris_state[][columns]) {
	//LCD_data(0x02);//Bottom
	//LCD_data(0x03);//All
	//LCD_data(0x01);//Top
	
	
		// Orientation = 0
		//	   [B2][B1]
		// [B3][C ]
		//
		
		// Orientation 2-4, add 90, 180 and 270 degree rotation counter clockwise respectively around center block
		
		// For B1-B3, these 3 arrays are the x/y difference between itself and the center block, based on the orientation 1-4
		
		
		struct tetromino_location t_loc = {
			rand() % 4, //orientation
			2, //entry_row
			16,			//entry_column
			
			{1, -1, -1, 1}, //c_b1_x
			{1, 1, -1, -1}, //c_b1_y
			
			{0, -1, 0, 1}, //c_b2_x
			{1, 0, -1, 0}, //c_b2_y
			
			{-1, 0, 1, 0}, //c_b3_x
			{0, -1, 0, 1}, //c_b3_y
			
		0,0, 0,0, 0,0};//Block1, Block2, Block3
		
		load_tetromino(columns, tetris_state, &t_loc);
	
}




//---------------------------------------
// Function: load_Z_tetromino
//
// Description: Create struct tetromino_location pointer with specific data for Z Tetromino and call load_tetromino function
//
// Input: int columns,
//        uint8_t tetris_state[][columns]
//
// Output: None
//---------------------------------------
void load_Z_tetromino(int columns, uint8_t tetris_state[][columns]) {
	//LCD_data(0x01);//Top
	//LCD_data(0x03);//All
	//LCD_data(0x02);//Bottom

		// Orientation = 0
		// [B1][B2]
		//     [C ][B3]
		//     
		
		// Orientation 2-4, add 90, 180 and 270 degree rotation counter clockwise respectively around center block
		
		// For B1-B3, these 3 arrays are the x/y difference between itself and the center block, based on the orientation 1-4
		
		
		struct tetromino_location t_loc = {
			rand() % 4, //orientation
			2, //entry_row
			16,			//entry_column
			
			{-1, 1, 1, -1}, //c_b1_x
			{1, 1, -1, -1}, //c_b1_y
			
			{0, 1, 0, -1}, //c_b2_x
			{1, 0, -1, 0}, //c_b2_y
			
			{1, 0, -1, 0}, //c_b3_x
			{0, -1, 0, 1}, //c_b3_y
			
		0,0, 0,0, 0,0};//Block1, Block2, Block3
		
		load_tetromino(columns, tetris_state, &t_loc);
	
}






//---------------------------------------
// Function: load_L_tetromino
//
// Description: Create struct tetromino_location pointer with specific data for L Tetromino and call load_tetromino function
//
// Input: int columns,
//        uint8_t tetris_state[][columns]
//
// Output: None
//---------------------------------------
void load_L_tetromino(int columns, uint8_t tetris_state[][columns]) {
	
		//LCD_data(0x02);//Bottom
		//LCD_data(0x02);//Bottom
		//LCD_data(0x03);//All

		// Orientation = 0
		// [B2][B1]
		//     [C]
		//     [B3]
		
		// Orientation 2-4, add 90, 180 and 270 degree rotation counter clockwise respectively around center block
		
		// For B1-B3, these 3 arrays are the x/y difference between itself and the center block, based on the orientation 1-4
		
		
		struct tetromino_location t_loc = {
			rand() % 4, //orientation
			2, //entry_row
			16,			//entry_column
			
			{0, -1, 0, 1}, //c_b1_x
			{1, 0, -1, 0}, //c_b1_y
			
			{-1, -1, 1, 1}, //c_b2_x
			{1, -1, -1, 1}, //c_b2_y
			
			{0, 1, 0, -1}, //c_b3_x
			{-1, 0, 1, 0}, //c_b3_y
			
		0,0, 0,0, 0,0};
		
		struct tetromino_location *t_loc_p = &t_loc;
		
		load_tetromino(columns, tetris_state, t_loc_p);
	
}





//---------------------------------------
// Function: load_random_tetromino
//
// Description: Drop random tetromino block into display from Y=15 and descend till it lands on an existing block or the bottom of display
//
// Input: int columns,
//        uint8_t tetris_state[][columns],
//        struct tetromino_location *t_loc_p
//
// Output: None
//---------------------------------------
void load_random_tetromino(int columns, uint8_t tetris_state[][columns]) {
	int rand_num = rand() % 7;
	
	switch(rand_num)
	{
		case 0:
			load_I_tetromino(columns,tetris_state);
			break;
	
		case 1:
			load_O_tetromino(columns,tetris_state);
			break;
	
		case 2:
			load_T_tetromino(columns,tetris_state);
			break;
	
		case 3:
			load_S_tetromino(columns,tetris_state);
			break;
	
		case 4:
			load_Z_tetromino(columns,tetris_state);
			break;
	
		case 5:
			load_J_tetromino(columns,tetris_state);
			break;
	
		case 6:
			load_L_tetromino(columns,tetris_state);
			break;
	}	
}


