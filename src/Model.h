//
// Created by fg on 24-11-18.
//

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <tiny_obj_loader.h>
#include <iostream>
#include "Triangle.h"

struct Object {
    std::string name;
    std::vector<cv::Vec3f> vertices{};
    std::vector<Triangle> triangles{};
};

class Model {
public:
    explicit Model(const std::string& path);
    std::vector<Object> objects;
};


#endif //MODEL_H
