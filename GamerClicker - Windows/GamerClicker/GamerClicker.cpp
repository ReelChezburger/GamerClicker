#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <stdexcept>
#include <limits>

using namespace std;

HWND target = NULL; // Target window declaration

//picks a random float based on the data entered
float getRandomRangeValue(float bottom, float top, float bottomLimit = -1, float topLimit = -1) {
    bool fuckMe = true; // fuckMe? fuckYou
    float CPS;
    //if no limit is passed, set the limits to be the top and bottom values
    if (bottomLimit == -1 && topLimit == -1) {
        bottomLimit = bottom;
        topLimit = top;
    }
    //run this until a good value is found
    while (fuckMe) {
        //CPS is set to the bottom value + a random value within the range
        CPS = ((top - bottom) * ((float)rand() / RAND_MAX)) + bottom;
        if (CPS > bottomLimit && CPS < topLimit) {
            fuckMe = false;
        }
    }
    return CPS;
};

//converts CPS to a frametime
int CPS2Delay(float clicks) {
    //divides 1000 by the CPS to get a frametime
    int result = round(1000 / clicks);
    return result;
};

//checks which key is pressed
bool KeyIsPressed(unsigned char k) {
    USHORT status = GetAsyncKeyState(k);
    return (((status & 0x8000) >> 15) == 1) || ((status & 1) == 1);
}

void randomPause() {
    //get random value to decide if stop click
    int sexyDecider = round(getRandomRangeValue(0, 200));
    if (sexyDecider == 50) {
        //decide length of pause
        int sexyDelay = round(getRandomRangeValue(500, 10000));
        std::cout << "Adding a " << sexyDelay << "ms pause" << endl;
        Sleep(sexyDelay);
    }
}

int main()
{
    //creates variables to put bottom and top range values into
    float bottomNum;
    float topNum;
    bool cpsEntered = false; // false = CPS not entered/valid true = CPS valid
    while (cpsEntered == false) { // Loops until CPS is valid
        std::cout << "Enter lowest target CPS..." << endl;
        std::cin >> bottomNum;

        std::cout << "Enter highest target CPS..." << endl;
        std::cin >> topNum;

        if (bottomNum > topNum) { // Checks if bottom is greater than top
            std::cout << "The lowest target can not be larger than the highest target" << endl;
        }
        else { // CPS is valid
            cpsEntered = true; // CPS is valid
            break;
        }
    }

    //creates bools that run loops
    bool runClicker = false; //no clicking will occur
    bool runProgram = true; //the main program loop will run
    bool hasRun = false; //the clicker has not run yet
    float currentCPS = 0.0; //filler variable for what CPS is assigned

    std::cout << "Select the target window and press Scroll Lock" << endl;

    while (target == NULL) { //Does not leave until target is set
        if (GetKeyState(VK_SCROLL) & 0x8000) { // Checks for scroll lock to be pressed
            target = GetForegroundWindow(); // Sets target as selected window
            if (!target) {
                std::cout << "Failed to get traget!";
            }
            else {
                while (GetKeyState(VK_SCROLL) & 0x8000) { //while key is pressed do not progress so that program is not inadvertantly started

                }
                break;
            }
        }
    }
    std::cout << "Window selected" << endl;

    //main program loop
    while (runProgram) {
        //checks if the clicker loop is to be run
        if (runClicker == true) {
            //if the program has not run, pick a ranom CPS within the range
            if (hasRun == false) {
                currentCPS = getRandomRangeValue(bottomNum, topNum, bottomNum, topNum);
                hasRun = true;
            }
            //if the program has run, pick a CPS within .5 of the previous CPS to appear fluid, and possibly pause
            else {
                //decide if program should pause
                randomPause();
                //add the new targets
                float newLow = currentCPS - 0.5;
                float newHigh = currentCPS + 0.5;
                currentCPS = getRandomRangeValue(newLow, newHigh, bottomNum, topNum);
            }
            if (currentCPS < 0) {
                std::cout << "CPS is less than 0" << endl;
                return -1;
            }
            //wait the frametime that correlates to the picked CPS
            Sleep(CPS2Delay(currentCPS));
            //mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Old Clicker code
            PostMessage(target, WM_LBUTTONDOWN, 0, 1 & 2 << 16); // Sends left mouse button down to target window
            PostMessage(target, WM_LBUTTONUP, 0, 1 & 2 << 16); // Sends left mouse button up to target window

            //checks if pause is being pressed
            if (GetKeyState(VK_PAUSE) & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
                //stops the clicker loop
                runClicker = false;
                std::cout << "Clicker Paused" << endl;
            }

        }
        //checks if scroll lock is being pressed
        else if (GetKeyState(VK_SCROLL) & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
            //starts the clicker next loop
            runClicker = true;
            std::cout << "Clicker Started" << endl;
        }
        //if all else fails, wait 1 frame
        else {
            Sleep(17);
        }
    }
}