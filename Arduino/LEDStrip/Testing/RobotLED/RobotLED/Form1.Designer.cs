namespace RobotLED
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Send_Cmd = new System.Windows.Forms.Button();
            this.Exit_Cmd = new System.Windows.Forms.Button();
            this.RobotIP = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.RightDrive_TB = new System.Windows.Forms.TrackBar();
            this.LeftDrive_TB = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.TeamColor = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.RobotID = new System.Windows.Forms.TextBox();
            this.LD_Value = new System.Windows.Forms.Label();
            this.RD_Value = new System.Windows.Forms.Label();
            this.Ack = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.RightDrive_TB)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.LeftDrive_TB)).BeginInit();
            this.SuspendLayout();
            // 
            // Send_Cmd
            // 
            this.Send_Cmd.Location = new System.Drawing.Point(145, 221);
            this.Send_Cmd.Name = "Send_Cmd";
            this.Send_Cmd.Size = new System.Drawing.Size(75, 23);
            this.Send_Cmd.TabIndex = 0;
            this.Send_Cmd.Text = "Send";
            this.Send_Cmd.UseVisualStyleBackColor = true;
            this.Send_Cmd.Click += new System.EventHandler(this.Send_Cmd_Click);
            // 
            // Exit_Cmd
            // 
            this.Exit_Cmd.Location = new System.Drawing.Point(235, 221);
            this.Exit_Cmd.Name = "Exit_Cmd";
            this.Exit_Cmd.Size = new System.Drawing.Size(75, 23);
            this.Exit_Cmd.TabIndex = 1;
            this.Exit_Cmd.Text = "Exit";
            this.Exit_Cmd.UseVisualStyleBackColor = true;
            this.Exit_Cmd.Click += new System.EventHandler(this.Exit_Cmd_Click);
            // 
            // RobotIP
            // 
            this.RobotIP.Location = new System.Drawing.Point(72, 16);
            this.RobotIP.Name = "RobotIP";
            this.RobotIP.Size = new System.Drawing.Size(100, 20);
            this.RobotIP.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Robot IP:";
            // 
            // RightDrive_TB
            // 
            this.RightDrive_TB.Location = new System.Drawing.Point(206, 153);
            this.RightDrive_TB.Maximum = 100;
            this.RightDrive_TB.Minimum = -100;
            this.RightDrive_TB.Name = "RightDrive_TB";
            this.RightDrive_TB.Size = new System.Drawing.Size(104, 45);
            this.RightDrive_TB.TabIndex = 4;
            this.RightDrive_TB.Scroll += new System.EventHandler(this.RightDrive_TB_Scroll);
            // 
            // LeftDrive_TB
            // 
            this.LeftDrive_TB.Location = new System.Drawing.Point(12, 153);
            this.LeftDrive_TB.Maximum = 100;
            this.LeftDrive_TB.Minimum = -100;
            this.LeftDrive_TB.Name = "LeftDrive_TB";
            this.LeftDrive_TB.Size = new System.Drawing.Size(104, 45);
            this.LeftDrive_TB.TabIndex = 5;
            this.LeftDrive_TB.Scroll += new System.EventHandler(this.LeftDrive_TB_Scroll);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(36, 133);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Left Drive:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(232, 133);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(63, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Right Drive:";
            // 
            // TeamColor
            // 
            this.TeamColor.BackColor = System.Drawing.Color.Blue;
            this.TeamColor.Location = new System.Drawing.Point(153, 80);
            this.TeamColor.Name = "TeamColor";
            this.TeamColor.Size = new System.Drawing.Size(75, 23);
            this.TeamColor.TabIndex = 8;
            this.TeamColor.Text = "Blue";
            this.TeamColor.UseVisualStyleBackColor = false;
            this.TeamColor.Click += new System.EventHandler(this.TeamColor_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(110, 85);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 13);
            this.label4.TabIndex = 9;
            this.label4.Text = "Team:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(14, 43);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 13);
            this.label5.TabIndex = 10;
            this.label5.Text = "Robot ID:";
            // 
            // RobotID
            // 
            this.RobotID.Location = new System.Drawing.Point(73, 40);
            this.RobotID.Name = "RobotID";
            this.RobotID.Size = new System.Drawing.Size(100, 20);
            this.RobotID.TabIndex = 11;
            // 
            // LD_Value
            // 
            this.LD_Value.AutoSize = true;
            this.LD_Value.Location = new System.Drawing.Point(59, 185);
            this.LD_Value.Name = "LD_Value";
            this.LD_Value.Size = new System.Drawing.Size(13, 13);
            this.LD_Value.TabIndex = 12;
            this.LD_Value.Text = "0";
            // 
            // RD_Value
            // 
            this.RD_Value.AutoSize = true;
            this.RD_Value.Location = new System.Drawing.Point(252, 185);
            this.RD_Value.Name = "RD_Value";
            this.RD_Value.Size = new System.Drawing.Size(13, 13);
            this.RD_Value.TabIndex = 13;
            this.RD_Value.Text = "0";
            // 
            // Ack
            // 
            this.Ack.AutoSize = true;
            this.Ack.BackColor = System.Drawing.Color.LightGreen;
            this.Ack.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Ack.Location = new System.Drawing.Point(163, 253);
            this.Ack.Name = "Ack";
            this.Ack.Size = new System.Drawing.Size(18, 15);
            this.Ack.TabIndex = 14;
            this.Ack.Text = "-1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(349, 275);
            this.Controls.Add(this.Ack);
            this.Controls.Add(this.RD_Value);
            this.Controls.Add(this.LD_Value);
            this.Controls.Add(this.RobotID);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.TeamColor);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.LeftDrive_TB);
            this.Controls.Add(this.RightDrive_TB);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.RobotIP);
            this.Controls.Add(this.Exit_Cmd);
            this.Controls.Add(this.Send_Cmd);
            this.Name = "Form1";
            this.Text = "Robot LED";
            ((System.ComponentModel.ISupportInitialize)(this.RightDrive_TB)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.LeftDrive_TB)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Send_Cmd;
        private System.Windows.Forms.Button Exit_Cmd;
        private System.Windows.Forms.TextBox RobotIP;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar RightDrive_TB;
        private System.Windows.Forms.TrackBar LeftDrive_TB;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button TeamColor;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox RobotID;
        private System.Windows.Forms.Label LD_Value;
        private System.Windows.Forms.Label RD_Value;
        private System.Windows.Forms.Label Ack;
    }
}

