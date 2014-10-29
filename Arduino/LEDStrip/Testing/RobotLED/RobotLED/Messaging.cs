using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Timers;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;


namespace RobotLED
{
    public struct Robot_Header
    {
        public uint Robot_Id;
        public uint Message_Type;
        public ulong Message_Id;
    }

    public struct Robot_LED_Cmd
    {                    // Message_Type: 0x01
        public Robot_Header header;
        public byte TeamColor;               // 0x88 = Red, 0xFF = Blue.
        public byte CodeStatus;              // 0x00 = Normal.
        public byte ErrorCode;               // 0x00 = No Error.
        public byte LeftDriveTrainPercent;   // 0x00 = 0%  0x80 = 100% (Forward) 0x8D = -10%  0xFF = -100% (Reverse)
        public byte RightDriveTrainPercent;  // 0x00 = 0%  0x80 = 100% (Forward) 0x8D = -10%  0xFF = -100% (Reverse)
    };

    public struct Robot_LED_Cmd_Resp
    {                    // Message_Type: 0x02
        public Robot_Header header;
        public ulong Acked_Message_Id;
    };


    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Robot_LED_Announce  // Message_Type: 0x03
    {
        public Robot_Header header;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 25)]
        public char[] TeamName;
        public byte TeamColor;               // 0x00 = Red, 0xFF = Blue.
    };

    
    public class MessageMgr
    {
        protected Semaphore sem = new Semaphore(1, 1);
        private Thread messagingThread;

        public delegate void MessageHandler(object sender, MessageEventArgs e);
        public event MessageHandler MessageConsumer = null;
        public class MessageEventArgs : EventArgs
        {
            public MessageClass msg;
        }


        public bool IsMessageConsumerNull()
        {
            return (MessageConsumer == null);
        }

        public void AddMessageDelegate(MessageHandler hndl)
        {
            MessageConsumer += hndl;
        }

        public void TriggerMessage(object sender, MessageEventArgs e)
        {
            if (MessageConsumer != null) MessageConsumer(sender, e);
        }

        UdpClient newsock;

        public int Initialize()
        {
            IPEndPoint ipep = new IPEndPoint(IPAddress.Any, 10000);
            newsock = new UdpClient(ipep);
            messagingThread = new Thread(new ThreadStart(Process));
            messagingThread.Start();
            return 0;
        }

        public void Terminate()
        {
            messagingThread.Join();
        }

        public void Process()
        {
            unsafe
            {
                if (sem.SafeWaitHandle.IsClosed)
                {
                    return;
                }
                sem.WaitOne();
                if (!IsMessageConsumerNull())
                {
                    MessageClass msg = new MessageClass();
                    while (GetNextMessage(ref msg))
                    {
                        MessageEventArgs args = new MessageEventArgs();
                        args.msg = msg;
                        TriggerMessage(this, args);
                    }
                }
                sem.Release();
            }
        }
        public bool GetNextMessage(ref MessageClass msg)
        {
            bool retval = false;
            IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
            int index = 0;

            byte[] data1 = newsock.Receive(ref sender);
            uint rId = BitConverter.ToUInt16(data1, index);
            index = index + 2;
            uint mType = BitConverter.ToUInt16(data1, index);
            index = index + 2;
            ulong mId = BitConverter.ToUInt32(data1, index);
            index = index + 4;

            switch (mType)
            {
                case 2:
                    LED_Cmd_Resp rmsg = new LED_Cmd_Resp();
                    rmsg.Message_Type = mType;
                    rmsg.Robot_Id = rId;
                    rmsg.Message_Id = mId;
                    rmsg.IP_Address = sender.Address.ToString();
                    rmsg.Acked_Message_Id = BitConverter.ToUInt32(data1, index);
                    msg = rmsg;
                    retval = true;
                    break;
                case 3:
                    LED_Announce amsg = new LED_Announce();
                    amsg.Message_Type = mType;
                    amsg.Robot_Id = rId;
                    amsg.Message_Id = mId;
                    amsg.IP_Address = sender.Address.ToString();

                    for (int i = 0; i < 25; i++)
                    {
                        amsg.TeamName[i] = BitConverter.ToChar(data1, index);
                        index++;
                    }

                    amsg.TeamColor = data1[index];
                    msg = amsg;
                    retval = true;
                    break;
                default:
                    retval = false;
                    break;
            }

            return retval;
        }

    }
}
