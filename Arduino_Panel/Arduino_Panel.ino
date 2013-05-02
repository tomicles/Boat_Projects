
// 3rd party libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_GPS.h>

// build in libraries
#include <SoftwareSerial.h>


/* 
############################################
             Pin Connections
############################################
*/ 

#define ONE_WIRE_BUS 4  // data pin for one wire temp sensors
#define DHTPIN 13     // data pin for AM2302 Temp/Humidity sensor

SoftwareSerial lcdSerial(10, 11, 1);  //RX, TX, Inverted
SoftwareSerial segSerial(6, 5);  //RX, TX


/* 
############################################
           Counts for temp sensor
	   data averaging
############################################
*/

#define CNT 3
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






/* 
############################################
                  SETUP 
############################################
*/

void setup(void)
{

	// set resolution of OneWire temperature Sensors to 12 bits
	sensors.setResolution(OneWire1, 12);
	sensors.setResolution(OneWire2, 12);
	sensors.setResolution(OneWire3, 12);
	sensors.setResolution(OneWire4, 12);
	sensors.setResolution(OneWireW, 12);

	// start DHT libarary for AM2302 Sensor
	dht.begin();
}







/* 
############################################
             Start Main Loop
############################################
*/

void loop(void)
{
	// Keep track of how many times we've looped per CNT
	if(first==1){
		current_count=0;
	}
	current_count = current_count + 1;
	if(current_count >= CNT){
		current_count=0;
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
###########################################################
*/
void setMinMax(float x[])
{
	if (x[0] >= x[2]) { x[2] = x[0]; }
	if (x[0] <= x[1]) { x[1] = x[0]; }
}



