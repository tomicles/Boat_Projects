
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

#define REFVOLTS = 5.14

SoftwareSerial lcdSerial(10, 11, 1);  //RX, TX, Inverted
SoftwareSerial segSerial(6, 5);  //RX, TX


/* 
############################################
           Counts for temp sensor
	   data averaging
############################################
*/

#define CNT 100
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
float AM2302_t[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // AM2302 Temperature
float AM2302_h[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // AM2302 Humidity
float i2c_a2_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // i2c Analog Input 2
float i2c_a3_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // i2c Analog Input 3
float i2c_a4_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // i2c Analog Input 4
float i2c_a5_array[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // i2c Analog Input 5

//int VAREND = VAR-1;  // starting point for where to store data in the array.
int first = 1;  // dont calculate average until 1 full set of array data is availabe.
int current_count = 0;  // starting point for keeping track of CNT on main loop

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
	//screenTest();

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
		current_count=0;
	}

	stableLight = stableLight * 0.98 + analogRead(LIGHTSENSOR) * 0.02;
	


	voltageDividerTest();

	delay(10);


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
###########################################################
*/
void setMinMax(float x[])
{
	if (x[0] >= x[2]) { x[2] = x[0]; }
	if (x[0] <= x[1]) { x[1] = x[0]; }
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
   lcdSerial.write(0x02); // Set font size one step larger (4x16->2x16->4x8->2x8->1x8->1x4)
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
    if (clearScreen == 1){ // only clear screen if first time through loop. 
    	lcdSerial.write(0x0C); // clear screen
      	clearScreen = 0;
    }
    lcdSerial.write(0x03); // set font back to normal
    lcdSerial.write(0x01); // Home
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
				voltage(x)
		Convert AD output to voltage. 
###########################################################
*/
float voltage(int sensorValue){
 //float result = sensorValue * (4.124 / 4096);
 float result = sensorValue * (5.00 / 4096);
 return result; 
}

/* 
###########################################################
			voltageDivider(divided_voltage, r1, r2)
		Calculate actual voltage from voltage divider
###########################################################
*/
float voltageDivider(float voltage, int r1, int r2){
	//int resistorFactor = 1024;
	float multiplier = (r1+r2)/r2;
	float actualVolts = voltage * multiplier;
	return actualVolts;

}


/* 
###########################################################
				voltageDividerTest()
Show raw input, voltage calculation, and multiply for vdivider
###########################################################
*/
void voltageDividerTest(void){
	int a2 = getAnalogInput(2);  // amp meter 8v return divided
	int a3 = getAnalogInput(3);  // 12v regulator divided
	int a4 = getAnalogInput(4);  // raw input divided
	int a5 = getAnalogInput(5);  // amp meter reading divided
	lcdSerial.write(0x01); // Home
	lcdSerial.write(0x03);
	lcdSerial.print(a3);
	lcdSerial.print(" ");
	lcdSerial.print(voltage(a3));
	lcdSerial.print(" ");
	lcdSerial.print(voltageDivider(voltage(a3),3213, 1000));
	lcdSerial.write(0x0D); // Carriage Return
	lcdSerial.print(a4);
	lcdSerial.print(" ");
	lcdSerial.print(voltage(a4));
	lcdSerial.print(" ");
	lcdSerial.print(voltageDivider(voltage(a4),3224, 991));
	lcdSerial.write(0x0D); // Carriage Return
	lcdSerial.print(a5);
	lcdSerial.print(" ");
	lcdSerial.print(voltage(a5));
	lcdSerial.print(" ");
	lcdSerial.print(voltageDivider(voltage(a5),325, 1029));
	lcdSerial.write(0x0D); // Carriage Return
	lcdSerial.print(a2);
	lcdSerial.print(" ");
	lcdSerial.print(voltage(a2));
	lcdSerial.print(" ");
	lcdSerial.print(voltageDivider(voltage(a2),993, 657));
}

/* 
###########################################################
				screenTest()
Show all fonts and fill the screen in each mode
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
	lcdSerial.write(0x03); // normal font
	lcdSerial.write(0x01); // home
	setFont(3);
	lcdSerial.write(0x01); // home
	lcdSerial.print("Starting");
	lcdSerial.write(0x0D); // beginning next line
	lcdSerial.print("Sailboat");
	lcdSerial.write(0x0D); // beginning next line
	lcdSerial.print("  9000  ");	
	delay(5000);
	lcdSerial.write(0x0C); //clear screen
	lcdSerial.write(0x01); // home
}