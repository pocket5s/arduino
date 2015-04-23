#include <TimerOne.h>
#include <SPI.h>

const byte PWMDAC1pin = 9; // PWM DAC, only pins 9 and 10 are allowed
//const byte PWMDAC2pin = 10; // example using second DAC
const byte period = 32; // for 8 bit DAC 

unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
unsigned char vol = 0;
const String VOL_CHANGE = "10-10-10-10-10-3-";
const String CD_PAUSED = "0-3-5-22-80-0-72-2-";
const String CD_PLAYING = "0-3-5-22-16-0-72-2-";
const String CD_TRACK_CHANGE = "0-3-5-22-128-0-72-2-";
const String RADIO_TRACK_CHANGE = "0-3-5-22-128-0-66-2-";
//int outputVolume[] = {0,7,14,21,28,35,42,49,56,63,70,77,84,91,98,105,112,119,126,133,140,147,154,161,168,175,182,189,196,203,210,217,224,231,238,245,250,255};

int outputVolume[] = {0,27,54,81, 108, 135, 162, 189, 216, 243, 270, 297, 324, 351, 378, 405, 432, 459, 486, 513, 540, 567, 594, 621, 648, 675, 702, 729, 756, 783, 810, 837, 864, 891, 918, 945, 972, 999, 1023};
char str[20];

void setup()
{

    pinMode(PWMDAC1pin, OUTPUT);  
    Timer1.initialize(period);
    Serial.begin(115200); 

}

void loop()
{
    //delay(10);
    
        String s = "0-";
        s = s + VOL_CHANGE;
        char incomingVol = vol;
        incomingVol = buf[0];
        //Serial.println(len);
        //Serial.println("---------------");
        for(int i = 0; i<len; i++)    // print the data
        {
          s = s + buf[i] + "-";
        }
        if( s.endsWith(VOL_CHANGE) ) {            
            processMessage( s );
        }
        else if( s == CD_TRACK_CHANGE ) {
          //Serial.println( "Track change" );
        }
        delay(2000);
}


void processMessage( String msg ) {
  Serial.println( msg );
  if( msg.endsWith( VOL_CHANGE ) ) {
    //Serial.println( "volume change" );
    int index = msg.indexOf("-");
    int value = msg.substring(0,index).toInt();
    //Serial.println( value );
    if( value != vol ) {
      vol = value;
      Serial.print( vol );
      Serial.print( "=" );
      Serial.println( outputVolume[vol] );
      Timer1.pwm(PWMDAC1pin, outputVolume[vol]);
    }
    
  }
}
