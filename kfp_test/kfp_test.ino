/**
 * \file kfp_test.ino
 * \brief Arduino program for testing KFP protocol bluetooth transmitters 
 * \author George Xian
 * \version 0.1
 * \date 2014-12-16
 */

#include <SoftwareSerial.h>
#include "BtStackTypes.h"

// Change these values below to your liking to select correct baud rates
#define PC_SERIAL_BAUD 115200
#define BT_SERIAL_BAUD 9600

// Hardware descriptors
SoftwareSerial BTSerial(10, 11);   // RX, TX

// Reception variables
BtStack_Frame recvFrame;
bool inFrame = false;
uint8_t frIndex = 0;

// Main program
void setup() 
{
  Serial.begin(PC_SERIAL_BAUD);    // serial link to PC
  BTSerial.begin(BT_SERIAL_BAUD);  // serial link to BT module
  
  Serial.println("Welcome to the KFP tester");
}

void loop() 
{
  int8_t incoming;
  
  if (BTSerial.available())
  {
    incoming = BTSerial.read();
    switch(incoming)
    {
      case(SLIP_END):
        if (inFrame)
        {
          if (frIndex == (KFP_FRAME_SIZE-2))
          {
            // end of frame
            BtStack_framePrint(&recvFrame, KFPPRINTFORMAT_HEX);
          }
          
          // reset states, ignore corrupt frames
          frIndex = 0;
          inFrame = false;
          break;
        }
        else
        {
          // start a new frame
          inFrame = true;
          frIndex = 0;
          break;
        }
      case(SLIP_ESC):
        if (inFrame)
        {
          incoming = BTSerial.read();
          switch (incoming)
          {
            case(SLIP_ESC_END):
              recvFrame.b8[frIndex] = SLIP_END;
              frIndex++;
              break;
            case(SLIP_ESC_ESC):
              recvFrame.b8[frIndex] = SLIP_ESC;
              frIndex++;
              break;
            default:
              break;  // invalid post escape character
          }
          break;
        }
        else
        {
          break;      // ignore corrupt frames
        }
      default:
        if (inFrame)
        {
          // standard character store
          recvFrame.b8[frIndex] = incoming;
          frIndex++;
          //Serial.print((char) recvFrame.b8[frIndex]);
        }
        break;      
    }
  }
}

void BtStack_framePrint(const BtStack_Frame* frame, KfpPrintFormat format)
{
	Serial.print("\nComplete Frame: ID = ");
	uint8_t i;
	for (i=0; i<4; i++)
	{
		Serial.print(frame->id.b8[i]);
                Serial.print(" ");
	}

	Serial.print(" Data = ");
	for (i=0; i<8; i++)
	{
		if (format == KFPPRINTFORMAT_ASCII)
		{
			Serial.print((char) frame->payload.b8[i]);
                        Serial.print(" ");
		}
		else if (format == KFPPRINTFORMAT_HEX)
		{
			Serial.print(frame->payload.b8[i], HEX);
                        Serial.print(" ");
		}
	}
	Serial.print("\n");
}
