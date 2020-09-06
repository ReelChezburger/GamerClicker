#include <stdexcept>
#include <limits>
#include <iostream>

void negativeTest(float c) {
    if (c < 0) {
        throw std::invalid_argument("value is less than 0");
    }
}