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
    Renderer(int width, int height, const std::string& path, Camera camera, bool print_z_buffer = false);

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
    cv::Matx44f perspective_matrix;

    cv::Matx44f orthographic_matrix;
    cv::Matx44f projection_matrix;

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
