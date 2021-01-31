using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GamerClickerV2
{
    class AutoClicker
    {
        private static bool running = false;
        private static modes activeMode;
        private static readonly OverlayForm overlay = new OverlayForm();
        private enum modes {
            LEFT,
            RIGHT,
            LEFTHOLD,
            RIGHTHOLD
        }
        public static bool isActive()
        {
            return running;
        }

        public static void setActiveTest()
        {
            if (running)
            {
                running = false;
            }
            else
            {
                running = true;
            }
        }

        private static void toggle(bool mRunning)
        {
            running = mRunning;
            if (running)
            {
                enable();
            } 
            else
            {
                disable();
            }
            updateOverlay();
        }
        private static void enable()
        {
            switch (activeMode)
            {
                case modes.LEFT:

                    return;
                case modes.RIGHT:

                    return;
                case modes.LEFTHOLD:

                    return;
                case modes.RIGHTHOLD:

                    return;
            }
        }
        private static void disable()
        {
            switch (activeMode)
            {
                case modes.LEFT:

                    return;
                case modes.RIGHT:

                    return;
                case modes.LEFTHOLD:

                    return;
                case modes.RIGHTHOLD:

                    return;
            }
        }

        private static void updateOverlay()
        {
            overlay.Refresh();
        }
    }
}
