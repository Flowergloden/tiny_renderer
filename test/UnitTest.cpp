//
// Created by 13270 on 24-10-9.
//
#include <iostream>
#include "Image.h"

int main() {
    // Image img(128, 128, Color(255, 0, 0, 255));
    Image img("test2.png");
    img.set(0, 0, Color(255, 0, 0));
    img.write("test3.png");

    Image lineImg(100, 100, Color(255, 255, 255));

    lineImg.line(40, 90, 10, 10, Color(255, 0, 0));

    lineImg.write("line_test.png");

    return 0;
}
