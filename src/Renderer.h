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
    Renderer(const int width, const int height, const std::string& path): model(path),
                                                                          img(width, height, Color(0, 0, 0)) {
    }

    void run();

    [[nodiscard]] cv::Vec3i world_to_screen(const cv::Vec3f& v) const;

    [[nodiscard]] cv::Vec3f screen_to_world(const cv::Vec3i& v) const;

private:
    Model model;
    Image img;
    const int screen_depth = 10;
    const cv::Vec3f light_dir = cv::Vec3f{0, 0, -1};

    void draw_edge();

    void draw_triangle();
};


#endif //RENDERER_H
