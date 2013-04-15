const float referenceVolts = 5.02; // measured ref voltage

const float R1 = 3022; //measured resistance
const float R2 = 1001; //measured resistance
// determine by voltage divider resistors, see text
// http://arduinotronics.blogspot.com/2011/03/monitoring-voltage-of-dc-battery-supply.html


const float resistorFactor = 255 / (R2/(R1 + R2)); 
const int batteryPin = 0; // +V from battery is connected to analog pin 0
const float multiplier = (R1+R2)/R2;  // to return to normal voltage
void setup()
{
Serial.begin(9600);
}

void loop()
{
int val = analogRead(batteryPin); // read the value from the sensor
float volts = (val / resistorFactor) * referenceVolts ; // Calculate stepped down voltage input
float volts2 = (volts * multiplier);  //need to multiply to get real voltage + add calculated correction
Serial.println(volts2);
delay(250);
}
