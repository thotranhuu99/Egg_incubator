﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.CodeDom.Compiler;
using System.Runtime.Remoting.Messaging;

namespace Serial
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            getAvailablePort();
        }
        //Global Variable define begin
        int communication_running = 0;

        //Global Variable define end

        void getAvailablePort()
        {
            String[] ports = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Init_Timer();
        }

        private Timer timer = new Timer();

        public void Init_Timer()
        {
            timer.Interval = 1000; // every 1 second
            timer.Tick += new EventHandler(timer_Tick);
            timer.Enabled = true;
        }
        void timer_Tick(object sender, EventArgs e)
        {

            Uart_Communication.Read_UART(communication_running, serialPort1, textBox2, textBox5, 11);
        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                if (comboBox1.Text == "" || comboBox2.Text == "")
                {
                    textBox2.Text = "Please select port settings";
                }
                else
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.BaudRate = Convert.ToInt32(comboBox2.Text);
                    serialPort1.Open();
                    progressBar1.Value = 100;
                    communication_running = 1;
                    button1.Enabled = true;
                    textBox1.Enabled = true;
                    textBox4.Enabled = true;
                    button3.Enabled = false;
                    button4.Enabled = true;
                    button2.Enabled = true;
                    button5.Enabled = true;
                }
            }
            catch (UnauthorizedAccessException)
            {
                textBox2.Text = "Unauthorized Access";
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            serialPort1.Close();
            progressBar1.Value = 0;
            button1.Enabled = false;
            button3.Enabled = true;
            button4.Enabled = false;
            textBox1.Enabled = false;
            textBox4.Enabled = false;
            communication_running = 0;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                textBox2.Text = serialPort1.ReadExisting();

            }
            catch (TimeoutException)
            {
                textBox2.Text = "Timeout Exception";
            }
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void groupBox3_Enter(object sender, EventArgs e)
        {

        }

        private void button6_Click(object sender, EventArgs e)
        {
            double temperature_set;
            Byte[] temperature_set_byte;
            temperature_set = Convert.ToDouble(textBox4.Text);
            temperature_set_byte = new Byte[] { SHT30.Encode_temperature(temperature_set)[0], SHT30.Encode_temperature(temperature_set)[1] };
            Uart_Communication.Send_UART(communication_running, "Set_temp", serialPort1, new Byte[] { temperature_set_byte[0], temperature_set_byte[1] });
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            Uart_Communication.Send_UART(communication_running, "Run", serialPort1, new byte[2]);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            Byte[] test = new byte[4];
            double test_db;
            test = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, SHT30.Encode_humidity(22)[0], SHT30.Encode_humidity(22)[1] };
            test_db = SHT30.Calculate_humidity(test);
            textBox3.Text = test_db.ToString();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            Uart_Communication.Send_UART(communication_running, "Stop", serialPort1, new byte[2]);
        }

        private void textBox4_TextChanged_1(object sender, EventArgs e)
        {

        }
    }

    public static class crc
    {
        public static byte[] test_1 = new byte[] { 0x72, 0x23 };

        public static Byte crc8(Byte[] data, uint len)
        {
            const uint POLYNOMIAL = 0x31;
            const uint constant_1 = 0x80;
            uint crc = 0xFF;
            uint k = 0;
            for (uint j = len; j > 0; --j)
            {
                crc ^= data[k];
                k++;

                for (uint i = 8; i > 0; --i)
                {
                    uint Temp = crc & constant_1;
                    if (Temp == 0)
                    {
                        crc = (crc << 1);
                    }
                    else
                        crc = (crc << 1) ^ POLYNOMIAL;
                }
            }
            return (Byte)crc;
        } //Calculate and return crc in Byte
    }

    public class Uart_Communication
    {

        public static Byte[] Receive = new Byte[6];

        public static int Read_UART(int communication_running, SerialPort serialPort1, TextBox textBox_temperature, TextBox textBox_humidity, int len)
        {
            int header_detected = 0;
            int error = 1;
            int[] STX = new int[] { 0x53, 0x65 };
            Byte[] ETX = new Byte[] { 0x45, 0x6E };
            Byte ACK = 0x2B;
            Byte not_ACK = 0x2D;
            Byte[] Receive_byte = new Byte[len];
            int[] Receive_int = new int[len];
            double temperature_read;
            double humidity_read;
            if (communication_running == 1)
            {
                int[] header_dectect_byte = new int[2];
                for (int i = 1; i < len; i++)
                {
                    header_dectect_byte[0] = serialPort1.ReadByte();
                    header_dectect_byte[1] = serialPort1.ReadByte();
                    header_detected = header_dectect(header_dectect_byte, STX);
                    if (header_detected == 1)
                    {
                        Receive_byte[0] = (Byte)header_dectect_byte[0];
                        Receive_byte[1] = (Byte)header_dectect_byte[1];
                        for (int j = 0; j < len - 2; j++)
                        {
                            Receive_byte[j + 2] = (Byte)serialPort1.ReadByte();
                        }
                        error = check_frame(Receive_byte, new byte[] { (Byte)STX[0], (Byte)STX[1] }, ETX, ACK, not_ACK, 11);
                        if (error == 0)
                        {
                            temperature_read = SHT30.Calculate_temperature(Receive_byte);
                            humidity_read = SHT30.Calculate_humidity(Receive_byte);
                            textBox_temperature.Text = temperature_read.ToString();
                            textBox_humidity.Text = humidity_read.ToString();
                            return 1;
                        }
                    }
                }
                return (0);
            }
            return (0);


            /*for (int i = 1; i<10; i++)
            {
                Receive_int[i] = serialPort1.ReadByte();
                Receive_byte[i] = (Byte) Receive_int[i];
            }*/

            //if (Receive.Length == 10)
            //{
            //textBox2.Text = Receive;
            //Receive_byte = Encoding.ASCII.GetBytes(Receive);
            //temperature_read = Calculate_temperature(Receive_byte);
            //    textBox2.Text = temperature_read.ToString();
            //}
            /*error = check_frame(Encoding.ASCII.GetBytes(Receive));
            if (error==0)
            {
                temperature_read = Calculate_temperature(Receive_byte);
                textBox2.Text = temperature_read.ToString();
            }*/
        }

        public static void Send_UART(int communication_running, string Command, SerialPort serialPort1, Byte[] Temperature_byte)
        {
            Byte[] STX = new Byte[] { 0x53, 0x65 };
            Byte[] ETX = new Byte[] { 0x45, 0x6E };
            if (Command == "Run")
            {
                Byte[] run_cmd = new Byte[] { STX[0], STX[1], 0x52, 0xff, 0xff, 0xff, ETX[0], ETX[1] };
                serialPort1.Write(run_cmd, 0, run_cmd.Length);
            }
            if (Command == "Stop")
            {
                Byte[] stop_cmd = new Byte[] { STX[0], STX[1], 0x50, 0xff, 0xff, 0xff, ETX[0], ETX[1] };
                serialPort1.Write(stop_cmd, 0, stop_cmd.Length);
            }
            if (Command == "Set_temp")
            {   Byte crc_8 = crc.crc8(Temperature_byte, 2);
                Byte[] Set_temp_cmd = new Byte[] { STX[0], STX[1], 0x54, Temperature_byte[0], Temperature_byte[1], crc_8, ETX[0], ETX[1] };
                serialPort1.Write(Set_temp_cmd, 0, Set_temp_cmd.Length);
            }
        }

        public static int header_dectect(int[] header_dectect_byte, int[] STX)
        {
            if (header_dectect_byte[0] == STX[0])
            {
                if (header_dectect_byte[1] == STX[1])
                {
                    return (1);
                }
                return (0);
            }
            return (0);
        }

        public static int check_frame(Byte[] Receive, Byte[] STX, Byte[] ETX, Byte ACK, Byte not_ACK, int len)
        {
            if (Receive.Length != len)
            {
                return (1);
            }

            if (Receive[0] != STX[0] | Receive[1] != STX[1])
            {
                return (1);
            }

            if (Receive[len - 2] != ETX[0] | Receive[len - 1] != ETX[1])
            {
                return (1);
            }

            if (Receive[len - 3] != ACK & Receive[len - 3] != not_ACK)
            {
                return (1);
            }

            if (crc.crc8(new byte[] { Receive[2], Receive[3] }, 2) != Receive[4] | crc.crc8(new byte[] { Receive[5], Receive[6] }, 2) != Receive[7])
            {
                return (1);
            }
            else
            {
                return (0);
            }
        }

    }
    

    public class SHT30
    {
        public static double Calculate_temperature(Byte[] Receive)
        {
            Byte[] Temperature_byte_read = new byte[2];
            Temperature_byte_read[0] = Receive[2];
            Temperature_byte_read[1] = Receive[3];
            return (175 * ((double)Temperature_byte_read[0] * 256 + (double)Temperature_byte_read[1]) / 65535 - 45);
        }

        public static double Calculate_humidity(Byte[] Receive)
        {   
            Byte[] Humidity_byte_read = new byte[2];
            Humidity_byte_read[0] = Receive[5];
            Humidity_byte_read[1] = Receive[6];
            return (100 * ((double)Humidity_byte_read[0] * 256 + (double)Humidity_byte_read[1]) / 65535);
        }

        public static Byte[] Encode_temperature(Double temperature)
        {
            Byte[] temperature_byte = new byte[2];
            temperature_byte[0] = (Byte) (uint)(65535*(temperature+45)/(175*256));
            temperature_byte[1] = (Byte)(((uint)(65535 * (temperature + 45) / 175)) - 256 * (uint)temperature_byte[0]);
            return (temperature_byte);
        }

        public static Byte[] Encode_humidity(Double humidity)
        {
            Byte[] humidity_byte = new byte[2];
            humidity_byte[0] = (Byte)(uint)(65535 * humidity / (100 * 256));
            humidity_byte[1] = (Byte)(((uint)(65535 * humidity / 100)) - 256 * (uint)humidity_byte[0]);
            return (humidity_byte);
        }
    }    
}
