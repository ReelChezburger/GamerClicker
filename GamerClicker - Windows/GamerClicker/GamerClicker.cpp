#include <iostream>
#include <windows.h>

using namespace std;

//picks a CPS based on the data entered
float getRandomCPS(float bottom, float top, float bottomLimit, float topLimit) {
    bool fuckMe = true; // fuckMe? fuckYou
    float CPS;
    //run this until a good value is found
    while (fuckMe) {
        //CPS is set to the bottom value + a random value within the range
        CPS = ((top - bottom) * ((float)rand() / RAND_MAX)) + bottom;
        if (CPS > bottomLimit && CPS < topLimit) {
            fuckMe = false;
        }
    }
    //outputs the decided CPS to the console
    std::cout << CPS << endl;
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
    HWND target = NULL; // Target window declaration

    std::cout << "Select the target window and press Scroll Lock" << endl;

    while (target == NULL) { //Does not leave until target is set
        if (GetKeyState(VK_SCROLL) & 0x8000) { // Checks for scroll lock to be pressed
            target = GetForegroundWindow(); // Sets target as selected window
            break;
        }
    }
    std::cout << "Window selected" << endl;

    //main program loop
    while (runProgram) {
        //checks if the clicker loop is to be run
        if (runClicker == true) {
            //if the program has not run, pick a ranom CPS within the range
            if (hasRun == false) {
                currentCPS = getRandomCPS(bottomNum, topNum, bottomNum, topNum);
                hasRun = true;
            }
            //if the program has run, pick a CPS within .5 of the previous CPS to appear fluid
            else {
                float newLow = currentCPS - .5;
                float newHigh = currentCPS + .5;
                currentCPS = getRandomCPS(newLow, newHigh, bottomNum, topNum);
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
            }
        }
        //checks if scroll lock is being pressed
        else if (GetKeyState(VK_SCROLL) & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
            //starts the clicker next loop
            runClicker = true;
        }
        //if all else fails, wait 1 frame
        else {
            Sleep(17);
        }
    }
}