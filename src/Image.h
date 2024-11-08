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
    uint8_t a;

    Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) : r(r), g(g), b(b), a(a) {
    }
};

class Image {
public:
    explicit Image(const std::string& path);

    Image(size_t width, size_t height, const Color& color);

    void set(size_t x, size_t y, const Color& color);

    void write(const std::string& path);

private:
    cv::Mat image{};
};

#endif //IMAGE_H
