#include <iostream>
#include <windows.h>

using namespace std;

float getRandomCPS(float bottom, float top, float bottomLimit, float topLimit) {
    bool fuckMe = true; // fuckMe? fuckYou
    float CPS;
    while (fuckMe) {
        CPS = ((top - bottom) * ((float)rand() / RAND_MAX)) + bottom;
        if (CPS > bottomLimit&& CPS < topLimit) {
            fuckMe = false;
        }
    }
    std::cout << CPS << endl;
    return CPS;
};

int CPS2Delay(float clicks) {
    int result = round(1000 / clicks);
    return result;
};

bool KeyIsPressed(unsigned char k) {
    USHORT status = GetAsyncKeyState(k);
    return (((status & 0x8000) >> 15) == 1) || ((status & 1) == 1);
}

int main()
{
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

    bool runClicker = false;
    bool runProgram = true;
    bool hasRun = false;
    float currentCPS = 0.0;
    HWND target = NULL; // Target window declaration

    std::cout << "Select the target window and press Scroll Lock" << endl;

    while (target == NULL) { //Does not leave until target is set
        if (GetKeyState(VK_SCROLL) & 0x8000) { // Checks for scroll lock to be pressed
            target = GetForegroundWindow(); // Sets target as selected window
            break;
        }
    }
    std::cout << "Window selected" << endl;

    while (runProgram) {
        if (runClicker == true) {
            if (hasRun == false) {
                currentCPS = getRandomCPS(bottomNum, topNum, bottomNum, topNum);
                hasRun = true;
            }
            else {
                float newLow = currentCPS - .5;
                float newHigh = currentCPS + .5;
                currentCPS = getRandomCPS(newLow, newHigh, bottomNum, topNum);
            }
            Sleep(CPS2Delay(currentCPS));
            //mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0); // Old Clicker code
            PostMessage(target, WM_LBUTTONDOWN, 0, 1 & 2 << 16); // Sends left mouse button down to target window
            PostMessage(target, WM_LBUTTONUP, 0, 1 & 2 << 16); // Sends left mouse button up to target window
            if (GetKeyState(VK_PAUSE) & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
                runClicker = false;
            }
        }
        else if (GetKeyState(VK_SCROLL) & 0x8000/*Check if high-order bit is set (1 << 15)*/) {
            runClicker = true;
        }
        else {
            Sleep(17);
        }
    }

}