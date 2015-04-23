#include <mcp_can.h>
#include <mcp_can_dfs.h>

// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2013-11-27


#include <SPI.h>
#include "mcp_can.h"

String previous = "";
unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
unsigned char vol = 10;
unsigned char is_on = 0;
const String ON_OFF = "49-68-0-78-0-111-0-32-";
const String CD = "0-3-5-";
const String VOL_CHANGE = "10-10-10-10-10-3-";
const String CD_PAUSED = "0-3-5-22-80-0-72-2-";
const String CD_PLAYING = "0-3-5-22-16-0-72-2-";
const String CD_TRACK_CHANGE = "0-3-5-22-128-0-72-2-";
const String RADIO_TRACK_CHANGE = "0-3-5-22-128-0-66-2-";
int outputVolume[] = {0,7,14,21,28,35,42,49,56,63,70,77,84,91,98,105,112,119,126,133,140,147,154,161,168,175,182,189,196,203,210,217,224,231,238,245,250,255};
int pwmPin = 9;
char str[20];

void setup()
{
    pinMode( pwmPin, OUTPUT );
    Serial.begin(115200);

START_INIT:

    if(CAN_OK == CAN.begin(CAN_125KBPS))                   // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(5000);
        goto START_INIT;
    }
}


void loop()
{
    //delay(10);
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        String s = "";
        char incomingVol = vol;
        incomingVol = buf[0];
        //Serial.println(len);
        //Serial.println("---------------");
        for(int i = 0; i<len; i++)    // print the data
        {
          s = s + buf[i] + "-";
        }
        /*
        if( s.startsWith( CD ) ) {
           Serial.print( "CD DID SOMETHING: ");
           Serial.println( s );
        }
        */
        if( s.startsWith( "0-13-5-22" ) ) {
          
           Serial.println( s ); 
        }
        /*
        if( s == ON_OFF ) {
          flipOnOff();
          if( is_on == 1 ) {
           Serial.println( "TURNED ON" ); 
           Serial.println( s );
          }
          else {
            Serial.println( "TURNED OFF" );
            Serial.println( s );
          }
           Serial.print( "PREVIOUS: " );
           Serial.println( previous );
        }
        */
        
        //previous = s;
        
        if( s.endsWith(VOL_CHANGE) ) {     
            //Serial.print( "VOLUME CHANGE: " );
            //Serial.println( s );
            //processMessage( s );
        }
        else if( s == CD_TRACK_CHANGE ) {
          Serial.println( "Track change" );
        }
    }
    else {
      //Serial.println("No message received");
    }
}

void processMessage( String msg ) {
  if( msg.endsWith( VOL_CHANGE ) ) {
    int index = msg.indexOf("-");
    int value = msg.substring(0,index).toInt();
    //Serial.println( value );
    if( value != vol ) {
      vol = value;
      
      //is_on = 1;
      Serial.print( vol );
      Serial.print( "=" );
      Serial.println( outputVolume[vol] );
      analogWrite( pwmPin, outputVolume[vol] );
    }
    
  }
}

  void flipOnOff() {
    if( is_on == 0 ) { is_on = 1; }
    else { is_on = 0; }  
  }



/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
