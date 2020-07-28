// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.


@SCREEN
D=A
@KBD
D=A-D					// &KBD - &SCREEN
@screenCellsNum
M=D-1

(WAIT_FOR_BTN)			// wait until any button is pressed
		@i				// reset loop counter
		M=0
		@KBD
		D=M
		@FILL_LOOP
		D;JNE
		@CLEAR_LOOP
		D;JEQ

(FILL_LOOP)				// screen filling loop	
	@i
	D=M
	@SCREEN
	A=A+D
	M=-1
	@i
	M=M+1
	D=M
	@screenCellsNum
	D=D-M
	@WAIT_FOR_BTN
	D;JGT
	@FILL_LOOP
	0;JMP

(CLEAR_LOOP)			// screen clearing loop
	@i
	D=M
	@SCREEN
	A=A+D
	M=0
	@i
	M=M+1
	D=M
	@screenCellsNum
	D=D-M
	@WAIT_FOR_BTN
	D;JGT
	@CLEAR_LOOP
	0;JMP
@WAIT_FOR_BTN
	
