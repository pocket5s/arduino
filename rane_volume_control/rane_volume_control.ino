#include <mcp_can.h>
#include <TimerOne.h>
#include <SPI.h>

int amps_relay = 4;
int rane_relay = 5;
int led_relay = 6;
int mute_relay = 7;
MCP_CAN CAN(9);

const byte PWMDAC1pin = 10; // PWM DAC, only pins 9 and 10 are allowed
//const byte PWMDAC2pin = 10; // example using second DAC
const byte period = 32; // for 8 bit DAC 
int no_msg_counter = 0;
unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
unsigned char vol = 0;
boolean system_on = false;
boolean leds_on = false;
boolean muted = false;
const String HU_OFF = "0-122-2-";
//const String HU_OFF = "-240-0-";
//0-13-5-22-16-0-102-2-  HU ON?
//0-13-5-22-240-0-126-2- Interior lights/guages on
// HU off       while running?
//const String HU_ON = "0-13-1-22-240-0-";
const String HU_ON = "22-240-0";
const String AUX = "0-76-2-";
const String VOL_CHANGE = "10-10-10-10-10-";
const String CD_PAUSED = "0-3-5-22-80-0-72-2-";
const String CD_PLAYING = "0-3-5-22-16-0-72-2-";
const String CD_TRACK_CHANGE = "0-3-5-22-128-0-72-2-";
const String RADIO_TRACK_CHANGE = "0-3-5-22-128-0-66-2-";
// 5-2-13-0-117-0 scroll up track list
// 5/0-2-24-1-29-0 scroll down track list
//int outputVolume[] = {0,7,14,21,28,35,42,49,56,63,70,77,84,91,98,105,112,119,126,133,140,147,154,161,168,175,182,189,196,203,210,217,224,231,238,245,250,255};

//int outputVolume[] = {0,27,54,81, 108, 135, 162, 189, 216, 243, 270, 297, 324, 351, 378, 405, 432, 459, 486, 513, 540, 567, 594, 621, 648, 675, 702, 729, 756, 783, 810, 837, 864, 891, 918, 945, 972, 999, 1023};
int outputVolume[] = {0,10,27,54, 108, 135, 162, 189, 216, 243, 270, 297, 324, 351, 378, 405, 432, 459, 486, 513, 540, 567, 594, 621, 648, 675, 702, 729, 756, 783, 810, 837, 864, 891, 918, 945, 972, 999, 1023};

char str[20];

void setup()
{
    //pinMode(2, INPUT);
    pinMode(PWMDAC1pin, OUTPUT);  
    pinMode(amps_relay, OUTPUT);
    pinMode(rane_relay, OUTPUT);
    pinMode(led_relay, OUTPUT);
    pinMode(mute_relay, OUTPUT);
    Timer1.initialize(period);
    Serial.begin(115200); 

START_INIT:

    if(CAN_OK == CAN.begin(CAN_125KBPS))  // init can bus : baudrate = 125k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(4000);
       
        goto START_INIT;    
    }
    
}

void loop()
{   
   
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        no_msg_counter = 0;
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        String s = "";
        for(int i = 0; i<len; i++) {
          s = s + buf[i] + "-";
        }
        //Serial.println( s );
        if( isVolumeChange(s) ) {
            changeVolume( parseVolume(s) );
        }
        else if( s == CD_TRACK_CHANGE ) {
          //Serial.println( "Track change" );
        }
        else if( isSourceOff( s ) ) {
          // The HU_OFF signal gets sent constantly
          // while it is off, so only flip the switch
          // if needed
          //Serial.println( "OFF: " + s );
          if( system_on ) {
            
            turnSystemOff();
          } 
          //Serial.print( "OFF: " );
          //Serial.println( s );
        }
        else if( isSourcePlaying( s ) ) {
          //Serial.print( "ON: " );
          //Serial.println( s );
          turnSystemOn(); 
        }
        else if( s.endsWith( AUX ) ) {
          if( !leds_on ) {
            turnLEDsOn();
           }
           else {
             if( leds_on ) {
               turnLEDsOff(); // makes sure they are off
             } 
           }
        }
        //0-13-5-22-240-0-110-2-   BLUETOOTH
        //0-13-5-22-240-0-72-2-    DISC
        //0-13-5-22-240-0-66-2-    RADIO
        //0-13-5-22-240-0-82-2-    SATTELITE
        //0-13-5-22-240-0-102-2-   IPOD
        //0-13-3-22-16-0-76-2-     AUX

        //31-111-xx-xxx-16-0-0-x     // command sequence
        //31-111-31-255-16-0-0-1-    // off
        //31-111-31-255-16-0-0-118-  // running ipod
        //31-111-50-0-16-0-0-118-    // acc ipod
        //31-111-31-255-16-0-0-11-   // running radio
        //31-111-31-255-64-0-0-11-   // acc radio
        //31-111-31-255-16-0-0-81-   // disc
        //31-111-16-0-16-0-0-0-   // opened door
        //31-111-16-0-16-0-1-251- // about to be off 
        //31-111-111-255-48-0-81/11  source switch
        
        //Serial.println( s );
    }
    else {
      no_msg_counter++;
      if( no_msg_counter > 5000 ) {
        //if( system_on ) {
          Serial.println( "No activity, turning off system" );
          turnSystemOff();
        //}
        no_msg_counter = 0;
      }
    }
}

boolean isSourceOff( String msg ) {
  if( msg.startsWith( "31-111" ) && msg.endsWith( "16-0-0-1-" ) ) {
    return true;
  }  
  else if( msg.startsWith( "31-111" ) && msg.endsWith( "16-0-1-251" ) ) {
     return true; 
  }
  else {
    return false;
  }
}

boolean isSourcePlaying( String msg ) {
  //Serial.println( msg );
  //if( msg.startsWith( "31-111" ) && msg.indexOf( "16-0-0" ) > 7 ) {
  if( msg.startsWith( "31-111" ) ) {
    //Serial.print( "source on: " );
    //Serial.println( msg );
    
    return true;
  } 
  else {
    return false;
  }
}

boolean isVolumeChange( String msg ) {
  return msg.indexOf( VOL_CHANGE ) > 1; 
}

int parseVolume( String msg ) {
  int index = msg.indexOf("-");
  int value = msg.substring(0,index).toInt();
  //Serial.println( "Volume " + value );
  return value;
}

void changeVolume( int value ) {
  if( system_on ) {
    //Serial.println( "volume change" );
    //Serial.println( value );
    if( value != vol ) {
      vol = value;
      /*
      Serial.print( vol );
      Serial.print( "=" );
      Serial.println( outputVolume[vol] );
      /**/
      if( vol == 0 ) {
        Timer1.pwm(PWMDAC1pin, 1);
        mute(); 
      }
      else {
        unMute();
        Timer1.pwm(PWMDAC1pin, outputVolume[vol]);
      }
    }
  }
}

void mute() {
  digitalWrite( mute_relay, HIGH );
  muted = true;
  //Serial.println( "Muted" );
}

void unMute() {
  if( muted ) {
    //Serial.println( "Un-muting" );
    digitalWrite( mute_relay, LOW );
    muted = false;
  }
}

void turnSystemOn() {
  if( !system_on ) {
    Serial.println( "Turning system on" );
    // Turn on Rane
    digitalWrite( rane_relay, HIGH );
    delay( 3000 );
    // Turn on amps
    digitalWrite( amps_relay, HIGH );
    system_on = true;
   } 
}

void turnSystemOff() { 
  
  //if( system_on ) {
    Serial.println( "Turning system off" );
    // Set volume to zero
    changeVolume( 0 );
    // Turn off amps
    digitalWrite( amps_relay, LOW );
    delay( 1000 );
    // Turn off rane
    digitalWrite( rane_relay, LOW );
    system_on = false;
    turnLEDsOff();
   //}
  
}

void turnLEDsOff() {
 if( leds_on ) {
    digitalWrite( led_relay, LOW );
    leds_on = false;
    Serial.println( "Turned LEDs off" );
  }  
}

void turnLEDsOn() {
  if( !leds_on ) {
    digitalWrite( led_relay, HIGH );
    leds_on = true;
    Serial.println( "Turned LEDs on" ); 
  }
}
