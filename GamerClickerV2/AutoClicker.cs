using System;
using System.Collections.Generic;
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
    class AutoClicker
    {
        private static bool running = false;
        private static bool activeKey = false;
        private static bool oldActiveKey = false;
        public static modes activeMode;
        private static readonly OverlayForm overlay = new OverlayForm();
        public static System.Windows.Forms.Timer autoClickerPeriodicTimer = new System.Windows.Forms.Timer { Interval = 50 };
        private static double bottomNum;
        private static double topNum;
        private static uint activeMouseActionDown;
        private static uint activeMouseActionUp;
        public enum modes {
            LEFT,
            RIGHT,
            LEFTHOLD,
            RIGHTHOLD
        }

        public static void init()
        {
            autoClickerPeriodicTimer.Tick += autoClickerPeriodicTimer_Tick;
            autoClickerPeriodicTimer.Start();
        }
        public static bool isActive()
        {
            return running;
        }

        private static void toggle()
        {
            if (!running)
            {
                running = true;
                enable();
                Form1.getInstance().deactivateAllSettings();
            } 
            else
            {
                running = false;
                disable();
                Form1.getInstance().activateAllSettings();
            }
            updateOverlay();
        }
        private static void autoClickAction()
        {
            bool hasRun = false;
            double currentCPS = 0.0;
            bool windowTargeting = Form1.getInstance().getWindowTargeting();
            bool autoEat = Form1.getInstance().getAutoEat();
            bool randomPauseBool = Form1.getInstance().getRandomPause();
            int clickIndex = 0;
            if (activeMode == modes.LEFT) {
                activeMouseActionDown = WM_LBUTTONDOWN;
                activeMouseActionUp = WM_LBUTTONUP;
            } else {
                activeMouseActionDown = WM_RBUTTONDOWN;
                activeMouseActionUp = WM_RBUTTONUP;
            }
            while (running)
            {
                if (hasRun == false)
                {
                    if (topNum - bottomNum >= 0.5f)
                    {
                        currentCPS = getRandomRangeValue(bottomNum, topNum, bottomNum, topNum);
                    }
                    else
                    {
                        currentCPS = bottomNum;
                    }
                    hasRun = true;
                }
                //if the program has run, pick a CPS within .5 of the previous CPS to appear fluid, and possibly pause
                else
                {
                    if (randomPauseBool == true)
                    {
                        randomPause();
                    }
                    //add the new targets
                    double newLow = currentCPS - 0.5;
                    double newHigh = currentCPS + 0.5;
                    if (topNum - bottomNum >= 0.5f)
                    {
                        currentCPS = getRandomRangeValue(newLow, newHigh, bottomNum, topNum);
                    }
                    else
                    {
                        currentCPS = bottomNum;
                    }
                }
                Thread.Sleep(CPS2Delay(currentCPS));
                if (!windowTargeting)
                {
                    MousePoint position = GetCursorPosition();
                    mouse_event(activeMouseActionDown, 0, 0, 0, 0);
                    Thread.Sleep(10);
                    mouse_event(activeMouseActionUp, 0, 0, 0, 0);
                    
                }
                else
                {
                    PostMessage(autoClickWindow, activeMouseActionDown, IntPtr.Zero, IntPtr.Zero);
                    Thread.Sleep(10);
                    PostMessage(autoClickWindow, activeMouseActionUp, IntPtr.Zero, IntPtr.Zero);
                }
                if (autoEat)
                {
                    clickIndex++;
                    if (clickIndex >= 100)
                    {
                        if (!windowTargeting)
                        {
                            MousePoint position = GetCursorPosition();
                            mouse_event(WM_RBUTTONDOWN, position.X, position.Y, 0, 0);
                            Thread.Sleep(2000);
                            mouse_event(WM_RBUTTONUP, position.X, position.Y, 0, 0);
                        }
                        else
                        {
                            PostMessage(autoClickWindow, WM_RBUTTONDOWN, IntPtr.Zero, IntPtr.Zero);
                            Thread.Sleep(2000);
                            PostMessage(autoClickWindow, WM_RBUTTONUP, IntPtr.Zero, IntPtr.Zero);
                        }
                    }
                }
            }
        }
        private static void autoHoldAction(object sender, EventArgs e)
        {
            while (running)
            {

            }
        }
        private static void enable()
        {
            try {
                bottomNum = Convert.ToDouble(Form1.getInstance().getTextBox1Value());
            } catch
            {
                bottomNum = 0;
            }
            try { 
                topNum = Convert.ToDouble(Form1.getInstance().getTextBox2Value());
            } catch
            {
                topNum = 0;
            }
            switch (activeMode)
            {
                case modes.LEFT:
                case modes.RIGHT:
                    Task autoClickerTask = Task.Factory.StartNew(() => { autoClickAction(); });
                    return;
                case modes.LEFTHOLD:
                case modes.RIGHTHOLD:
                    
                    return;
            }
        }
        private static void disable()
        {
            switch (activeMode)
            {
                case modes.LEFT:
                case modes.RIGHT:

                    return;
                case modes.LEFTHOLD:
                case modes.RIGHTHOLD:

                    return;
            }
        }

        private static void autoClickerPeriodicTimer_Tick(object sender, EventArgs e)
        {
            if (Form1.getInstance().getTabControl1SelectedIndex() == 0)
            {
                oldActiveKey = activeKey;
                activeKey = (GetAsyncKeyState(Keys.Scroll) & 32768) > 0;
                if (oldActiveKey != activeKey && (GetAsyncKeyState(Keys.Scroll) & 32768) > 0)
                {
                    toggle();
                }
            }
        }

        public static void disableClicker()
        {
            if (running)
            {
                disable();
            }
        }

        private static void updateOverlay()
        {
            //overlay.Refresh();
        }
        public static string GetActiveWindow()
        {
            const int nChars = 256;
            IntPtr handle;
            StringBuilder Buff = new StringBuilder(nChars);
            handle = GetForegroundWindow();
            if (GetWindowText(handle, Buff, nChars) > 0) return Buff.ToString();
            else return "None";
        }

        public static double getRandomRangeValue(double bottom, double top, double bottomLimit = -1, double topLimit = -1)
        {
            bool badValue = true;
            double cps = -1;
            if (bottomLimit == -1 && topLimit == -1)
            {
                bottomLimit = bottom;
                topLimit = top;
            }
            Random rand = new Random();
            while (badValue)
            {
                cps = ((top - bottom) * (rand.NextDouble())) + bottom;
                if (cps > bottomLimit && cps < topLimit)
                {
                    badValue = false;
                }
            }
            return cps;
        }

        private static int CPS2Delay(double clicks)
        {
            return (int)Math.Round(1000 / clicks);
        }

        private static int randomPause()
        {
            int deciderInt = (int)Math.Round(getRandomRangeValue(0, 200));
            if (deciderInt == 50)
            {
                int delayInt = (int)Math.Round(getRandomRangeValue(500, 10000));
                return delayInt;
            }
            return -1;
        }

        public static MousePoint GetCursorPosition()
        {
            MousePoint currentMousePoint;
            var gotPoint = GetCursorPos(out currentMousePoint);
            if (!gotPoint) { currentMousePoint = new MousePoint(0, 0); }
            return currentMousePoint;
        }

        private const int WM_LBUTTONDOWN = 0x201;
        private const int WM_LBUTTONUP = 0x202;
        private const int WM_RBUTTONDOWN = 0x204;
        private const int WM_RBUTTONUP = 0x205;
        [DllImport("user32.dll")]
        public static extern bool PostMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll")]
        public static extern IntPtr GetForegroundWindow();

        [DllImport("user32.dll")]
        public static extern int GetWindowText(IntPtr hWnd, StringBuilder text, int count);

        [DllImport("user32.dll", CharSet = CharSet.Auto, ExactSpelling = true)]
        public static extern short GetKeyState(int keyCode);

        public static IntPtr autoClickWindow = IntPtr.Zero;

        [DllImport("user32.dll")]
        public static extern short GetAsyncKeyState(System.Windows.Forms.Keys vKey);

        [DllImport("user32.dll")]
        private static extern void mouse_event(uint dwFlags, int dx, int dy, int dwData, int dwExtraInfo);

        [DllImport("user32.dll")]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool GetCursorPos(out MousePoint lpMousePoint);

        public struct MousePoint
        {
            public int X;
            public int Y;

            public MousePoint(int x, int y)
            {
                X = x;
                Y = y;
            }
        }
    }
}
