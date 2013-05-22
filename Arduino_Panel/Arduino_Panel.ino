
// 3rd party libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_GPS.h>

// build in libraries
#include <SoftwareSerial.h>
#include <Wire.h>


/* 
############################################
             Pin Connections & Defines
############################################
*/ 

#define ONE_WIRE_BUS 4  // data pin for one wire temp sensors
#define DHTPIN 13     // data pin for AM2302 Temp/Humidity sen
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

#define CNT 100  // how many values to save for average
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
//float OneWire1_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWire1
//float OneWire2_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWire2
//float OneWire3_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWire3
//float OneWire4_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWire4
//float OneWireW_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // OneWireW
//float AM2302_t[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // AM2302 Temperature
//float AM2302_h[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // AM2302 Humidity
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

long lastDebounce1Time = 0;
long debounce1Delay = 50;


int menuItems = 2;
int menuItem = 0;

/* 
############################################
             Call libraries
############################################
*/

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices
DHT dht(DHTPIN, DHT22); // AM2302 Sensor Connection.  Removed reference to DHT22 and called it directly
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 


/* 
############################################
             Define variables
############################################
*/

// light sensor to dim lcd screen

float stableLight = 0;
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

	// start DHT libarary for AM2302 Sensor
	dht.begin();

	// Start Serial ports
	lcdSerial.begin(9600);
	Serial.begin(9600);
	resetScreen();
	delay(1000);
	startupScreen();
	// screenTest();

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
		first=0;
	}

	stableLight = stableLight * 0.98 + analogRead(LIGHTSENSOR) * 0.02;

	//sensors.requestTemperatures();
	//OneWire1_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWire1));
	//OneWire2_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWire2));
	//OneWire3_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWire3));
	//OneWire4_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWire4));
	//OneWireW_array[0] = DallasTemperature::toFahrenheit(sensors.getTempC(OneWireW));
	//AM2302_t[0] = DallasTemperature::toFahrenheit(dht.readTemperature());
	//AM2302_h[0] = dht.readHumidity();
	i2c_a2_array[0] = getAnalogInput(2);
	i2c_a3_array[0] = getAnalogInput(3);
	i2c_a4_array[0] = getAnalogInput(4);
	i2c_a5_array[0] = getAnalogInput(5);

	//setMinMax(OneWire1_array);
	//setMinMax(OneWire2_array);
	//setMinMax(OneWire3_array);
	//setMinMax(OneWire4_array);
	//setMinMax(OneWireW_array);
	//setMinMax(AM2302_t);
	//setMinMax(AM2302_h);
	setMinMax(i2c_a2_array);
	setMinMax(i2c_a3_array);
	setMinMax(i2c_a4_array);
	setMinMax(i2c_a5_array);
	
	//getTemp(OneWire1_array, current_count);
	//getTemp(OneWire2_array, current_count);
	//getTemp(OneWire3_array, current_count);
	//getTemp(OneWire4_array, current_count);
	//getTemp(OneWireW_array, current_count);
	//getTemp(AM2302_t, current_count);
	//getTemp(AM2302_h, current_count);
	getTemp(i2c_a2_array, current_count);
	getTemp(i2c_a3_array, current_count);
	getTemp(i2c_a4_array, current_count);
	getTemp(i2c_a5_array, current_count);


	// voltageDividerTest(); 
	// ampTest();
	// buttonTest();

	button1State = digitalRead(BUTTON1);
	if(button1State != lastButton1State) //compare increment button state to its last state
	{
		if(button1State == LOW)//increment button is pressed
			{
				menuItem = menuItem + 1; //increment the counter
				lcdSerial.write(0x0C); //clear screen
				lcdSerial.write(0x01); // home
				if (menuItem > menuItems){
					menuItem = 0;
				}
			}
		}
	lastButton1State = button1State;

	// if (button1State == HIGH) {  
	// 	menuItem = menuItem + 1;
	// 	delay(20);
	// 	if (menuItem > menuItems){
	// 		menuItem = 0;
	// 	}
	// } 


	switch (menuItem){
		case 0:
			voltageDividerTest();
			// lcdSerial.write(0x01);  // Home cursor
			// lcdSerial.write(0x03);  // normal font
			// lcdSerial.print("Case 0");
			break;
		case 1:
			ampTest();
			// lcdSerial.write(0x01);  // Home cursor
			// lcdSerial.write(0x03);  // normal font
			// lcdSerial.print("Case 1");
			break;
		case 2:
			//screenTest();
			lcdSerial.write(0x01);  // Home cursor
			lcdSerial.write(0x03);  // normal font
			lcdSerial.print("Case 2");
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
    gpsTest(void)
    GPS
###########################################################
*/
// void gpsTest(void){
// 	lcdSerial.write(0x01); // Home
// 	lcdSerial.write(0x03);
// 	lcdSerial.print("Fix: ");
// 	lcdSerial.print((int)GPS.fix);
// 	lcdSerial.write(0x0D); // Carriage Return
// 	lcdSerial.print(" quality: ");
// 	lcdSerial.print((int)GPS.fixquality);
// 	if (GPS.fix) {
// 		lcdSerial.write(0x01); // Home
// 		lcdSerial.write(0x03);
// 		//lcdSerial.print("Location: ");
// 		//lcdSerial.print(GPS.latitude, 4); lcdSerial.print(GPS.lat);
// 		//lcdSerial.print(", "); 
// 		//lcdSerial.print(GPS.longitude, 4); 
// 		//lcdSerial.print(GPS.lon);
// 		lcdSerial.print("Speed (knots): "); lcdSerial.print(GPS.speed);
// 		lcdSerial.write(0x0D); // Carriage Return
// 		lcdSerial.print("Angle: "); lcdSerial.print(GPS.angle);
// 		lcdSerial.write(0x0D); // Carriage Return
// 		lcdSerial.print("Altitude: "); lcdSerial.print(GPS.altitude);
// 		lcdSerial.write(0x0D); // Carriage Return
// 		lcdSerial.print("Satellites: "); lcdSerial.print((int)GPS.satellites);
// 	}

// }
/* 
###########################################################
    setMinMax(OneWire1_array)
    Store min or max value from array into same array
    array[0] current value
    array[1] min
    array[2] max
###########################################################
*/
void setMinMax(float x[])
{
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
void getTemp(float x[], int count)
{
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
  const byte DAT[8] = {
    0x8C,0xCC,0x9C,0xDC,0xAC,0xEC,0xBC,0xFC    };
  // Constant configuration data
  int loWord;
  byte Adval_High, Adval_Low;    // Store A/D value (high byte, low byte)

  Wire.beginTransmission(I2C_address);
  Wire.write(DAT[x]);        // Configure the device to read each CH
  Wire.endTransmission();
  delay(1);
  // Read A/D value
  Wire.requestFrom(I2C_address, 2);
  while(Wire.available())          // Checkf for data from slave
  {
    Adval_High = Wire.read();   // Receive A/D high byte
    Adval_Low = Wire.read();    // Receive A/D low byte
  }
  result = word(Adval_High, Adval_Low);
  return result;
}  

/* 
###########################################################
				amps(x)
		Convert voltage to amps from amp meter
###########################################################
*/
// float amps(float volts){
// 	float vPerAmp = 0.033;
// 	float zero = 

// }


/* 
###########################################################
				voltage(x)
		Convert AD output to voltage. 
###########################################################
*/
float voltage(int sensorValue){
 //float result = sensorValue * (4.124 / 4096);
 //float result = sensorValue * (4.123 / 4096);
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
		float current = (output - (half+0.2)) / 0.033;
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
	lcdSerial.print("Sailboat");
	lcdSerial.write(0x0D); // beginning next line
	lcdSerial.print("  9000  ");	
	delay(2000);
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
	lcdSerial.print("Wait... ");	
	lcdSerial.print(104-current_count);
	if(current_count >= CNT+VAR-2){
		lcdSerial.write(0x0C);
	}

}