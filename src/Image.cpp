//
// Created by fgolden on 24-11-7.
//
#include "Image.h"

Image::Image(const std::string& path) {
    image = imread(path, cv::IMREAD_COLOR);
    width = image.cols;
    height = image.rows;
}

Image::Image(const int width, const int height, const Color& color) {
    image = cv::Mat(width, height, CV_8UC3, cv::Scalar(color.b, color.g, color.r));
    this->width = width;
    this->height = height;
}

void Image::set(const int x, const int y, const Color& color) {
    image.at<cv::Vec3b>(y, x) = cv::Vec3b(color.b, color.g, color.r);
}

void Image::write(const std::string& path) const {
    imwrite(path, image);
}

void Image::line(int x0, int y0, int x1, int y1, const Color& color) {
    bool steep = false;
    if (abs(x1 - x0) < abs(y1 - y0)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    for (int x = x0; x <= x1; ++x) {
        const float t = static_cast<float>(x - x0) / static_cast<float>(x1 - x0);
        const int y = static_cast<int>(static_cast<float>(y0) + static_cast<float>(y1 - y0) * t);
        if (steep) {
            set(y, x, color);
        } else {
            set(x, y, color);
        }
    }
}
