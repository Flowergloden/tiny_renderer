//
// Created by fg on 24-11-17.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <array>
#include <opencv2/core/matx.hpp>


class Triangle {
public:
    Triangle(const cv::Vec3f& p0, const cv::Vec3f& p1, const cv::Vec3f& p2,
             const cv::Vec3f& t0, const cv::Vec3f& t1, const cv::Vec3f& t2,
             const cv::Vec3f& n0, const cv::Vec3f& n1, const cv::Vec3f& n2) {
        points[0] = p0;
        points[1] = p1;
        points[2] = p2;

        tex_coors[0] = t0;
        tex_coors[1] = t1;
        tex_coors[2] = t2;

        normals[0] = n0;
        normals[1] = n1;
        normals[2] = n2;
    }

    Triangle(const std::array<cv::Vec3f, 3>&& points, const std::array<cv::Vec3f, 3>&& tex_coors,
             const std::array<cv::Vec3f, 3>&& normals): points(points),
                                                        normals(normals), tex_coors(tex_coors) {
    }

    const std::array<cv::Vec2f, 2>& get_bounding_box();

    [[nodiscard]] cv::Vec3f barycentric(const cv::Vec2f& p) const;

    [[nodiscard]] const std::array<cv::Vec3f, 3>& get_points() const { return points; }
    [[nodiscard]] const std::array<cv::Vec3f, 3>& get_tex_coors() const { return tex_coors; }
    [[nodiscard]] const std::array<cv::Vec3f, 3>& get_normals() const { return normals; }

private:
    std::array<cv::Vec3f, 3> points{};
    std::array<cv::Vec3f, 3> normals{};
    std::array<cv::Vec3f, 3> tex_coors{};
    std::array<cv::Vec2f, 2> bbox;

    bool data_changed = true;
};


#endif //TRIANGLE_H
