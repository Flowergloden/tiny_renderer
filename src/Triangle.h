//
// Created by fg on 24-11-17.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <array>
#include <opencv2/core/matx.hpp>


class Triangle {
public:
    Triangle(const cv::Vec3i& p0, const cv::Vec3i& p1, const cv::Vec3i& p2) {
        points[0] = p0;
        points[1] = p1;
        points[2] = p2;
    }

    const std::array<cv::Vec2i, 2>& get_bounding_box();

    cv::Vec3f barycentric(const cv::Vec2i& p);

private:
    std::array<cv::Vec3i, 3> points{};
    std::array<cv::Vec2i, 2> bbox;

    bool data_changed = true;
};


#endif //TRIANGLE_H
