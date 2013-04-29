
// Connect to Temp/humidity sensors and record data, print to serial
// Capacitor between GND and Reset to prevent usb connection reset

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// Screen Software Serial Setup
SoftwareSerial lcdSerial(10, 11, 1); // RX, TX, Inverted

#define ONE_WIRE_BUS 4  // Data wire is plugged into pin 8 on the Arduino

// how many sensor values to store for averaging. 
// 360 w/5000 delay = 1hr average
#define CNT 3
#define VAR 5 // last, min, max, ave, sum
#define DELAY 50

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// Assign the addresses of your 1-Wire temp sensors.
// See the tutorial on how to obtain these addresses:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-address-finder.html

DeviceAddress insideThermometer = { 
  0x28, 0x48, 0x89, 0x2B, 0x04, 0x00, 0x00, 0x88 };
DeviceAddress waterThermometer = { 
  0x28, 0x1F, 0x5F, 0xCB, 0x03, 0x00, 0x00, 0xD2 };
DeviceAddress twoThermometer = {
  0x28, 0xC8, 0xCC, 0x7A, 0x04, 0x00, 0x00, 0x0E };
DeviceAddress threeThermometer = { 
  0x28, 0x62, 0x17, 0x7B, 0x04, 0x00, 0x00, 0x3C };
DeviceAddress fourThermometer = { 
  0x28, 0x12, 0xDF, 0x00, 0x04, 0x00, 0x00, 0x84 };
  


// AM2302 Sensor Connection
#define DHTPIN 13     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// Initialize sensor arrays {sensorvalue, min, max, ave, sum, [readings]}
float insideThermometer_temp1[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // inside OneWire
float waterThermometer_temp2[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // outside OneWire
float twoThermometer_temp[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // outside OneWire
float threeThermometer_temp[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // outside OneWire
float fourThermometer_temp[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // outside OneWire
float AM2302_temp[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // inside AM2302(2)
float AM2302_humid[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // inside AM2302(2)

int i = VAR-1;  // starting point for where to store data in the array.
int first = 1;  // dont calculate average until 1 full set of array data is availabe.

int clearScreen;
void setup(void)
{
  lcdSerial.begin(9600);
  // Start up the library
  sensors.begin();
  // set the resolution of the sensors to 12 bit
  sensors.setResolution(insideThermometer, 12);
  sensors.setResolution(waterThermometer, 12);
  sensors.setResolution(twoThermometer, 12);
  sensors.setResolution(threeThermometer, 12);
  sensors.setResolution(fourThermometer, 12);
  dht.begin();
}

// pass array of a sensor to this function, records min or max value if logic says so. 
// setMinMax(insideThermometer_temp1);
void setMinMax(float x[])  
{
  if (x[0] >= x[2]) { x[2] = x[0]; }
  if (x[0] <= x[1]) { x[1] = x[0]; } 
}





void getTemp(float x[], int count)
{
	int sum;
	x[i] = x[0]; // set actual value into array loop position
	if(count==CNT+VAR-1){   // reset variable after the rist loop is reached
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

void loop(void)
{ 
	for(i=VAR-1;i<VAR+CNT; i++){
		sensors.requestTemperatures();
		insideThermometer_temp1[0] = DallasTemperature::toFahrenheit(sensors.getTempC(insideThermometer));
		waterThermometer_temp2[0] = DallasTemperature::toFahrenheit(sensors.getTempC(waterThermometer));
                twoThermometer_temp[0] = DallasTemperature::toFahrenheit(sensors.getTempC(twoThermometer));
                threeThermometer_temp[0] = DallasTemperature::toFahrenheit(sensors.getTempC(threeThermometer));
                fourThermometer_temp[0] = DallasTemperature::toFahrenheit(sensors.getTempC(fourThermometer));
		AM2302_temp[0] = DallasTemperature::toFahrenheit(dht.readTemperature());
		AM2302_humid[0] = dht.readHumidity();
  
		setMinMax(insideThermometer_temp1);
		setMinMax(waterThermometer_temp2);
                setMinMax(twoThermometer_temp);
                setMinMax(threeThermometer_temp);
                setMinMax(fourThermometer_temp);
		setMinMax(AM2302_temp);
		setMinMax(AM2302_humid);

		getTemp(insideThermometer_temp1,i);
		getTemp(waterThermometer_temp2,i);
		getTemp(twoThermometer_temp,i);
		getTemp(threeThermometer_temp,i);
		getTemp(fourThermometer_temp,i);
		getTemp(AM2302_temp,i);
		getTemp(AM2302_humid,i);
  
		Serial.print("{'insideThermometer_temp1': [");
		Serial.print(insideThermometer_temp1[0]);
		Serial.print(", ");
		Serial.print(insideThermometer_temp1[1]);
		Serial.print(", ");
		Serial.print(insideThermometer_temp1[2]);
		Serial.print(", ");
		Serial.println(insideThermometer_temp1[3]);
		Serial.print("], 'waterThermometer_temp2': [");
		Serial.print(waterThermometer_temp2[0]);
		Serial.print(", ");
		Serial.print(waterThermometer_temp2[1]);
		Serial.print(", ");
		Serial.print(waterThermometer_temp2[2]);
		Serial.print(", ");
		Serial.println(waterThermometer_temp2[3]);
		Serial.print("{'twoThermometer_temp': [");
		Serial.print(twoThermometer_temp[0]);
		Serial.print(", ");
		Serial.print(twoThermometer_temp[1]);
		Serial.print(", ");
		Serial.print(twoThermometer_temp[2]);
		Serial.print(", ");
		Serial.println(twoThermometer_temp[3]);
		Serial.print("{'threeThermometer_temp': [");
		Serial.print(threeThermometer_temp[0]);
		Serial.print(", ");
		Serial.print(threeThermometer_temp[1]);
		Serial.print(", ");
		Serial.print(threeThermometer_temp[2]);
		Serial.print(", ");
		Serial.println(threeThermometer_temp[3]);
		Serial.print("{'fourThermometer_temp': [");
		Serial.print(fourThermometer_temp[0]);
		Serial.print(", ");
		Serial.print(fourThermometer_temp[1]);
		Serial.print(", ");
		Serial.print(fourThermometer_temp[2]);
		Serial.print(", ");
		Serial.println(fourThermometer_temp[3]);
		Serial.print("], 'AM2302_temp': [");
		Serial.print(AM2302_temp[0]);
		Serial.print(", ");
		Serial.print(AM2302_temp[1]);
		Serial.print(", ");
		Serial.print(AM2302_temp[2]);
		Serial.print(", ");
		Serial.println(AM2302_temp[3]);
		Serial.print("], 'AM2302_humid': [");
		Serial.print(AM2302_humid[0]);
		Serial.print(", ");
		Serial.print(AM2302_humid[1]);
		Serial.print(", ");
		Serial.print(AM2302_humid[2]);
		Serial.print(", ");
		Serial.print(AM2302_humid[3]);
		Serial.println("]}");
		Serial.println();
		delay(DELAY);
	}
  clearScreen = 1;
  resetScreen();
  setFont(1);
  lcdSerial.print("Temp: ");
  lcdSerial.print(insideThermometer_temp1[0]);
  lcdSerial.print(",");
  lcdSerial.print(twoThermometer_temp[0]);
  lcdSerial.print(",");
  lcdSerial.print(threeThermometer_temp[0]);
  lcdSerial.print(",");
  lcdSerial.print(fourThermometer_temp[0]);
  lcdSerial.print(",");
  lcdSerial.print(waterThermometer_temp2[0]);
}
// {'C2': [20, 30, 40], 'C1': [11, 21, 31]}
// {insideThermometer_temp1: ['val', 'min', 'max', 'ave'], waterThermometer_temp2: ['val', 'min', 'max', 'ave']}


// Call out by number font size
// 1 = 4x16, 2 = 2x16, 3=4x8, 4=2x8, 5=1x8, 6=1x4
int setFont(int x){
  lcdSerial.write(0x03); // set font back to normal
  for (int i = 1; i < x; i++){
   lcdSerial.write(0x02); // Set font size one step larger (4x16->2x16->4x8->2x8->1x8->1x4)
  }
}

// reset serial screen to normal boot position
int resetScreen(){
    if (clearScreen == 1){ // only clear screen if first time through loop. 
      lcdSerial.write(0x0C); // clear screen
      clearScreen = 0;
    }
    lcdSerial.write(0x03); // set font back to normal
    lcdSerial.write(0x01); // Home
    
}
