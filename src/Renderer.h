//
// Created by fgolden on 24-11-13.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <tiny_obj_loader.h>

#include <utility>
#include "Image.h"
#include "Model.h"
#include "Camera.h"

class Renderer {
public:
    // TODO: use .mtl instead texture
    Renderer(const int width, const int height, const std::string& path, Camera camera, bool print_z_buffer = false)
        : model(path),
          img(width, height, Color(0, 0, 0)),
          texture("african_head_diffuse.png"),
          camera(std::move(camera)),
          print_z_buffer(print_z_buffer) {
        for (int i = 0; i < height; ++i) {
            z_buffer.emplace_back();
            for (int j = 0; j < width; ++j) {
                z_buffer[i].push_back(0);
            }
        }

        view_matrix = camera.get_view_matrix();
        is_perspective = camera.projection_type == Camera::ProjectionType::perspective;

        viewport_matrix = cv::Matx44f{
            static_cast<float>(width) / 2, 0, 0, static_cast<float>(width) / 2,
            0, static_cast<float>(height) / 2, 0, static_cast<float>(height) / 2,
            0, 0, screen_depth / 2, screen_depth / 2,
            0, 0, 0, 1
        };
    }

    void run();

    [[nodiscard]] cv::Vec3i world_to_screen(const cv::Vec3f& v) const;

    [[nodiscard]] cv::Vec3f screen_to_world(const cv::Vec3i& v) const;

private:
    Model model;
    Image img;
    std::vector<std::vector<float>> z_buffer{};
    const float screen_depth = 2;
    const cv::Vec3f light_dir = normalize(cv::Vec3f{1, 1, 1}); // use left-hand coordinate system
    Image texture;
    Camera camera;
    bool print_z_buffer;

    cv::Matx44f view_matrix;
    bool is_perspective;
    const cv::Matx44f perspective_matrix =
            cv::Matx44f{
                camera.near_distance, 0, 0, 0,
                0, camera.near_distance, 0, 0,
                0, 0, camera.near_distance + camera.far_distance, -camera.near_distance * camera.far_distance,
                0, 0, 1, 0
            }.inv();

    const cv::Matx44f orthographic_matrix =
            cv::Matx44f{
                2 / camera.width, 0, 0, 0,
                0, 2 / camera.height, 0, 0,
                0, 0, 2 / (camera.far_distance - camera.near_distance), 0,
                0, 0, 0, 1
            }
            * cv::Matx44f{
                // TODO
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1,
            };

    const cv::Matx44f model_matrix{
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    cv::Matx44f viewport_matrix;

    void draw_edge();

    void draw_triangle();
};


#endif //RENDERER_H
