
// Connect to Temp/humidity sensors and record data, print to serial
// Capacitor between GND and Reset to prevent usb connection reset

#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define ONE_WIRE_BUS 8  // Data wire is plugged into pin 8 on the Arduino

// how many sensor values to store for averaging. 
// 360 w/5000 delay = 1hr average
#define CNT 10
#define VAR 5 // last, min, max, ave, sum
#define DELAY 250

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

// AM2302 Sensor Connection
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// Initialize sensor arrays {sensorvalue, min, max, ave, sum, [readings]}
float temp1[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // inside OneWire
float temp2[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // outside OneWire
float temp3[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // inside AM2302(2)
float humid1[CNT+VAR] = {-99.0, 999.9, -999.9, -999.9}; // inside AM2302(2)

int i = VAR-1;  // starting point for where to store data in the array.
int first = 1;  // dont calculate average until 1 full set of array data is availabe.


void setup(void)
{
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  // set the resolution of the sensors to 12 bit
  sensors.setResolution(insideThermometer, 12);
  sensors.setResolution(waterThermometer, 12);
  dht.begin();
}

// pass array of a sensor to this function, records min or max value if logic says so. 
// setMinMax(temp1);
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
		temp1[0] = DallasTemperature::toFahrenheit(sensors.getTempC(insideThermometer));
		temp2[0] = DallasTemperature::toFahrenheit(sensors.getTempC(waterThermometer));
		temp3[0] = DallasTemperature::toFahrenheit(dht.readTemperature());
		humid1[0] = dht.readHumidity();
  
		setMinMax(temp1);
		setMinMax(temp2);
		setMinMax(temp3);
		setMinMax(humid1);

		getTemp(temp1,i);
		getTemp(temp2,i);
		getTemp(temp3,i);
		getTemp(humid1,i);
  
		Serial.print("{'Temp1': [");
		Serial.print(temp1[0]);
		Serial.print(", ");
		Serial.print(temp1[1]);
		Serial.print(", ");
		Serial.print(temp1[2]);
		Serial.print(", ");
		Serial.print(temp1[3]);
		Serial.print("], 'Temp2': [");
		Serial.print(temp2[0]);
		Serial.print(", ");
		Serial.print(temp2[1]);
		Serial.print(", ");
		Serial.print(temp2[2]);
		Serial.print(", ");
		Serial.print(temp2[3]);
		Serial.print("], 'Temp3': [");
		Serial.print(temp3[0]);
		Serial.print(", ");
		Serial.print(temp3[1]);
		Serial.print(", ");
		Serial.print(temp3[2]);
		Serial.print(", ");
		Serial.print(temp3[3]);
		Serial.print("], 'Humid1': [");
		Serial.print(humid1[0]);
		Serial.print(", ");
		Serial.print(humid1[1]);
		Serial.print(", ");
		Serial.print(humid1[2]);
		Serial.print(", ");
		Serial.print(humid1[3]);
		Serial.print("]}");
		Serial.println();
		delay(DELAY);
	}
}
// {'C2': [20, 30, 40], 'C1': [11, 21, 31]}
// {Temp1: ['val', 'min', 'max', 'ave'], Temp2: ['val', 'min', 'max', 'ave']}
