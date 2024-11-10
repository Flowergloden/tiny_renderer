//
// Created by fgolden on 24-11-7.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <cstdint>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b) {
    }
};

class Image {
public:
    explicit Image(const std::string& path);

    Image(int width, int height, const Color& color);

    void set(int x, int y, const Color& color);

    void write(const std::string& path);

private:
    cv::Mat image{};
    int width;
    int height;
};

#endif //IMAGE_H
