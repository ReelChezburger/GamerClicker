#include <iostream>
#include <windows.h>

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