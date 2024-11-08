//
// Created by fgolden on 24-11-7.
//
#include "Image.h"

Image::Image(const std::string& path) {
    image = cv::imread(path, cv::IMREAD_COLOR);
}

Image::Image(size_t width, size_t height, const Color& color) {
}
