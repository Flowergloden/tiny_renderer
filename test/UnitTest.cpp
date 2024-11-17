//
// Created by 13270 on 24-10-9.
//
#include <iostream>

#include "Renderer.h"

int main() {
    // Image img(128, 128, Color(255, 0, 0, 255));
    // Image img("test2.png");
    // img.set(0, 0, Color(255, 0, 0));
    // img.write("test3.png");

    Image lineImg(100, 100, Color(255, 255, 255));

    lineImg.line(40, 90, 10, 10, Color(255, 0, 0));
    lineImg.line(10, 10, 90, 10, Color(0, 255, 0));
    lineImg.line(90, 10, 60, 90, Color(0, 0, 255));

    lineImg.write("line_test.png");

    Renderer renderer(4096, 4096);
    renderer.run("african_head.obj");

    Triangle triangle{cv::Vec3i{0, 0, 0}, cv::Vec3i{1, 0, 0}, cv::Vec3i{0, 1, 0}};

    std::cout << "min: " << triangle.get_bounding_box()[0][0] << ", " << triangle.get_bounding_box()[0][1] << std::endl;
    std::cout << "max: " << triangle.get_bounding_box()[1][0] << ", " << triangle.get_bounding_box()[1][1] << std::endl;

    cv::Vec2i p{1, 1};
    std::cout << "p: " << p[0] << ", " << p[1] << std::endl;
    std::cout << "barycentric: " << triangle.barycentric(p)[0] << ", " << triangle.barycentric(p)[1] << ", " << triangle
            .barycentric(p)[2] << std::endl;

    return 0;
}
