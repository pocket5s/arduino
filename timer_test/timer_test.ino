#include <TimerOne.h>
 
const byte PWMDAC1pin = 9; // PWM DAC, only pins 9 and 10 are allowed
const byte period = 128; // for 8 bit DAC 
int outputVolume[] = {0,27,54,81, 108, 135, 162, 189, 216, 243, 270, 297, 324, 351, 378, 405, 432, 459, 486, 513, 540, 567, 594, 621, 648, 675, 702, 729, 756, 783, 810, 837, 864, 891, 918, 945, 972, 999, 1023};


void setup() 
{ 
  pinMode(PWMDAC1pin, OUTPUT);  
  Timer1.initialize(period); 
  Serial.begin(115200);
  delay(1000); 
}
 
void loop() 
{ 
  
    for(int i=0; i<39; i++)
    { 
      Serial.print( i );
      Serial.print("=");
      Serial.println( outputVolume[i] );
      Timer1.pwm(PWMDAC1pin, outputVolume[i]);
      
      delay(500);
    }
    
    Timer1.pwm(PWMDAC1pin, 0);
  delay(5000);
}
