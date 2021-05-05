using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;

namespace GamerClickerV2
{
    public partial class Form1 : Form
    {
        //OverlayForm overlayForm = OverlayForm.getInstance();
        public static Form1 mInstance = null;

        public static Form1 getInstance()
        {
            if (mInstance == null)
                mInstance = new Form1();
            return mInstance;
        }

        public Form1()
        {
            InitializeComponent();
            //overlayForm.Hide();
            panel2.Hide();
            AutoClicker.init();
        }

        private void performClick(object sender, EventArgs e)
        {
            /*
            if (autoClickWindow != IntPtr.Zero)
            {
                PostMessage(autoClickWindow, WM_RBUTTONDOWN, IntPtr.Zero, IntPtr.Zero);
                Thread.Sleep(500);
                //PostMessage(autoClickWindow, WM_LBUTTONUP, IntPtr.Zero, IntPtr.Zero);
            }*/
            //AutoClicker.setActiveTest();
        }
 
        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox4.Checked) {
                
            } else {
                
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            AutoClicker.autoClickerPeriodicTimer.Stop();
            int windowSelectionTimeOut = 15000;
            int windowGrabberClockInterval = 200;
            int index = 0;
            button3.Text = "Press Scroll Lock to Select";
            Task<bool> windowGrabber = Task<bool>.Factory.StartNew(() =>
            {
                int startVar = AutoClicker.GetKeyState(145);
                while (AutoClicker.GetKeyState(145) == startVar)
                {
                    index++;
                    Thread.Sleep(windowGrabberClockInterval);
                    if (windowSelectionTimeOut / windowGrabberClockInterval < index)
                    {
                        return false;
                    }
                }
                AutoClicker.autoClickWindow = AutoClicker.GetForegroundWindow();
                return true;
            });
            bool windowResult = windowGrabber.Result;
            if (windowResult)
            {
                label5.Text = AutoClicker.GetActiveWindow();
                button3.Text = "Select a New Window";
            } else
            {
                button3.Text = "Select Window";
            }
            AutoClicker.autoClickerPeriodicTimer.Start();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
            {
                panel2.Show();
            }
            else
            {
                panel2.Hide();
            }
        }

        public void deactivateAllSettings()
        {
            radioButton1.Enabled = false;
            radioButton2.Enabled = false;
            radioButton3.Enabled = false;
            radioButton4.Enabled = false;

            textBox1.Enabled = false;
            textBox2.Enabled = false;

            checkBox1.Enabled = false;
            checkBox2.Enabled = false;
            checkBox3.Enabled = false;

            button3.Enabled = false;

            label1.Enabled = false;
            label2.Enabled = false;
            label3.Enabled = false;
        }

        public void activateAllSettings()
        {
            radioButton1.Enabled = true;
            radioButton2.Enabled = true;
            radioButton3.Enabled = true;
            radioButton4.Enabled = true;

            textBox1.Enabled = true;
            textBox2.Enabled = true;

            checkBox1.Enabled = true;
            checkBox2.Enabled = true;
            checkBox3.Enabled = true;

            button3.Enabled = true;

            label1.Enabled = true;
            label2.Enabled = true;
            label3.Enabled = true;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                AutoClicker.activeMode = AutoClicker.modes.LEFT;
            }
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton2.Checked)
            {
                AutoClicker.activeMode = AutoClicker.modes.RIGHT;
            }
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton3.Checked)
            {
                AutoClicker.activeMode = AutoClicker.modes.LEFTHOLD;
            }
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton4.Checked)
            {
                AutoClicker.activeMode = AutoClicker.modes.RIGHTHOLD;
            }
        }

        private void radioButton9_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton9.Checked)
            {
                AutoClicker.activeMode = AutoClicker.modes.LEFTRIGHTHOLD;
            }
        }

        public string getTextBox1Value()
        {
            return textBox1.Text;
        }

        public string getTextBox2Value()
        {
            return textBox2.Text;
        }

        public bool getWindowTargeting()
        {
            return checkBox1.Checked;
        }

        public bool getAutoEat()
        {
            return checkBox3.Checked;
        }

        public bool getRandomPause()
        {
            return checkBox2.Checked;
        }

        public int getTabControl1SelectedIndex()
        {
            return tabControl1.SelectedIndex;
        }

        private void TabControl1_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            if (tabControl1.SelectedIndex != 0)
                AutoClicker.disableClicker();
        }

        private void textBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '.' && ((TextBox)sender).Text.Contains('.'))
                e.Handled = true;
            if (char.IsNumber(e.KeyChar) || e.KeyChar == '.')
            {
                if (Regex.IsMatch(((TextBox)sender).Text, "^\\d*\\.\\d{4}$")) e.Handled = true;
            }
            else e.Handled = e.KeyChar != (char)Keys.Back;
        }

        private void tabPage1_Click(object sender, EventArgs e)
        {

        }

        private void label8_Click(object sender, EventArgs e)
        {

        }
    }
}
