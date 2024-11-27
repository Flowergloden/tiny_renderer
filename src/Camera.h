//
// Created by fg on 24-11-26.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <opencv2/core.hpp>

class Camera {
public:
    enum ProjectionType {
        perspective,
        orthographic,
    };

    Camera(const float fov, const float aspect_ratio,
           const float near_distance, const float far_distance,
           const cv::Vec3f& position, const cv::Vec3f& target_point = cv::Vec3f{0, 0, 0},
           const cv::Vec3f& up = cv::Vec3f{0, 1, 0},
           const ProjectionType projection_type = perspective)
        : fov(fov),
          aspect_ratio(aspect_ratio),
          near_distance(near_distance),
          far_distance(far_distance),
          position(position),
          target_point(target_point),
          up(up),
          projection_type(projection_type) {
        height = 2.f * near_distance * std::tan(fov / 2.f);
        width = height * aspect_ratio;
    }

    float fov;
    float aspect_ratio;
    float near_distance;
    float far_distance;
    cv::Vec3f position;
    cv::Vec3f target_point;
    cv::Vec3f up;
    ProjectionType projection_type;

    float height;
    float width;

    // BUG: incorrect rotation
    [[nodiscard]] cv::Matx44f get_view_matrix() const {
        const cv::Vec3f z = normalize(target_point - position);
        const cv::Vec3f x = normalize(normalize(up).cross(z));
        const cv::Vec3f y = z.cross(x);

        return cv::Matx44f{
            x[0], x[1], x[2], position[0],
            y[0], y[1], y[2], position[1],
            z[0], z[1], z[2], position[2],
            0, 0, 0, 1
        }.inv();
    }
};


#endif //CAMERA_H
