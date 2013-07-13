#include <SoftwareSerial.h>

SoftwareSerial lcdSerial(10, 11, 1); // RX, TX
#define BUTTON1 8
#define BUTTON2 9  
int button1State;
int button2State;
int lastButton1State = LOW;
int lastButton2State = LOW;

void setup()  
{
  pinMode(BUTTON1, INPUT); 
  pinMode(BUTTON2, INPUT); 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // set the data rate for the SoftwareSerial port
  lcdSerial.begin(9600);
  delay(1000);
  resetScreen();
  delay(1000);
}

void loop() // run over and over
{
  lcdSerial.write(Serial.read());
  button1State = digitalRead(BUTTON1);
  button2State = digitalRead(BUTTON2);

  if(button1State != lastButton1State){ //compare increment button state to its last state
    if(button1State == LOW){ //increment button is pressed
        Serial.println("1");
    }
  }
  if(button2State != lastButton2State){ //compare increment button state to its last state
    if(button2State == LOW){ //increment button is pressed
        Serial.println("2");
    }
  }
  lastButton1State = button1State;
  lastButton2State = button2State;
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
