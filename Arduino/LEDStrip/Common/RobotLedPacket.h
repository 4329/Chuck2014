#ifndef _ROBOT_LED_CMD_
#define _ROBOT_LED_CMD_ 1

#define rLED_Port 10000

struct Robot_Header {
   unsigned int  Robot_Id;
   unsigned int  Message_Type;
   unsigned long Message_Id;              
};

typedef struct Robot_Header rHeader;

struct Robot_LED_Cmd {                    // Message_Type: 0x01
            rHeader header;
            byte TeamColor;               // 0x88 = Red, 0xFF = Blue.
            byte CodeStatus;              // 0x00 = Normal.
            byte ErrorCode;               // 0x00 = No Error.
            byte LeftDriveTrainPercent;   // 0 = 0%  100 = 100% (Forward) 101 = -1%  200 = -100% (Reverse)
            byte RightDriveTrainPercent;  // 0 = 0%  100 = 100% (Forward) 101 = -1%  200 = -100% (Reverse)
};

typedef struct Robot_LED_Cmd rLedCmd;


struct Robot_LED_Resp {                   // Message_Type: 0x02
            rHeader header;
            unsigned long Acked_Message_Id;
};

typedef struct Robot_LED_Resp rLedResp;

struct Robot_LED_Announce {               // Message_Type: 0x03
            rHeader header;
            char TeamName[25];
            byte TeamColor;               // 0x00 = Red, 0xFF = Blue.
};
                        

typedef struct Robot_LED_Announce rLedAnn;

#endif
