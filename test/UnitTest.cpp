//
// Created by 13270 on 24-10-9.
//
#include <iostream>
#include "Image.h"

int main() {
    // Image img(128, 128, Color(255, 0, 0, 255));
    Image img("test2.png");
    img.write("test3.png");

    return 0;
}
