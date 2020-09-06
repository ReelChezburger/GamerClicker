#include <iostream>
#include <windows.h>

using namespace std;

float getRandomCPS(float bottom, float top, float bottomLimit, float topLimit) {
    bool fuckMe = true;
    float CPS;
    while (fuckMe) {
        CPS = ((top - bottom) * ((float)rand() / RAND_MAX)) + bottom;
        if (CPS > bottomLimit && CPS < topLimit) {
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

int main()
{
    float bottomNum;
    float topNum;
    std::cout << "Enter lowest target CPS..." << endl;
    std::cin >> bottomNum;
    std::cout << "Enter highest target CPS..." << endl;
    std::cin >> topNum;

    bool runClicker = false;
    bool runProgram = true;
    bool hasRun = false;
    float currentCPS = 0.0;
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
            mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
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