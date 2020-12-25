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
    float cps;
    //if no limit is passed, set the limits to be the top and bottom values
    if (bottomLimit == -1 && topLimit == -1) {
        bottomLimit = bottom;
        topLimit = top;
    }

    //run this until a good value is found
    while (fuckMe) {
        //CPS is set to the bottom value + a random value within the range
        cps = ((top - bottom) * ((float)rand() / RAND_MAX)) + bottom;
        if (cps > bottomLimit && cps < topLimit) {
            fuckMe = false;
        }
    }

    return cps;
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
    bool randomPauseBool;
    std::string pauseString;
    float bottomNum;
    float topNum;
    bool cpsEntered = false; // false = CPS not entered/valid true = CPS valid
    while (cpsEntered == false) { // Loops until CPS is valid
        std::cout << "Enter lowest target CPS..." << endl;
        std::cin >> bottomNum;

        std::cout << "Enter highest target CPS..." << endl;
        std::cin >> topNum;

        //ask if you want pauses
        bool pauseSettingSet = false;
        while (pauseSettingSet == false) {
            std::cout << "would you like random pauses? Y/N" << endl;
            std::cin >> pauseString;

            //conver the response to be lowercase
            for (auto elem : pauseString) {
                pauseString = std::tolower(elem);
            }
            //check the response
            if (pauseString == "yes" || pauseString == "y" || pauseString == "") {
                randomPauseBool = true;
                std::cout << "Random Pauses Enabled" << endl;
                break;
            }
            else if (pauseString == "no" || pauseString == "n") {
                randomPauseBool = false;
                std::cout << "Disabled" << endl;
                break;
            }
            else {
                std::cout << "Invalid entry, try again" << endl;
            }
        }

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

    std::string targetString = "";
    bool windowTargeting = false;
    std::cout << "would you like window targeting? Y/N" << endl;
    std::cin >> targetString;

    //conver the response to be lowercase
    for (auto elem : targetString) {
        targetString = std::tolower(elem);
    }
    //check the response
    if (targetString == "yes" || targetString == "y" || targetString == "") {
        windowTargeting = true;
        std::cout << "Window targeting Enabled" << endl;
    }
    else if (targetString == "no" || targetString == "n") {
        windowTargeting = false;
        std::cout << "Disabled" << endl;
    }
    else {
        std::cout << "Invalid entry, try again" << endl;
    }

    bool autoEat = false;
    std::cout << "Would you like auto right click periodically? Y/N" << endl;
    std::cin >> targetString;

    //conver the response to be lowercase
    for (auto elem : targetString) {
        targetString = std::tolower(elem);
    }
    //check the response
    if (targetString == "yes" || targetString == "y" || targetString == "") {
        autoEat = true;
        std::cout << "Auto right click Enabled" << endl;
    }
    else if (targetString == "no" || targetString == "n") {
        autoEat = false;
        std::cout << "Disabled" << endl;
    }
    else {
        std::cout << "Invalid entry, try again" << endl;
    }

    if (windowTargeting) {
        std::cout << "Select the target window and press Scroll Lock" << endl;
    }
    while (target == NULL) { //Does not leave until target is set
        if (windowTargeting) {
            if (GetKeyState(VK_SCROLL) & 0x8000) { // Checks for scroll lock to be pressed
                target = GetForegroundWindow(); // Sets target as selected window
                if (!target) {
                    std::cout << "Failed to get traget!";
                }
                else {
                    while (GetKeyState(VK_SCROLL) & 0x8000) { //while key is pressed do not progress so that program is not inadvertantly started

                    }
                    std::cout << "Window selected" << endl;
                    break;
                }
            }
        }
        else {
            break;
        }
    }

    std::cout << "Settings completed, press Scroll Lock to start, and Pause Break to stop." << endl;
    
    int clickIndex = 0;

    //main program loop
    while (runProgram) {
        //checks if the clicker loop is to be run
        if (runClicker == true) {
            //if the program has not run, pick a random CPS within the range
            if (hasRun == false) {
                if (topNum - bottomNum >= 0.5f) {
                    currentCPS = getRandomRangeValue(bottomNum, topNum, bottomNum, topNum);
                }
                else {
                    currentCPS = bottomNum;
                }
                hasRun = true;
            }
            //if the program has run, pick a CPS within .5 of the previous CPS to appear fluid, and possibly pause
            else {
                //decide if program should pause
                if (randomPauseBool == true) {
                    randomPause();
                }
                //add the new targets
                float newLow = currentCPS - 0.5;
                float newHigh = currentCPS + 0.5;
                if (topNum - bottomNum >= 0.5f) {
                    currentCPS = getRandomRangeValue(newLow, newHigh, bottomNum, topNum);
                }
                else {
                    currentCPS = bottomNum;
                }
            }
            if (currentCPS < 0) {
                std::cout << "CPS is less than 0" << endl;
                return -1;
            }
            //wait the frametime that correlates to the picked CPS
            Sleep(CPS2Delay(currentCPS));
            if (!windowTargeting) {
                mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); //No targetting
            }
            else {
                PostMessage(target, WM_LBUTTONDOWN, 0, 1 & 2 << 16); // Sends left mouse button down to target window
                PostMessage(target, WM_LBUTTONUP, 0, 1 & 2 << 16); // Sends left mouse button up to target window
            }
            if (autoEat) {
                clickIndex++;
                if (clickIndex >= 100) {
                    if (!windowTargeting) {
                        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0); //No targetting
                        Sleep(2000);
                        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0); //No targetting
                    }
                    else {
                        PostMessage(target, WM_RBUTTONDOWN, 0, 1 & 2 << 16); // Sends left mouse button down to target window
                        Sleep(2000);
                        PostMessage(target, WM_RBUTTONUP, 0, 1 & 2 << 16); // Sends left mouse button up to target window
                    }
                }
            }
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