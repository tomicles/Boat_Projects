
// 3rd party libraries
#include <OneWire.h>
#include <DallasTemperature.h>

// build in libraries
#include <SoftwareSerial.h>
#include <Wire.h>


/* 
############################################
             Pin Connections & Defines
############################################
*/ 

#define ONE_WIRE_BUS 4  // data pin for one wire temp sensors
#define BUTTON1 8
#define BUTTON2 9           
#define LIGHTSENSOR A0 //A0 --> 14


SoftwareSerial lcdSerial(10, 11, 1);  //RX, TX, Inverted
SoftwareSerial segSerial(6, 5);  //RX, TX



/* 
############################################
       Counts for temp sensor
	   data averaging
############################################
*/

#define CNT 30  // how many values to save for average
#define VAR 5  // last,min,max,ave,sum

/* 
############################################
           OneWire Temp Sensors
############################################
*/ 

DeviceAddress OneWire1 = {
  0x28, 0x48, 0x89, 0x2B, 0x04, 0x00, 0x00, 0x88 };
DeviceAddress OneWire2 = {
  0x28, 0xC8, 0xCC, 0x7A, 0x04, 0x00, 0x00, 0x0E };
DeviceAddress OneWire3 = {
  0x28, 0x62, 0x17, 0x7B, 0x04, 0x00, 0x00, 0x3C };
DeviceAddress OneWire4 = {
  0x28, 0x12, 0xDF, 0x00, 0x04, 0x00, 0x00, 0x84 };
DeviceAddress OneWireW = {
  0x28, 0x1F, 0x5F, 0xCB, 0x03, 0x00, 0x00, 0xD2 };


// Initialize sensor arrays {sensorvalue, min, max, ave, sum, [readings]}
float OneWire1_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWire1
float OneWire2_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWire2
float OneWire3_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWire3
float OneWire4_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWire4
float OneWireW_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWireW
float i2c_a2_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // i2c Analog Input 2
float i2c_a3_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // i2c Analog Input 3
float i2c_a4_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // i2c Analog Input 4
float i2c_a5_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // i2c Analog Input 5

int VAREND = VAR-1;  // starting point for where to store data in the array.
int first = 1;  // dont calculate average until 1 full set of array data is availabe.
int current_count = VAR-1;  // starting point for keeping track of CNT on main loop

int button1State;
int button2State;
int lastButton1State = LOW;

int menuItems = 7;
int menuItem = 0;

/* 
############################################
             Call libraries
############################################
*/

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 


/* 
############################################
             Define variables
############################################
*/

// light sensor to dim lcd screen

float stableLight = 0;
float rawLight = 0;
int lightSensorMax = 0; // lowest value to start
int lightSensorMin =  1023;  // highest valuse to start
int clearScreen;  //just needs to be defined for later 

/* 
############################################
                  SETUP 
############################################
*/

void setup(void)
{

	// Set buttons to input  
	pinMode(BUTTON1, INPUT); 
	pinMode(BUTTON2, INPUT); 



	// set resolution of OneWire temperature Sensors to 12 bits
	sensors.setResolution(OneWire1, 12);
	sensors.setResolution(OneWire2, 12);
	sensors.setResolution(OneWire3, 12);
	sensors.setResolution(OneWire4, 12);
	sensors.setResolution(OneWireW, 12);


	// Start Serial ports
	lcdSerial.begin(9600);
	//Serial.begin(9600);
	resetScreen();
	delay(1000);
	startupScreen();

	// wire library for I2C AD. join i2c bus 
	Wire.begin();

	delay(1000);
}




/* 
############################################
             Start Main Loop
############################################
*/

void loop(void)
{



	current_count = current_count + 1;
	if(current_count >= CNT+VAR){
		current_count=VAR-1;
		first = 0;
	}
	rawLight = analogRead(LIGHTSENSOR);
	stableLight = stableLight * 0.98 + rawLight * 0.02;

	sensors.requestTemperatures();
	OneWire1_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWire1));
	OneWire2_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWire2));
	OneWire3_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWire3));
	OneWire4_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWire4));
	OneWireW_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWireW));
	i2c_a2_array[0] = getAnalogInput(2);
	i2c_a3_array[0] = getAnalogInput(3);
	i2c_a4_array[0] = getAnalogInput(4);
	i2c_a5_array[0] = getAnalogInput(5);

	setMinMax(OneWire1_array);
	setMinMax(OneWire2_array);
	setMinMax(OneWire3_array);
	setMinMax(OneWire4_array);
	setMinMax(OneWireW_array);
	setMinMax(i2c_a2_array);
	setMinMax(i2c_a3_array);
	setMinMax(i2c_a4_array);
	setMinMax(i2c_a5_array);
	
	getTemp(OneWire1_array, current_count);
	getTemp(OneWire2_array, current_count);
	getTemp(OneWire3_array, current_count);
	getTemp(OneWire4_array, current_count);
	getTemp(OneWireW_array, current_count);
	getTemp(i2c_a2_array, current_count);
	getTemp(i2c_a3_array, current_count);
	getTemp(i2c_a4_array, current_count);
	getTemp(i2c_a5_array, current_count);

	button1State = digitalRead(BUTTON1);
	if(button1State != lastButton1State){ //compare increment button state to its last state
		if(button1State == LOW){ //increment button is pressed
			menuItem = menuItem + 1; //increment the counter
			lcdSerial.write(0x0C); //clear screen
			lcdSerial.write(0x01); // home
			if (menuItem > menuItems){
				menuItem = 0;
			}
		}
	}
	lastButton1State = button1State;

	dimScreen(200);

	// cycle through screens from button  press
	switch (menuItem){
		case 0:
			voltageScreen();
			break;
		case 1:
			tempScreen1();
			break;
		case 2:
			tempScreen2();
			break;
		case 3:
			waterTempDetail();
			break;
		case 4:
			arduinoTempDetail();
			break;
		case 5:
			insideTempDetail();
			break;
		case 6:
			lightDetail();
			break;
		
	}
}



/* 
############################################
	     define functions
############################################
*/


/* 
###########################################################
    setMinMax(OneWire1_array)
    Store min or max value from array into same array
    array[0] current value
    array[1] min
    array[2] max
###########################################################
*/
void setMinMax(float x[]){
	if (x[0] >= x[2]) { x[2] = x[0]; }
	if (x[0] <= x[1]) { x[1] = x[0]; }
}

/* 
###########################################################
    getTemp(array, count)
Store actual value into array position for current loop iteration
Calculate and store average
###########################################################
*/
void getTemp(float x[], int count){
	int sum;
	x[current_count] = x[0]; // set actual value into array loop position
	if(count==CNT+VAR-1){   // reset variable after the frist loop is reached
		first = 0;
	}
	if(first==0){	//start calculating average since we have a full set of data
		x[4]=0;
		for(sum=5; sum<(CNT+VAR); sum++){
			x[4] += x[sum];
		}
		x[3] = x[4]/CNT;
	}
}
/* 
###########################################################
				setFont(x)
where x is 1 = 4x16, 2 = 2x16, 3=4x8, 4=2x8, 5=1x8, 6=1x4
###########################################################
*/
int setFont(int x){
	lcdSerial.write(0x03); // set font back to normal
	for (int i = 1; i < x; i++){
		lcdSerial.write(0x02); // Set font size one step larger 
						  // (4x16->2x16->4x8->2x8->1x8->1x4)
		delay(10);
	}
}

/* 
###########################################################
				resetScreen()
		reset serial screen to normal boot position
###########################################################
*/
void resetScreen(void){
    lcdSerial.write(0x03); // set font back to normal
    lcdSerial.write(0x01); // Home
    lcdSerial.write(0x0C); // clear screen
}

/* 
###########################################################
				dimScreen(x)
		dim screen if in low ambient light
###########################################################
*/
void dimScreen(int x){
	// offset by 10 to stop brightness jitter
	if (stableLight < (x - 5)) {
		lcdSerial.write(0x0F); // Display Driver Off
	}
	if (stableLight > (x + 5)) {
		lcdSerial.write(0x0E); // Display Driver On
	}
}


/* 
###########################################################
				getAnalogInput(x)
		    x is analog channel 0 - 7
###########################################################
*/
int getAnalogInput(byte x){  // x is analog channel 0 - 7
	int result;
	const int I2C_address = 0x48;  // I2C write address
	const byte DAT[8] = { 0x8C,0xCC,0x9C,0xDC,0xAC,0xEC,0xBC,0xFC };
	// Constant configuration data
	int loWord;
	byte Adval_High, Adval_Low;    // Store A/D value (high byte, low byte)

	Wire.beginTransmission(I2C_address);
	Wire.write(DAT[x]);        // Configure the device to read each CH
	Wire.endTransmission();
	delay(1);
	// Read A/D value
	Wire.requestFrom(I2C_address, 2);
	while(Wire.available()){          // Checkf for data from slave
		Adval_High = Wire.read();   // Receive A/D high byte
		Adval_Low = Wire.read();    // Receive A/D low byte
	}
	result = word(Adval_High, Adval_Low);
	return result;
}  


/* 
###########################################################
				voltage(x)
		Convert AD output to voltage. 
###########################################################
*/
float voltage(int sensorValue){
	float sample1 = 4096*(3.054/3040);
	float result = sensorValue * (sample1 / 4096);
	return result; 
}

/* 
###########################################################
			voltageRatio(divided_voltage, input)
		Calculate actual voltage from voltage divider
		input = 0-7 for I2C-ADC Input channel 0-7
###########################################################
*/
float voltageRatio(float voltage, int input){
	// input = number
	if(input==0){  // a0 analog input
	}
	if(input==1){  // a1 analog input
	}
	if(input==2){  // a2 analog input
		// 8 volt linear regulator from amp meter board
		// passes through Voltage Divider R1 = 993ohm, R2 = 657ohm
		// sample 1 ratio = 7.76/3.09
		float actualVolts = voltage *(7.76/3.09);  // measured
		return actualVolts;
	}
	if(input==3){  // a3 analog input
		// amp meter board readout
		// passes through Voltage Divider R1 = 325ohm, R2 = 1029ohm
		// sample 1 ratio = 3.59/2.708
		float actualVolts = voltage *(3.59/2.708);  // measured
		return actualVolts;
	}
	if(input==4){  // a4 analog input
		// Raw power input
		// passes through Voltage Divider R1 = 3213ohm, R2 = 1000ohm
		// sample 1 ratio = 12.14/2.865
		float actualVolts = voltage *(12.14/2.865);  // measured
		return actualVolts;
	}
	if(input==5){  // a5 analog input
		// 12v linear regulator output voltage
		// passes through Voltage Divider R1 = 3224ohm, R2 = 991ohm
		// sample 1 ratio = 10.64/2.521
		float actualVolts = voltage *(10.64/2.521);  // measured
		return actualVolts;
	}
	if(input==6){  // a6 analog input
	}
	if(input==7){  // a7 analog input
	}
}


/* 
###########################################################
				voltageScreen()
Show raw input, voltage calculation, and multiply for vdivider
		| System Power      |
		| Battery:   12.45  |  
		| 12v reg:   10.83  |  
		| Amps   :    7.28  |  
###########################################################
*/
void voltageScreen(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print("System Power");
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Battery:  ");
		//lcdSerial.write(0x12); //right align
		//lcdSerial.write('5'); // number of right align digits
		float battery = voltageRatio(voltage(i2c_a4_array[3]),4);
		lcdSerial.print(battery); //Battery
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("12v Reg:  ");
		float twelvevReg = voltageRatio(voltage(i2c_a5_array[3]),5);
		lcdSerial.print(twelvevReg);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Amps   :    ");
		float input = voltageRatio(voltage(i2c_a2_array[3]),2);
		float output = voltageRatio(voltage(i2c_a3_array[3]),3);
		float half = input/2;
		float current = (output - half) / 0.033;
		lcdSerial.write(0x12); // ctrl-R right-align the data, erasing unused digits
		lcdSerial.write('4'); // # digits
		lcdSerial.print(current);
	}
}

/* 
###########################################################
				tempScreen1()
			Show raw temperatures
		| Temperatures    |
		| Water  : 12.45  |  
		| Outside: 10.83  |  
		| Inside : 7.28   |  
###########################################################
*/
void tempScreen1(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print("Temperatures");
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Water  : ");
		lcdSerial.print(OneWireW_array[3]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Outside: ");
		lcdSerial.print(OneWire2_array[3]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Inside : ");
		lcdSerial.print(OneWire3_array[3]);

	}
}

/* 
###########################################################
				tempScreen2()
			Show raw temperatures
		| Temperatures    |
		| Circuit: 12.45  |  
		| Inside2: 10.83  |  
		| 				  |  
###########################################################
*/
void tempScreen2(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print("Temperatures");
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Circuit: ");
		lcdSerial.print(OneWire1_array[3]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Inside2: ");
		lcdSerial.print(OneWire4_array[3]);
	}
}

/* 
###########################################################
			waterTempDetail()
			Show raw temperatures
		| Temperatures      |
		| Water Min: 12.45  |  
		| Water Max: 10.83  |  
		| Water Ave: 12.15	|  
###########################################################
*/
void waterTempDetail(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print("Water Temps");
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Min: ");
		lcdSerial.print(OneWireW_array[1]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Max: ");
		lcdSerial.print(OneWireW_array[2]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Ave: ");
		lcdSerial.print(OneWireW_array[3]);
	}
}

/* 
###########################################################
			arduinoTempDetail()
			Show raw temperatures
		| Temperatures      |
		| Water Min: 12.45  |  
		| Water Max: 10.83  |  
		| Water Ave: 12.15	|  
###########################################################
*/
void arduinoTempDetail(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print("Arduino Temps");
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Min: ");
		lcdSerial.print(OneWire1_array[1]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Max: ");
		lcdSerial.print(OneWire1_array[2]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Ave: ");
		lcdSerial.print(OneWire1_array[3]);
	}
}
/* 
###########################################################
			insideTempDetail()
			Show raw temperatures
		| Temperatures      |
		| Water Min: 12.45  |  
		| Water Max: 10.83  |  
		| Water Ave: 12.15	|  
###########################################################
*/
void insideTempDetail(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print("Inside Temps");
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Min: ");
		lcdSerial.print(OneWire3_array[1]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Max: ");
		lcdSerial.print(OneWire3_array[2]);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Ave: ");
		lcdSerial.print(OneWire3_array[3]);
	}
}
/* 
###########################################################
			lightDetail()
			Show raw light sensor data
		| Light Sensor    |
		| Actual : 12.45  |  
		| Soft   : 10.83  |  
		| 			      |  
###########################################################
*/
void lightDetail(void){
	if(first==1){
		firstLoop();
	}
	if(first==0){
		lcdSerial.write(0x01); // Home
		lcdSerial.write(0x03);
		lcdSerial.print("Light Sensor");
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Actual : ");
		lcdSerial.print(rawLight);
		lcdSerial.write(0x0D); // Carriage Return
		lcdSerial.print("Soft   : ");
		lcdSerial.print(stableLight);
	}
}
/* 
###########################################################
				startupScreen()
		dim screen if in low ambient light
###########################################################
*/
void startupScreen(void){
	lcdSerial.write(0x0C); //clear screen
	lcdSerial.write(0x01); // home
	lcdSerial.write(0x03); // normal font
	lcdSerial.print("Starting");
	lcdSerial.write(0x0D); // beginning next line
	lcdSerial.print("Santana 2023c");
	lcdSerial.write(0x0D); // beginning next line
	lcdSerial.print("Sensor Arrays");	
	delay(5000);
	lcdSerial.write(0x0C); //clear screen
	lcdSerial.write(0x01); // home
}

/* 
###########################################################
				firstLoop()
		Show text while collecting data for first loop
###########################################################
*/
void firstLoop(void){
	lcdSerial.write(0x01); // home
	lcdSerial.write(0x03); // normal font
	lcdSerial.print("gathering data");
	lcdSerial.write(0x0D); // beginning next line
	lcdSerial.print("for 1st average");
	lcdSerial.write(0x0D); // beginning next line
	lcdSerial.print("computation.");	
	lcdSerial.write(0x0D); // beginning next line
	lcdSerial.print("Wait...    ");	
	lcdSerial.write(0x12); // ctrl-R right-align the data, erasing unused digits
    lcdSerial.write('3'); // # digits
	lcdSerial.print((CNT+VAR)-current_count);
	if(current_count >= CNT+VAR-4){
		lcdSerial.write(0x0C);
	}
}