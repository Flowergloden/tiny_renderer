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

void Image::write(const std::string& path) {
    imwrite(path, image);
}
