using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

    
namespace RobotLED
{
    public class MT
    {
        public const uint MT_Robot_LED_Cmd = 0x01;
        public const uint MT_Robot_LED_Cmd_Resp = 0x02;
        public const uint MT_Robot_LED_Announce = 0x03;
    }

    public class MessageClass
    {
        public uint Robot_Id;
        public uint Message_Type;
        public ulong Message_Id;
        public String IP_Address;
    };

    public class LED_Cmd_Message : MessageClass
    {
        public byte TeamColor;               // 0x88 = Red, 0xFF = Blue.
        public byte CodeStatus;              // 0x00 = Normal.
        public byte ErrorCode;               // 0x00 = No Error.
        public byte LeftDriveTrainPercent;   // 0 = 0%  100 = 100% (Forward) 101 = -1%  200 = -100% (Reverse)
        public byte RightDriveTrainPercent;  // 0 = 0%  100 = 100% (Forward) 101 = -1%  200 = -100% (Reverse)
    };

    public class LED_Cmd_Resp : MessageClass
    {
        public ulong Acked_Message_Id;
    };

    public class LED_Announce : MessageClass
    {
        public char[] TeamName = new char[25];
        public byte TeamColor;               // 0x00 = Red, 0xFF = Blue.
    };

}
