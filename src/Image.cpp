//
// Created by fgolden on 24-11-7.
//
#include "Image.h"

Image::Image(const std::string& path) {
    image = imread(path, cv::IMREAD_COLOR);
}

Image::Image(const int width, const int height, const Color& color) {
    image = cv::Mat(width, height, CV_8UC4, cv::Scalar(color.b, color.g, color.r, color.a));
}

void Image::set(int x, int y, const Color& color) {
    // TODO: Not implemented
    return;
}

void Image::write(const std::string& path) {
    imwrite(path, image);
}
