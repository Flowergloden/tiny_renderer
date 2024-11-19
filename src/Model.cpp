//
// Created by fg on 24-11-18.
//

#include "Model.h"

Model::Model(const std::string& path) {
    auto obj = tinyobj::ObjReader();
    obj.ParseFromFile(path);

    if (!obj.Valid()) {
        std::cerr << "Failed to load " << path << std::endl;
    }

    const auto attribute = obj.GetAttrib();

    for (const auto& [name, mesh, lines, points]: obj.GetShapes()) {
        Object object;
        object.name = name;

        for (const auto& [vertex_index, _n, _t]: points.indices) {
            object.vertices.push_back(cv::Vec3f{
                attribute.vertices[3 * vertex_index + 0],
                attribute.vertices[3 * vertex_index + 1],
                attribute.vertices[3 * vertex_index + 2]
            });
        }

        for (int i = 0, index = 0; i < mesh.num_face_vertices.size(); ++i) {
            const int sum = static_cast<int>(mesh.num_face_vertices[i]);

            // TODO: deal with non-triangle faces
            if (sum != 3) {
                std::cerr << "Object " << name << " has non-triangle faces" << std::endl;
            }

            std::vector<int> vertices{};
            for (int j = 0; j < sum; ++j) {
                vertices.push_back(3 * mesh.indices[index + j].vertex_index);
            }
            object.triangles.emplace_back(
                cv::Vec3f{
                    attribute.vertices[vertices[0] + 0],
                    attribute.vertices[vertices[0] + 1],
                    attribute.vertices[vertices[0] + 2]
                },
                cv::Vec3f{
                    attribute.vertices[vertices[1] + 0],
                    attribute.vertices[vertices[1] + 1],
                    attribute.vertices[vertices[1] + 2]
                },
                cv::Vec3f{
                    attribute.vertices[vertices[2] + 0],
                    attribute.vertices[vertices[2] + 1],
                    attribute.vertices[vertices[2] + 2]
                }
            );

            index += sum;
        }

        objects.push_back(object);
    }
}
