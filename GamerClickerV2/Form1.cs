using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GamerClickerV2
{
    public partial class Form1 : Form
    {
        private const int WM_LBUTTONDOWN = 0x201;
        private const int WM_LBUTTONUP = 0x202;
        private const int WM_RBUTTONDOWN = 0x204;
        private const int WM_RBUTTONUP = 0x205;
        [DllImport("user32.dll")]
        static extern bool PostMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll")]
        static extern IntPtr GetForegroundWindow();

        [DllImport("user32.dll")]
        static extern int GetWindowText(IntPtr hWnd, StringBuilder text, int count);

        private IntPtr autoClickWindow = IntPtr.Zero;

        OverlayForm overlayForm = OverlayForm.getInstance();

        public Form1()
        {
            InitializeComponent();
            overlayForm.Hide();
        }

        private void grabActiveWindow(object sender, EventArgs e)
        {
            Thread.Sleep(2000);
            autoClickWindow = GetForegroundWindow();
            Debug.WriteLine("Grabbed Window");
            GetActiveWindow();
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
        private void GetActiveWindow()
        {
            const int nChars = 256;
            IntPtr handle;
            StringBuilder Buff = new StringBuilder(nChars);
            handle = GetForegroundWindow();
            if (GetWindowText(handle, Buff, nChars) > 0)
            {
                Console.WriteLine(Buff.ToString());
            }
        }

        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox4.Checked) {
                
            } else {
                
            }
        }
    }
}
