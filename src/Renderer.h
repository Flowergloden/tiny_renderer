//
// Created by fgolden on 24-11-13.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <tiny_obj_loader.h>
#include "Image.h"
#include "Model.h"

class Renderer {
public:
    // TODO: use .mtl instead texture
    Renderer(const int width, const int height, const std::string& path): model(path),
                                                                          img(width, height, Color(0, 0, 0)),
                                                                          texture("african_head_diffuse.png") {
        for (int i = 0; i < height; ++i) {
            z_buffer.emplace_back();
            for (int j = 0; j < width; ++j) {
                z_buffer[i].push_back(0);
            }
        }
    }

    void run();

    [[nodiscard]] cv::Vec3i world_to_screen(const cv::Vec3f& v) const;

    [[nodiscard]] cv::Vec3f screen_to_world(const cv::Vec3i& v) const;

private:
    Model model;
    Image img;
    std::vector<std::vector<float>> z_buffer{};
    const int screen_depth = 10;
    const cv::Vec3f light_dir = normalize(cv::Vec3f{1, 1, 1}); // use left-hand coordinate system
    Image texture;

    // TODO: replace this with complete camera def
    const cv::Vec3f camera_position{0, 0, -1};
    const float near_distance = 1;
    const float far_distance = 100;
    const float top_distance = 1;
    const float bottom_distance = -1;
    const float right_distance = 1;
    const float left_distance = -1;
    const cv::Matx44f view_matrix{
        1, 0, 0, camera_position[0],
        0, 1, 0, camera_position[1],
        0, 0, 1, camera_position[2],
        0, 0, 0, 1
    };
    // TODO: config this
    bool is_perspective = true;
    const cv::Matx44f perspective_matrix{
        near_distance, 0, 0, 0,
        0, near_distance, 0, 0,
        0, 0, near_distance + far_distance, -near_distance * far_distance,
        0, 0, 1, 0
    };

    const cv::Matx44f orthographic_matrix{
        (right_distance - left_distance) / 2, 0, 0, 0,
        0, (top_distance - bottom_distance) / 2, 0, 0,
        0, 0, (far_distance - near_distance) / 2, 0,
        0, 0, 0, 1
    };

    const cv::Matx44f model_matrix{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    void draw_edge();

    void draw_triangle();
};


#endif //RENDERER_H
