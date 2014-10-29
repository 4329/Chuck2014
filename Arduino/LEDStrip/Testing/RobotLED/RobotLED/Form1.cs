using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.InteropServices;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using RobotLED;

namespace RobotLED
{
    public partial class Form1 : Form
    {
        public MessageMgr msgMgr;
        private MessageMgr.MessageHandler messageDelegate;


        public uint txMsgId = 0;

        public Form1()
        {
            InitializeComponent();
            msgMgr = new MessageMgr();
            Initialize();
        }

        public void Initialize()
        {
            messageDelegate = new MessageMgr.MessageHandler(this.OnMessage);
            msgMgr.MessageConsumer += messageDelegate;
            msgMgr.Initialize();
        }

        

        private void TeamColor_Click(object sender, EventArgs e)
        {
            if (TeamColor.BackColor == Color.Blue)
            {
                TeamColor.BackColor = Color.Red;
                TeamColor.Text = "Red";
            }
            else
            {
                TeamColor.BackColor = Color.Blue;
                TeamColor.Text = "Blue";
            }
        }

        private void Exit_Cmd_Click(object sender, EventArgs e)
        {
            msgMgr.Terminate();
            Close();
        }

        private void Send_Cmd_Click(object sender, EventArgs e)
        {
            Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram,ProtocolType.Udp);
            IPAddress serverAddr = IPAddress.Parse(RobotIP.Text);
            IPEndPoint endPoint = new IPEndPoint(serverAddr, 10000);
            Byte[] txBuffer = new byte[1024];

            int index = 0;
            Byte[] temp = new Byte[4];

            temp = BitConverter.GetBytes(Convert.ToUInt16(RobotID.Text));
            txBuffer[index] = temp[0];
            index++;
            txBuffer[index] = temp[1];
            index++;
            temp = BitConverter.GetBytes(MT.MT_Robot_LED_Cmd);
            txBuffer[index] = temp[0];
            index++;
            txBuffer[index] = temp[1];
            index++;
            temp = BitConverter.GetBytes(txMsgId);
            txBuffer[index] = temp[0];
            index++;
            txBuffer[index] = temp[1];
            index++;
            txBuffer[index] = temp[2];
            index++;
            txBuffer[index] = temp[3];
            index++;
            if (TeamColor.BackColor == Color.Red)
            {
                txBuffer[index] = 0x88;
                index++;
            } else if (TeamColor.BackColor == Color.Blue)
            {
                txBuffer[index] = 0xff;
                index++;
            }
            txBuffer[index] = 0x00;
            index++;
            txBuffer[index] = 0x00;
            index++;

            int temp2;
            uint temp3;

            temp2 = Convert.ToInt16(LD_Value.Text);
            if (temp2 < 0)
            {
                temp3 = Convert.ToUInt16(Math.Abs(temp2) + 100);
            } else
            {
                temp3 = Convert.ToUInt16(Math.Abs(temp2));
            }
            //txBuffer[index] = Convert.ToByte(Convert.ToSByte(LD_Value.Text));
            txBuffer[index] = Convert.ToByte(temp3);
            index++;

            temp2 = Convert.ToInt16(RD_Value.Text);
            if (temp2 < 0)
            {
                temp3 = Convert.ToUInt16(Math.Abs(temp2) + 100);
            }
            else
            {
                temp3 = Convert.ToUInt16(Math.Abs(temp2));
            }
            //txBuffer[index] = Convert.ToByte(Convert.ToSByte(RD_Value.Text));
            txBuffer[index] = Convert.ToByte(temp3);
            index++;

            sock.SendTo(txBuffer,index,SocketFlags.None, endPoint);
            Ack.Text = txMsgId.ToString();
            Ack.BackColor = Color.Yellow;
            txMsgId++;
        }

        public void OnMessage(object sender, MessageMgr.MessageEventArgs args)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<object, MessageMgr.MessageEventArgs>(OnMessage), new object[] { sender, args });
                return;
            }

            if (args.msg.Message_Type == MT.MT_Robot_LED_Announce)
            {
                LED_Announce msgData = (LED_Announce)args.msg;
                Process_LED_Announce(msgData);
            } else if (args.msg.Message_Type == MT.MT_Robot_LED_Cmd_Resp)
            {
                LED_Cmd_Resp msgData = (LED_Cmd_Resp)args.msg;
                Process_LED_Resp(msgData);
            }
        }

        public void Process_LED_Announce(LED_Announce msg)
        {
            RobotIP.Text = msg.IP_Address;
            RobotID.Text = msg.Robot_Id.ToString();
//            if (msg.TeamColor == 0x88)
//            {
//                TeamColor.BackColor = Color.Red;
//                TeamColor.Text = "Red";
//            } else if (msg.TeamColor == 0xff)
//            {
//                TeamColor.BackColor = Color.Blue;
//                TeamColor.Text = "Blue";
//            }
        }

        public void Process_LED_Resp(LED_Cmd_Resp msg)
        {
            if (Ack.Text == msg.Acked_Message_Id.ToString())
            {
                Ack.BackColor = Color.LightGreen;
            }
        }

        private void LeftDrive_TB_Scroll(object sender, EventArgs e)
        {
            LD_Value.Text = LeftDrive_TB.Value.ToString();
        }

        private void RightDrive_TB_Scroll(object sender, EventArgs e)
        {
            RD_Value.Text = RightDrive_TB.Value.ToString();
        }
    }
}
