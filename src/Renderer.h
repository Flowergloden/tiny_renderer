//
// Created by fgolden on 24-11-13.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <tiny_obj_loader.h>
#include "Image.h"
#include "Triangle.h"

class Renderer {
public:
    Renderer(const int width, const int height): img(width, height, Color(0, 0, 0)) {
    }

    void run(const std::string& path);

private:
    tinyobj::ObjReader obj;
    Image img;

    void draw_edge();
};


#endif //RENDERER_H
