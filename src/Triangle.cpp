//
// Created by fg on 24-11-17.
//

#include "Triangle.h"

const std::array<cv::Vec2f, 2>& Triangle::get_bounding_box() {
    if (data_changed) {
        std::array bbox{cv::Vec2f(INT_MAX, INT_MAX), cv::Vec2f(INT_MIN, INT_MIN)};

        for (int i = 0; i < 3; ++i) {
            bbox[0][0] = std::min(bbox[0][0], points[i][0]);
            bbox[0][1] = std::min(bbox[0][1], points[i][1]);

            bbox[1][0] = std::max(bbox[1][0], points[i][0]);
            bbox[1][1] = std::max(bbox[1][1], points[i][1]);
        }
        this->bbox = bbox;
        data_changed = false;
    }

    return bbox;
}

cv::Vec3f Triangle::barycentric(const cv::Vec2f& p) const {
    std::array<cv::Vec3f, 3> pts = points;
    cv::Vec3f u = cv::Vec3f{
                (pts[2][0] - pts[0][0]),
                (pts[1][0] - pts[0][0]),
                (pts[0][0] - p[0])
            }
            .cross(cv::Vec3f{
                (pts[2][1] - pts[0][1]),
                (pts[1][1] - pts[0][1]),
                (pts[0][1] - p[1])
            });

    if (u[2] == 0)return cv::Vec3f{-1, 1, 1};
    return cv::Vec3f{1 - (u[0] + u[1]) / u[2], u[1] / u[2], u[0] / u[2]};
}
