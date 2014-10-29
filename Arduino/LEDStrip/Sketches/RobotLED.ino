#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>         
#include "RobotLedPacket.h"


//************************************************//
//*  Network Setup.                              *//
//************************************************//
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 43, 29, 12);
IPAddress bip(255,255,255,255);
unsigned int localPort = rLED_Port;

// Network UDP Socket Interface.
EthernetUDP Udp;

//************************************//
//* Preallocated Storage for network *//
//* Message Buffers.                 *//
//************************************//
rLedCmd  rxCmd;
rLedResp txResp;
rLedAnn  txAnn;

// Network Message Index Counter.
unsigned long msgCount = 0;

// This is the id number of the robot.  [For future potential use]
const unsigned int RobotId = 1;

//*************************//
//* IO Pin Setup          *//
//*************************//
const int clkPin = 4;    // Clock Pin to LED Strip      -- CLK --
const int mdiPin = 2;    // Data Input Pin to LED Strip -- Di --
const int ledPin = 13;   // Arduino Ethernet on-Board LED.


//***************************************//
//* General LED Strip Configuration.    *//
//***************************************//

const int LEDCnt = 55;                          // Number of LEDs to be addressed.
const byte BitsPerColor = 8;                    // Color Depth.
const byte ColorsPerLED = 3;                    // Colors per LED -- RGB.
byte BitsPerLED = BitsPerColor * ColorsPerLED;  // Total Bits to Configure each LED.

const unsigned int LED_Reset_usec = 850;        // Amount of Time Required For Which
                                                // CLK Must Stay Low Before Starting
                                                // Next LED Strip Update. <no less than 500>


//**************************************//
//* Definitions of Robot LED segments. *//
//**************************************//
//************** Chuck *****************//

// Left Vertical Segment.
byte Low_Left_Leg_LED = 0;
byte High_Left_Leg_LED = 14;

// Left Upper Segment.
byte Low_Seg1_LED  = 15;
byte High_Seg1_LED = 22;

// Upper Horizontal Segment.
byte Low_Seg2_LED  = 23;
byte High_Seg2_LED = 30;

// Right Upper Segment.
byte Low_Seg3_LED  = 31;
byte High_Seg3_LED = 38;

// Right Vertical Segment.
byte Low_Right_Leg_LED = 39;
byte High_Right_Leg_LED = 54;

// Left Upper Segment Size Calculation.
byte Seg1_LED_Size = High_Seg1_LED - Low_Seg1_LED + 1;
// Left Upper Segment Single LED Percentage of Total Segment.
byte Seg1_LED_Pct = (100 - (100%Seg1_LED_Size))/Seg1_LED_Size;

// Upper Horizontal Segment Size Calculation.
byte Seg2_LED_Size = High_Seg2_LED - Low_Seg2_LED + 1;
// Upper Horizontal Segment Single LED Percentage of Total Segment.
byte Seg2_LED_Pct = (100 - (100%Seg2_LED_Size))/Seg2_LED_Size;

// Right Upper Segment Size Calculation.
byte Seg3_LED_Size = High_Seg3_LED - Low_Seg3_LED + 1;
// Right Upper Segment Single LED Percentage of Total Segment.
byte Seg3_LED_Pct = (100 - (100%Seg3_LED_Size))/Seg3_LED_Size;


//****************************************//
//*  Basic Color Definitions.            *//
//****************************************//
const unsigned long Black   = 0x00000000;
const unsigned long Blue    = 0x000000FF; 
const unsigned long Lime    = 0x0000FF00; 
const unsigned long Cyan    = 0x0000FFFF;
const unsigned long Red     = 0x00FF0000;
const unsigned long Magenta = 0x00FF00FF;
const unsigned long Yellow  = 0x00FFFF00;
const unsigned long Orange  = 0x00FFA500;
const unsigned long Silver  = 0x00C0C0C0;
const unsigned long Gray    = 0x00808080;
const unsigned long Maroon  = 0x00800000;
const unsigned long Olive   = 0x00808000;
const unsigned long Green   = 0x00008000;
const unsigned long Purple  = 0x00800080;
const unsigned long Teal    = 0x00008080;
const unsigned long Navy    = 0x00000080;
const unsigned long White   = 0x00FFFFFF;


// Color Sequences.
const unsigned long CSequence[11] = {Red,Maroon,Orange,Yellow,Lime,Green,Olive,Teal,Navy,Blue,Purple};
const unsigned long CBLKSequence[6] = {Black,Red,Maroon,Olive,Navy,Blue};


// Array Containing RGB Settings For Each LED to be Programmed to the Strip by SetLEDStrip(). 
unsigned long colors[LEDCnt];

// Current Global Color. [To Be Removed]
unsigned long currentC = Red;

//********************************************************//
//* Parameters for Configuring Drive Train LED Segments. *//
//********************************************************//
// Right.
unsigned long RDTc;  // Primary Color.
unsigned long RDTc1; // Alternate Color.
unsigned int  RDTl;  // Number of LEDs to Light.

// Left
unsigned long LDTc;  // Primary Color.
unsigned long LDTc1; // Alternate Color.
unsigned int  LDTl;  // Number of LEDs to Light.

// Team Color.
unsigned long TeamC = Red;   // RGB color.
byte TC = 1;                 // Index into Color Sequence.
boolean TeamSet = false;     // Flag Indicating That Team Color Has Been Set.

// This is stores the starting bit mask value for use when setting LED values. 
const unsigned long maskSave = 0x00800000;

// Flag indicating if the arduino has been discovered by the controlling node.
boolean found = false;

//***************************************************************************//
//* Function: setup                                                         *//
//* Description: One shot setup routine called to initialize the Sketch.    *//
//* Arguments: None                                                         *//
//* Return Value: N/A                                                       *//
//***************************************************************************//
void setup() {
  // Pre-Load some protocol message values to save time in the runtime loop.
  txAnn.header.Robot_Id = RobotId;
  txAnn.header.Message_Type = 0x03;
  strcpy(txAnn.TeamName,"LHS Roboteers");
  
  txResp.header.Robot_Id = RobotId;
  txResp.header.Message_Type = 0x02;
  
  // Prepare the network.
  //Ethernet.begin(mac);  // For DHCP
  Ethernet.begin(mac,ip); // For fixed IP.
  Udp.begin(localPort);

  // Setup IO Pins.
  pinMode(mdiPin, OUTPUT);
  pinMode(clkPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  
  for (int i = 0; i < 5; i++)
  {
     digitalWrite(ledPin, HIGH);
     delay(500);
     digitalWrite(ledPin, LOW);
     delay(500); 
  }
  
  // For USB debugging.
  Serial.begin(115000); 
}


//***************************************************************************//
//* Function: loop                                                          *//
//* Description: Main Processing Loop for the Sketch.                       *//
//* Arguments: None                                                         *//
//* Return Value: N/A                                                       *//
//***************************************************************************//
void loop() {

  // Reset the LED Strip to All LEDs OFF.
  LEDReset();
  delay(100);

  // Show Power On Init Sequence To Visually Verify Program Startup.  
  PowerOnInit();

  // Do this forever, or until the power is turned off.
  while(true)
  {
     // Determine if there is a network packet to look at.
     int packetSize = Udp.parsePacket();
     
     // If so, process it.
     if(packetSize)
     {
         ProcessNetPacket();
         found = true;    // Controlling node has received an announcement.
     } 
     
     // If a controlling node has not sent a command, continue sending announcement packets.
     if (!found)
     {
       SendAnnouncement();
       delay(500);  // Wait for good measure.
     }
     
     // Perform two drive train updating calls to allow for simple flashing.
     Robot_DriveTrain_LEDS(100,1,LDTc, LDTc1,RDTc,RDTc1);   // Primary.
     Robot_DriveTrain_LEDS(50,1,LDTc1,LDTc, RDTc1,RDTc);    // Alternate. 
  }
}


//***************************************************************************//
//* Function: PowerOnInit                                                   *//
//* Description: Controls what is displayed for visual confirmation of      *//
//*              loop start.                                                *//
//* Arguments: None                                                         *//
//* Return Value: N/A                                                       *//
//***************************************************************************//
void PowerOnInit()
{
    ProgSweepLED(1,true, 0, 5);   // Forward Progressive Color Sweep between color sequence index 0 and 5.
    ProgSweepLED(1,false, 0, 5);  // Reverse Progressive Color Sweep between color sequence index 0 and 5.
}


//******************************************************************************//
//* Function: SweepLED                                                         *//
//* Description: Creates the appearance of a single LED chasing up and down    *//
//*              the entire LED Strip.                                         *//
//* Arguments: Hold - time in millisecs to delay after setting the LED strip.  *//
//*            color - the RGB color for the sweeping LED.                     *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void SweepLED(int hold, unsigned int color)
{
    // Sweep down
    for (int i = 0; i < LEDCnt; i++)
    { 
       // Set all other LEDs to black.
       for (int j = 0; j < LEDCnt; j++)
       {
           colors[j] =  Black;
       }
       // Set the color on the sweeping LED's current position.
       colors[i] = color;
       // Program the strip.
       SetLEDStrip(hold);
    }
    
    // Sweep back
    for (int i = (LEDCnt -1 ); i >= 0; i--)
    { 
       // Set all other LEDs to black.
       for (int j = 0; j < LEDCnt; j++)
       {
           colors[j] =  Black;
       }
       // Set the color on the sweeping LED's current position.       
       colors[i] = color;
       // Program the strip.
       SetLEDStrip(hold);
    }
}

//******************************************************************************//
//* Function: ProgSweepLED                                                     *//
//* Description: Creates the appearance of colors chasing up or down           *//
//*              the entire LED Strip.                                         *//
//* Arguments: Hold - time in millisecs to delay after setting the LED strip.  *//
//*            forward - true if sweeping forward, false if backward.          *//
//*            StartC - Starting Color index in the sequence.                  *//
//*            StopC - Ending color index in the sequence.                     *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void ProgSweepLED(int hold, boolean forward, unsigned int StartC, unsigned int StopC)
{
    unsigned int scolors[LEDCnt];

// Clear color array and initialize the sweep array.
    for (int j = 0; j < LEDCnt; j++)
    {
        colors[j] =  Black;

// When sweeping forward start with the starting color.
        if (forward)
        {
            scolors[j] = StartC;
         } else // otherwise start with the ending color.
        {
            scolors[j] = StopC;
        }
    }

    // if going forward in direction.
    if (forward)
    {  
       // This just ensures the correct number of loops are executed to get al the desired colors.
       for (int k = StartC; k <= StopC; k++)   
       { 
          // make the sweep progress through each LED individually.
          for (int i = 0; i < LEDCnt; i++)
          { 
             colors[i] = CBLKSequence[scolors[i]];  // Set this LED to the color corresponding to the index.
             scolors[i] = scolors[i] + 1;  // increment the color index.
             SetLEDStrip(hold);   // Set the strip.
          }
       }
    } else
    {  // This just ensures the correct number of loops are executed to get al the desired colors.
       for (int k = StartC; k <= StopC; k++)
       {  // make the sweep progress through each LED individually in the reverse direction.
          for (int i = (LEDCnt - 1); i >= 0; i--)
          { 
             colors[i] = CBLKSequence[scolors[i]];  // Set this LED to the color corresponding to the index.
             scolors[i] = scolors[i] - 1;  // decrement the color index.
             SetLEDStrip(hold);  // Set the strip.
          }
       }      
    }
}

//******************************************************************************//
//* Function: SkipLED                                                          *//
//* Description: Programs LEDs on the strip at specified skip distances        *//
//* Arguments: Hold - time in millisecs to delay after setting the LED strip.  *//
//*            skip - the number of LEDs to skip before setting a color.       *//
//*            color - the color to set the LED to.                            *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void SkipLED(int hold, int skip, unsigned int color)
{
    // Set the LEDS to black.
    for (int j = 0; j < LEDCnt; j++)
    {
       colors[j] =  Black;
    }

    // Set LEDs at the specified skip distance to color.
    for (int i = 0; i < LEDCnt; i = i + skip)
    { 
       colors[i] = color;
    }
    SetLEDStrip(hold);  // Set the strip.
}


//******************************************************************************//
//* Function: SetLEDStrip                                                      *//
//* Description: Programs each LED on the strip using the colors array.        *//
//* Arguments: Hold - time in millisecs to delay after setting the LED strip.  *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void SetLEDStrip(int hold) {

  unsigned long mask;  // Holds the current bit mask.
  
  // For every LED on the strip.
  for (int i = 0; i < LEDCnt; i++)
  {
    // unsigned long this_led_color = colors[i];
    //this_led_color = colors[i];    
    
    // Reset current bit mask to the highest order bit.
    mask = maskSave;

    // program each RGB bit for this LED from highest order to lowest order bit.
    for (byte color_bit = 0; color_bit < (byte) BitsPerLED; color_bit++)
    {
     // If this isnt the last bit, bit shift the mask to the right by 1 bit position.
     if (color_bit != 0)
     {
        mask = mask >> 1; // bit shift to the right.
     }
     
//     if ((this_led_color & mask) != 0)

     // Bitwise compare the bit mask and RGB value for this LED and....
     if ((colors[i] & mask) != 0)     
       digitalWrite(mdiPin,HIGH);  // If the bit comparison finds a 1 bit then set the data pin high.
      else
       digitalWrite(mdiPin,LOW);   // if the bit comparison finds a 0 bit set the data pin low.

     digitalWrite(clkPin,HIGH);    // Raise the clock pin to tell the LED Strip chip to latch the bit.

     digitalWrite(clkPin, LOW);    // Lower the clock pin to get ready for the next bit.
    }
  }
    digitalWrite(clkPin,LOW);           // Ensure the clock pin is in low state.
    delayMicroseconds(LED_Reset_usec);  // Wait long enough to allow the LED Strip chip to detect this strip update has completed.
    
    delay(hold);  // Wait before continuing. 
}

//******************************************************************************//
//* Function: LEDReset                                                         *//
//* Description: Set LED strip to all off.                                     *//
//* Arguments: N/A                                                             *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void LEDReset()
{
  // For each LED on the strip.
  for (int i = 0; i < LEDCnt; i++)
  {
     // for each RGB bit on the LED
     for (int r = 0; r < BitsPerLED; r++)
     {
        digitalWrite(mdiPin,LOW);    // set data pin low
        digitalWrite(clkPin,HIGH);   // strobe the clock
        digitalWrite(clkPin, LOW);
     }
  }
  
  digitalWrite(clkPin,LOW);            // set clock to low.
  delayMicroseconds(LED_Reset_usec);   // Reset LED strip program cycle.
}


//******************************************************************************//
//* Function: ProcessNetPacket                                                 *//
//* Description: Processes a packet from the network.                          *//
//* Arguments: N/A                                                             *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void ProcessNetPacket()
{
  IPAddress remote = Udp.remoteIP();  // gets the souce IP address from the packet. Not used.

  // Read the raw payload bytes into the preallocated cmd message structure.
  Udp.read((char *) &rxCmd, sizeof(rLedCmd));

  // Team color is valid
  if (Set_Team(rxCmd.TeamColor))
  {
        ShowTeamColor();  // show the team color.
  } 

  // Set the drive train speed and direction from the values in the packet.
  Set_LeftDriveTrain(rxCmd.LeftDriveTrainPercent);
  Set_RightDriveTrain(rxCmd.RightDriveTrainPercent);
  // Set the status from the packet values.
  Set_Status(rxCmd.CodeStatus, rxCmd.ErrorCode);
  SetLEDStrip(0); // Set the strip.

  // Prepare the acknowledge packet info.
  msgCount++;
  txResp.header.Message_Id = msgCount;  // local message id.
  txResp.Acked_Message_Id = rxCmd.header.Message_Id;  // message being acknoledged.
  
  // Send Acknowledge.  
  Udp.beginPacket(bip, localPort);
  Udp.write((byte *) &txResp, sizeof(txResp));
  Udp.endPacket();

}

//******************************************************************************//
//* Function: SendAnnouncement                                                 *//
//* Description: Announces existence on the network.                           *//
//* Arguments: N/A                                                             *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void SendAnnouncement()
{
  // Setup endpoint info.
  Udp.beginPacket(bip, localPort);
  
  // Set team color.
  txAnn.TeamColor = TeamC;   
  
  // Set next local message id.
  msgCount++;  
  txAnn.header.Message_Id = msgCount;
  
  // Write the packet to the network.
  Udp.write((byte *) &txAnn, sizeof(txAnn));
  Udp.endPacket();
}


//******************************************************************************//
//* Function: Set_RightDriveTrain                                              *//
//* Description: Configures the LED parameters to reflect RDT status.          *//
//* Arguments: RightDriveTrainPercent [0 - 100 forward 101 - 200 backward      *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void Set_RightDriveTrain(byte RightDriveTrainPercent)
{
   // Set all RDT LEDS to black (OFF).
   for(int i = Low_Seg3_LED; i <= High_Seg3_LED; i++) colors[i] = Black;
   
   // If moving
   if (RightDriveTrainPercent != 0)
   { 
      // in a positive (forward) direction
      if (RightDriveTrainPercent <= 100)
      {
         RDTc = Green;  // Primary color
         RDTc1 = Navy;  // Secondary color
         // Calculate # of LEDs to light:
         //   ->| Ensure Numerator can be divided by denominator without 
         //                                     having a fractional component. |<-
         RDTl = (RightDriveTrainPercent - (RightDriveTrainPercent%Seg3_LED_Pct))/Seg3_LED_Pct;      
      } else // in a negative (backward) direction
      {
         RDTc = Purple;  // Primary color
         RDTc1 = Yellow; // Secondary color
         
         // Calculate # of LEDs to light:
         //   ->| Ensure Numerator can be divided by denominator without having a fractional component. |<-         
         RDTl = ((RightDriveTrainPercent - 100)   -   ((RightDriveTrainPercent - 100)%Seg3_LED_Pct))    / Seg3_LED_Pct;           
         //    ->| Remove (backward) Bias     |<-    ->| Remove (backward) Bias     |<-  
      }
   } else  // not moving
   {
       RDTc = Black;          // Primary Color (OFF)
       RDTc1 = Black;         // Secondary Color (OFF)
       RDTl = Seg3_LED_Size;  // All LEDS
   }
}


//******************************************************************************//
//* Function: Set_LeftDriveTrain                                               *//
//* Description: Configures the LED parameters to reflect LDT status.          *//
//* Arguments: LeftDriveTrainPercent [0 - 100 forward 101 - 200 backward       *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void Set_LeftDriveTrain(byte LeftDriveTrainPercent)
{
   // Set all LDT LEDS to black (OFF).  
   for(int i = Low_Seg1_LED; i <= High_Seg1_LED; i++) colors[i] = Black;
   
   // If moving   
   if (LeftDriveTrainPercent != 0)
   {  // in a positive (forward) direction
      if (LeftDriveTrainPercent <= 100)
      {
         LDTc = Green;  // Primary color
         LDTc1 = Navy;  // Secondary color
         
         // Calculate # of LEDs to light:
         //   ->| Ensure Numerator can be divided by denominator without 
         //                                     having a fractional component. |<-         
         LDTl = (LeftDriveTrainPercent - (LeftDriveTrainPercent%Seg1_LED_Pct)) / Seg1_LED_Pct;      
      } else // in a negative (backward) direction
      {
         LDTc = Purple;   // Primary color
         LDTc1 = Yellow;  // Secondary color

         // Calculate # of LEDs to light:
         //   ->| Ensure Numerator can be divided by denominator without having a fractional component. |<-                  
         LDTl = ((LeftDriveTrainPercent - 100)   -   ((LeftDriveTrainPercent - 100)%Seg1_LED_Pct))      / Seg1_LED_Pct;           
         //    ->| Remove (backward) Bias    |<-    ->| Remove (backward) Bias    |<-           
      }
   } else
   {
       LDTc = Black;          // Primary Color (OFF)
       LDTc1 = Black;         // Secondary Color (OFF)
       LDTl = Seg1_LED_Size;  // All LEDS
   }
}

//******************************************************************************//
//* Function: Set_Status                                                       *//
//* Description: Intended to be used to display limited status on the LED      *//
//* Arguments: CodeStatus - will represent status of executing code            *//
//*            ErrorCode  - will represent some error conditions               *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void Set_Status(byte CodeStatus, byte ErrorCode)
{
  // Temp: Just setting to team color for now.
   for(int i = Low_Seg2_LED; i <= High_Seg2_LED; i++) colors[i] = TeamC;  

   for (int i = Low_Left_Leg_LED; i <= High_Left_Leg_LED; i++)
   {
     colors[i] = TeamC;
   } 

   for (int i = Low_Right_Leg_LED; i <= High_Right_Leg_LED; i++)
   {
     colors[i] = TeamC;
   } 

}

//******************************************************************************//
//* Function: Set_Team                                                         *//
//* Description: Sets the team the robot has been assigned to.                 *//
//* Arguments: TeamColor - 0x88 = RED, 0xFF = Blue, Others invalid.            *//
//* Return Value: Boolean to indicate valid team color.                        *//
//******************************************************************************//
boolean Set_Team(byte TeamColor)
{
    boolean retval = false;  // default return value if neither of the following two conditions apply.
    // If team is red
    if (TeamColor == 0x88) 
    {
      // Set Team info.
      TeamC = Red;
      TC = 1;
      // Set return value.
      retval = true;
    } else if (TeamColor == 0xFF) // If team is blue
    {
      // Set Team info.
      TeamC = Blue;
      TC = 10;
      // Set return value.
      retval = true;
    }
}


//******************************************************************************//
//* Function: ShowTeamColor                                                    *//
//* Description: Set the team color to be displayed.                           *//
//* Arguments: N/A                                                             *//
//* Return Value: N/A                                                          *//
//******************************************************************************//
void ShowTeamColor()
{
   // Set the team color down the left leg 
   for (int i = Low_Left_Leg_LED; i <= High_Left_Leg_LED; i++)
   {
     colors[i] = TeamC;
   } 

   // Set the team color down the right leg.
   for (int i = Low_Right_Leg_LED; i <= High_Right_Leg_LED; i++)
   {
     colors[i] = TeamC;
   } 

   // Set the strip.
   SetLEDStrip(0);  
}


//********************************************************************************//
//* Function: Robot_DriveTrain_LEDS                                              *//
//* Description: Creates the effect of having a primary and secondary color      *//
//*              repesenting the state of the left and right drive train.        *//
//* Arguments: hold - time in millisecs to delay after setting the LED strip.    *//
//*            skip - how many LEDS to skip between secondary color placement.   *//
//*            color1 - the RGB color for the primary left drive train color.    *//
//*            color2 - the RGB color for the secondary left drive train color.  *//
//*            color3 - the RGB color for the primary right drive train color.   *//
//*            color4 - the RGB color for the secondary right drive train color. *//
//* Return Value: N/A                                                            *//
//********************************************************************************//
void Robot_DriveTrain_LEDS(int hold, int skip, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4)
{
    // Left Drive Train - LED segment slopes up to the right.
    for(int i = Low_Seg1_LED; i < (Low_Seg1_LED + LDTl); i++)          colors[i] = color1;  // Primary
    for(int i = Low_Seg1_LED; i < (Low_Seg1_LED + LDTl); i = i + skip) colors[i] = color2;  // Secondary    
    
    // Right Drive Train - LED segment slopes down to the right.
    for(int i = (High_Seg3_LED); i >= (High_Seg3_LED - RDTl + 1); i--)          colors[i] = color3;  // Primary
    for(int i = (High_Seg3_LED); i >= (High_Seg3_LED - RDTl + 1); i = i - skip) colors[i] = color4;  // Secondary

    SetLEDStrip(hold);
}

