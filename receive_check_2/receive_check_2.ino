#include <mcp_can.h>
#include <SPI.h>

unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
char str[20];
unsigned char previous = 0;
MCP_CAN CAN(9);

void setup()
{
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
        for(int i = 0; i<len; i++)    // print the data
        {
            s = s + buf[i] + "\t";
            //Serial.print(buf[i]);Serial.print("\t");
            //Serial.println();
            //Serial.println( s );
        }
        if( previous == 1 ) {
           Serial.print( "Previous: " );
           Serial.println( s );
          previous = 0; 
        }
        if( s.startsWith( "0\t0\t40" ) == false &&
            s.endsWith( "0\t0\t1\t" ) == false &&
            s.endsWith( "0\t0\t0\t" ) == false &&
            s.startsWith("31\t111\t96\t") == false &&
            s.startsWith( "253\t" ) == false &&
            s.startsWith( "255\t" ) == false &&
            s.startsWith( "0\t13\t5" ) == false &&
            s != "3\t" &&
            s != "7\t237\t86\t0\t" &&
            s != "127\t127\t127\t127\t120\t7\t" &&
            s != "0\t114\t28\t0\t7\t255\t7\t255\t" &&
            s != "127\t127\t127\t127\t0\t15\t0\t0\t" &&
            s != "30\t5\t0\t0\t0\t0\t0\t0\t" &&
            s != "192\t0\t15\t0\t0\t0\t0\t" &&
            s != "48\t30\t2\t15\t0\t" &&
            s != "28\t0\t0\t0\t28\t0\t0\t0\t" &&
            s != "31\t111\t111\t255\t16\t0\t0\t11\t" &&
            s != "65\t112\t255\t11\t255\t36\t1\t0\t" &&
            s != "69\t2\t27\t184\t29\t0\t0\t0\t" &&
            s != "37\t8\t0\t" &&
            s != "200\t0\t2\t" &&
            s != "49\t41\t98\t16\t255\t15\t0\t0\t" &&
            s != "17\t0\t0\t0\t0\t0\t0\t0\t" &&
            s != "1\t10\t10\t10\t10\t10\t3\t" &&
            s != "1\t5\t22\t16\t0\t66\t2\t"	&&
            s != "36\t41\t20\t0\t0\t0\t0\t1\t" &&
            s != "31\t111\t111\t255\t48\t0\t0\t11\t" &&
            s != "0\t117\t28\t0\t7\t255\t7\t255\t" &&
s != "192\t0\t15\t0\t0\t0\t9\t" &&	
s != "2\t44\t20\t0\t0\t0\t0\t1\t" &&
s != "3\t44\t20\t0\t0\t0\t0\t1\t" &&
s != "31\t111\t111\t255\t64\t0\t0\t11\t" &&
s.endsWith("197\t0\t0\t0\t") == false &&
s.startsWith("192\t0\t15") == false &&
s != "31\t111\t111\t255\t16\t0\t0\t93\t" &&
s != "0\t3\t5\t22\t16\t0\t72\t2\t" &&
s != "0\t112\t28\t0\t7\t255\t7\t255\t" &&
s.startsWith( "31\t111\t" ) == false &&
//s.startsWith( "0\t13\t5" ) == false &&
s.startsWith( "12\t9\t152" ) == false &&
s.startsWith( "0\t108\t28" ) == false &&
s.startsWith( "10\t10\t10" ) == false &&
s.endsWith( "239\t240\t0\t") == false &&
s.startsWith( "33\t80" ) == false &&
s.startsWith( "17\t16" ) == false &&
s.startsWith( "1\t4\t" ) == false &&
s.startsWith( "1\t16\t" ) == false &&
s.startsWith( "10\t10\t10" ) == false &&
s.startsWith( "0\t109\t") == false &&
s.startsWith( "0\t50\t") == false &&
s.startsWith( "2\t169\t") == false &&
s.startsWith( "2\t193\t") == false &&
s.startsWith( "2\t") == false &&
s.startsWith( "0\t128\t") == false &&
s.startsWith( "1\t84\t") == false &&
s.startsWith( "12\t97\t") == false &&
s.startsWith( "5\t10\t10" ) == false &&
s.startsWith( "12\t125\t235" ) == false &&
s.startsWith( "33\t6\t0" ) == false &&
s.startsWith( "49\t68" ) == false && // 1st
s.startsWith( "17\t6" ) == false && // after volume
s.startsWith( "49\t70" ) == false && // after volume
s.startsWith( "33\t4" ) == false && // after volume
s.startsWith( "1\t6" ) == false && // after volume
s.endsWith( "10\t10\t3\t" ) == false && // volume amount
s.startsWith("0\t1\t1\t0\t") == false // don't show track #


            ) {
        
              if( s == "17\t4\t0\t101\t0\t116\t0\t111\t" ) {
                previous = 1;
                //Serial.println( "Set previous" );
              }
                  Serial.println( s );
                  //Serial.println();
        }
    }
    else {
      //Serial.println("No message received");
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
