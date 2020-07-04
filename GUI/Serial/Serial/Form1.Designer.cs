namespace Serial
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.Open_Port_Button = new System.Windows.Forms.Button();
            this.Close_Port_Button = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.Run_Button = new System.Windows.Forms.Button();
            this.Stop_Button = new System.Windows.Forms.Button();
            this.Set_Button = new System.Windows.Forms.Button();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.zedGraphControl1 = new ZedGraph.ZedGraphControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.zedGraphControl2 = new ZedGraph.ZedGraphControl();
            this.pbMode = new System.Windows.Forms.Button();
            this.Clear_graph = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.Draw_Graph_Button = new System.Windows.Forms.Button();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox2
            // 
            this.textBox2.Enabled = false;
            this.textBox2.Location = new System.Drawing.Point(103, 32);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(105, 28);
            this.textBox2.TabIndex = 0;
            this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
            // 
            // textBox5
            // 
            this.textBox5.Enabled = false;
            this.textBox5.Location = new System.Drawing.Point(103, 105);
            this.textBox5.Multiline = true;
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size(105, 28);
            this.textBox5.TabIndex = 2;
            this.textBox5.TextChanged += new System.EventHandler(this.textBox5_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(7, 36);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(90, 13);
            this.label4.TabIndex = 3;
            this.label4.Text = "Temperature(°C) :";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(30, 108);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(67, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Humidity(%) :";
            this.label5.Click += new System.EventHandler(this.label5_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.textBox5);
            this.groupBox2.Controls.Add(this.textBox2);
            this.groupBox2.Location = new System.Drawing.Point(463, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(225, 196);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Measured_value";
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(9, 48);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(85, 21);
            this.comboBox1.TabIndex = 0;
            // 
            // comboBox2
            // 
            this.comboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox2.FormattingEnabled = true;
            this.comboBox2.Items.AddRange(new object[] {
            "9600",
            "115200"});
            this.comboBox2.Location = new System.Drawing.Point(9, 100);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(85, 21);
            this.comboBox2.TabIndex = 1;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(130, 48);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(89, 23);
            this.progressBar1.TabIndex = 2;
            // 
            // Open_Port_Button
            // 
            this.Open_Port_Button.Location = new System.Drawing.Point(9, 143);
            this.Open_Port_Button.Name = "Open_Port_Button";
            this.Open_Port_Button.Size = new System.Drawing.Size(85, 23);
            this.Open_Port_Button.TabIndex = 5;
            this.Open_Port_Button.Text = "Open Port";
            this.Open_Port_Button.UseVisualStyleBackColor = true;
            this.Open_Port_Button.Click += new System.EventHandler(this.Open_Port_Button_Click);
            // 
            // Close_Port_Button
            // 
            this.Close_Port_Button.Enabled = false;
            this.Close_Port_Button.Location = new System.Drawing.Point(130, 143);
            this.Close_Port_Button.Name = "Close_Port_Button";
            this.Close_Port_Button.Size = new System.Drawing.Size(89, 23);
            this.Close_Port_Button.TabIndex = 6;
            this.Close_Port_Button.Text = "Close Port";
            this.Close_Port_Button.UseVisualStyleBackColor = true;
            this.Close_Port_Button.Click += new System.EventHandler(this.Close_Port_Button_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Port Name";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 75);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Baud rate";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(127, 18);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(92, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Connection status";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label8);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.textBox3);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.Close_Port_Button);
            this.groupBox3.Controls.Add(this.Open_Port_Button);
            this.groupBox3.Controls.Add(this.progressBar1);
            this.groupBox3.Controls.Add(this.comboBox2);
            this.groupBox3.Controls.Add(this.comboBox1);
            this.groupBox3.Location = new System.Drawing.Point(47, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(233, 196);
            this.groupBox3.TabIndex = 10;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "UART Communication";
            this.groupBox3.Enter += new System.EventHandler(this.groupBox3_Enter);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(127, 75);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(59, 13);
            this.label8.TabIndex = 14;
            this.label8.Text = "ACK status";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(130, 101);
            this.textBox3.Name = "textBox3";
            this.textBox3.ReadOnly = true;
            this.textBox3.Size = new System.Drawing.Size(89, 20);
            this.textBox3.TabIndex = 13;
            this.textBox3.TextChanged += new System.EventHandler(this.textBox3_TextChanged_1);
            // 
            // Run_Button
            // 
            this.Run_Button.Enabled = false;
            this.Run_Button.Location = new System.Drawing.Point(27, 36);
            this.Run_Button.Name = "Run_Button";
            this.Run_Button.Size = new System.Drawing.Size(61, 33);
            this.Run_Button.TabIndex = 0;
            this.Run_Button.Text = "Run";
            this.Run_Button.UseVisualStyleBackColor = true;
            this.Run_Button.Click += new System.EventHandler(this.Run_Button_Click);
            // 
            // Stop_Button
            // 
            this.Stop_Button.Enabled = false;
            this.Stop_Button.Location = new System.Drawing.Point(135, 36);
            this.Stop_Button.Name = "Stop_Button";
            this.Stop_Button.Size = new System.Drawing.Size(61, 33);
            this.Stop_Button.TabIndex = 1;
            this.Stop_Button.Text = "Stop";
            this.Stop_Button.UseVisualStyleBackColor = true;
            this.Stop_Button.Click += new System.EventHandler(this.Stop_Button_Click);
            // 
            // Set_Button
            // 
            this.Set_Button.Enabled = false;
            this.Set_Button.Location = new System.Drawing.Point(27, 100);
            this.Set_Button.Name = "Set_Button";
            this.Set_Button.Size = new System.Drawing.Size(61, 33);
            this.Set_Button.TabIndex = 12;
            this.Set_Button.Text = "Set";
            this.Set_Button.UseVisualStyleBackColor = true;
            this.Set_Button.Click += new System.EventHandler(this.Set_Button_Click);
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(119, 113);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(99, 20);
            this.textBox4.TabIndex = 14;
            this.textBox4.TextChanged += new System.EventHandler(this.textBox4_TextChanged_1);
            // 
            // textBox6
            // 
            this.textBox6.Enabled = false;
            this.textBox6.Location = new System.Drawing.Point(101, 156);
            this.textBox6.Multiline = true;
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size(166, 34);
            this.textBox6.TabIndex = 15;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(11, 168);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(77, 13);
            this.label6.TabIndex = 16;
            this.label6.Text = "Error message:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(116, 91);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(102, 13);
            this.label7.TabIndex = 17;
            this.label7.Text = "Set temperature(°C):";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Controls.Add(this.textBox6);
            this.groupBox4.Controls.Add(this.textBox4);
            this.groupBox4.Controls.Add(this.Set_Button);
            this.groupBox4.Controls.Add(this.Stop_Button);
            this.groupBox4.Controls.Add(this.Run_Button);
            this.groupBox4.Location = new System.Drawing.Point(706, 12);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(273, 196);
            this.groupBox4.TabIndex = 11;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Control";
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(3, 214);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1007, 333);
            this.tabControl1.TabIndex = 14;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.zedGraphControl1);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(999, 307);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Temperature";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // zedGraphControl1
            // 
            this.zedGraphControl1.Location = new System.Drawing.Point(31, 11);
            this.zedGraphControl1.Name = "zedGraphControl1";
            this.zedGraphControl1.ScrollGrace = 0D;
            this.zedGraphControl1.ScrollMaxX = 0D;
            this.zedGraphControl1.ScrollMaxY = 0D;
            this.zedGraphControl1.ScrollMaxY2 = 0D;
            this.zedGraphControl1.ScrollMinX = 0D;
            this.zedGraphControl1.ScrollMinY = 0D;
            this.zedGraphControl1.ScrollMinY2 = 0D;
            this.zedGraphControl1.Size = new System.Drawing.Size(932, 290);
            this.zedGraphControl1.TabIndex = 12;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.zedGraphControl2);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(999, 307);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Humidity";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // zedGraphControl2
            // 
            this.zedGraphControl2.Location = new System.Drawing.Point(31, 11);
            this.zedGraphControl2.Name = "zedGraphControl2";
            this.zedGraphControl2.ScrollGrace = 0D;
            this.zedGraphControl2.ScrollMaxX = 0D;
            this.zedGraphControl2.ScrollMaxY = 0D;
            this.zedGraphControl2.ScrollMaxY2 = 0D;
            this.zedGraphControl2.ScrollMinX = 0D;
            this.zedGraphControl2.ScrollMinY = 0D;
            this.zedGraphControl2.ScrollMinY2 = 0D;
            this.zedGraphControl2.Size = new System.Drawing.Size(932, 290);
            this.zedGraphControl2.TabIndex = 13;
            this.zedGraphControl2.Load += new System.EventHandler(this.zedGraphControl2_Load);
            // 
            // pbMode
            // 
            this.pbMode.Location = new System.Drawing.Point(28, 91);
            this.pbMode.Name = "pbMode";
            this.pbMode.Size = new System.Drawing.Size(75, 23);
            this.pbMode.TabIndex = 14;
            this.pbMode.Text = "Compact";
            this.pbMode.UseVisualStyleBackColor = true;
            this.pbMode.Click += new System.EventHandler(this.Scoll_Compact_Mode_Button_Click);
            // 
            // Clear_graph
            // 
            this.Clear_graph.Location = new System.Drawing.Point(28, 48);
            this.Clear_graph.Name = "Clear_graph";
            this.Clear_graph.Size = new System.Drawing.Size(75, 23);
            this.Clear_graph.TabIndex = 13;
            this.Clear_graph.Text = "Clear Graph";
            this.Clear_graph.UseVisualStyleBackColor = true;
            this.Clear_graph.Click += new System.EventHandler(this.Clear_Graph_Button_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.Draw_Graph_Button);
            this.groupBox1.Controls.Add(this.Clear_graph);
            this.groupBox1.Controls.Add(this.pbMode);
            this.groupBox1.Location = new System.Drawing.Point(304, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(140, 196);
            this.groupBox1.TabIndex = 15;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Graph control";
            // 
            // Draw_Graph_Button
            // 
            this.Draw_Graph_Button.Enabled = false;
            this.Draw_Graph_Button.Location = new System.Drawing.Point(28, 132);
            this.Draw_Graph_Button.Name = "Draw_Graph_Button";
            this.Draw_Graph_Button.Size = new System.Drawing.Size(75, 23);
            this.Draw_Graph_Button.TabIndex = 15;
            this.Draw_Graph_Button.Text = "Draw Graph";
            this.Draw_Graph_Button.UseVisualStyleBackColor = true;
            this.Draw_Graph_Button.Click += new System.EventHandler(this.Draw_Graph_Button_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1014, 559);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox4);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Serial Communication";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.ComboBox comboBox2;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Button Open_Port_Button;
        private System.Windows.Forms.Button Close_Port_Button;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button Run_Button;
        private System.Windows.Forms.Button Stop_Button;
        private System.Windows.Forms.Button Set_Button;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button Clear_graph;
        private ZedGraph.ZedGraphControl zedGraphControl1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button pbMode;
        private System.Windows.Forms.GroupBox groupBox1;
        private ZedGraph.ZedGraphControl zedGraphControl2;
        private System.Windows.Forms.Button Draw_Graph_Button;
    }
}

