Arduino Panel Manual Test Functions
/* 
###########################################################
				ampTest()  NOT WORKING YET
Show power input, power output, difference, amps
		| input : 7.60      |
		| output: 3.47      |
		| diff  : 4.13      |
		| amps  : 0.00      |
###########################################################
*/
void ampTest(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		float input = voltageRatio(voltage(i2c_a2_array[3]),2);
		float output = voltageRatio(voltage(i2c_a3_array[3]),3);
		float half = input/2;
		float diff = half - output; 
		//float current = (output - (half+0.2)) / 0.033;
		float current = (output - half) / 0.033;

		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print("input : ");
		lcdSerial.print(input);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("output: ");
		lcdSerial.print(output);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("half   : ");
		lcdSerial.print(half);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("current: ");
		lcdSerial.print(current);

		//Current = ((analogRead(1)*(5.00/1024))- 2.5)/ .017;
	}
}



/* 
###########################################################
				voltageDividerTest()
Show raw input, voltage calculation, and multiply for vdivider
		| 3001 3.01 7.57   |
		| 2589 2.60 3.45   |
		| 2842 2.86 12.10  |
		| 2507 2.52 10.63  |
###########################################################
*/
void voltageDividerTest(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print(i2c_a2_array[3],0);
		lcdSerial.print(" ");	
		lcdSerial.print(voltage(i2c_a2_array[3]),2);
		lcdSerial.print(" ");
		lcdSerial.print(voltageRatio(voltage(i2c_a2_array[3]),2));
			lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print(i2c_a3_array[3],0);
		lcdSerial.print(" ");	
		lcdSerial.print(voltage(i2c_a3_array[3]),2);
		lcdSerial.print(" ");
		lcdSerial.print(voltageRatio(voltage(i2c_a3_array[3]),3));
				lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print(i2c_a4_array[3],0);
		lcdSerial.print(" ");	
		lcdSerial.print(voltage(i2c_a4_array[3]),2);
		lcdSerial.print(" ");
		lcdSerial.print(voltageRatio(voltage(i2c_a4_array[3]),4));
				lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print(i2c_a5_array[3],0);
		lcdSerial.print(" ");	
		lcdSerial.print(voltage(i2c_a5_array[3]),2);
		lcdSerial.print(" ");
		lcdSerial.print(voltageRatio(voltage(i2c_a5_array[3]),5));
	}
}

/* 
###########################################################
				buttonTest()
Show button1 and button2 press state
###########################################################
*/
void buttonTest(void){
	//lcdSerial.write(0x0C);
	//delay(500);
	lcdSerial.write(0x01);  // Home cursor
	lcdSerial.write(0x03);  // normal font
	lcdSerial.print("Button Test");
	// read the state of the pushbutton value:
	button1State = digitalRead(BUTTON1);
	button2State = digitalRead(BUTTON2);
	//button2State = digitalRead(BUTTON2);

	// check if the pushbutton is pressed.
	// if it is, the buttonState is HIGH:
	if (button1State == HIGH) {   
		lcdSerial.write(0x0C);
		lcdSerial.write(0x0D); // beginning next line)
		lcdSerial.print("Button 1 pressed");
	} 
	else {
		lcdSerial.write(0x01);  // Home cursor
		lcdSerial.write(0x03);  // normal font
		lcdSerial.print("Button Test");
	}
	if (button2State == HIGH) {
		lcdSerial.write(0x0C);
		lcdSerial.write(0x0D); // beginning next line)
		lcdSerial.print("Button 2 pressed");
	}
	else {
		lcdSerial.write(0x01);  // Home cursor
		lcdSerial.write(0x03);  // normal font
		lcdSerial.print("Button Test");
	}

	//lcdSerial.print("Button1: ")
}

/*
###########################################################
				screenTest()
Show all fonts and fills the screen in each mode
###########################################################
*/
void screenTest(void){
	lcdSerial.write(0x0C);
	delay(500);
	lcdSerial.write(0x01);  // Home cursor
	lcdSerial.write(0x03);  // normal font
	lcdSerial.print("0123456789abcdef");
	lcdSerial.print("0123456789abcdef");
	lcdSerial.print("0123456789abcdef");
	lcdSerial.print("0123456789abcdef");
	delay(1000);

	lcdSerial.write(0x0C);
	delay(500);
	lcdSerial.write(0x01);  // Home cursor
	lcdSerial.write(0x03);  // normal font
	setFont(2);
	lcdSerial.print("0123456789abcdef");
	lcdSerial.print("0123456789abcdef");	
	delay(1000);

	lcdSerial.write(0x0C);
	delay(500);
	lcdSerial.write(0x01);  // Home cursor
	lcdSerial.write(0x03);  // normal font
	setFont(3);
	lcdSerial.print("01234567");
	lcdSerial.print("01234567");	
	lcdSerial.print("01234567");
	lcdSerial.print("01234567");	
	delay(1000);

	lcdSerial.write(0x0C);
	delay(500);
	lcdSerial.write(0x01);  // Home cursor
	lcdSerial.write(0x03);  // normal font
	setFont(4);
	lcdSerial.print("01234567");
	lcdSerial.print("01234567");	
	delay(1000);

	lcdSerial.write(0x0C);
	delay(500);
	lcdSerial.write(0x01);  // Home cursor
	lcdSerial.write(0x03);  // normal font
	setFont(5);
	lcdSerial.print("01234567");
	delay(1000);

	lcdSerial.write(0x0C);
	delay(500);
	lcdSerial.write(0x01);  // Home cursor
	lcdSerial.write(0x03);  // normal font
	setFont(6);
	lcdSerial.print("0123");
	delay(1000);

	lcdSerial.write(0x0C);
	delay(500);
	lcdSerial.write(0x01);  // Home cursor
	lcdSerial.write(0x03);  // normal font
	setFont(6);
	lcdSerial.write(0x13);
	lcdSerial.print("0123");
	delay(3000);
}