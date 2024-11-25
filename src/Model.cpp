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

            std::vector<tinyobj::index_t> indices{};
            for (int j = 0; j < sum; ++j) {
                indices.push_back(mesh.indices[index + j]);
            }
            object.triangles.emplace_back(
                std::array{
                    cv::Vec3f{
                        attribute.vertices[3 * indices[0].vertex_index + 0],
                        attribute.vertices[3 * indices[0].vertex_index + 1],
                        attribute.vertices[3 * indices[0].vertex_index + 2]
                    },
                    cv::Vec3f{
                        attribute.vertices[3 * indices[1].vertex_index + 0],
                        attribute.vertices[3 * indices[1].vertex_index + 1],
                        attribute.vertices[3 * indices[1].vertex_index + 2]
                    },
                    cv::Vec3f{
                        attribute.vertices[3 * indices[2].vertex_index + 0],
                        attribute.vertices[3 * indices[2].vertex_index + 1],
                        attribute.vertices[3 * indices[2].vertex_index + 2]
                    }
                },

                std::array{
                    cv::Vec3f{
                        attribute.texcoords[2 * indices[0].texcoord_index + 0],
                        attribute.texcoords[2 * indices[0].texcoord_index + 1],
                        0
                    },
                    cv::Vec3f{
                        attribute.texcoords[2 * indices[1].texcoord_index + 0],
                        attribute.texcoords[2 * indices[1].texcoord_index + 1],
                        0
                    },
                    cv::Vec3f{
                        attribute.texcoords[2 * indices[2].texcoord_index + 0],
                        attribute.texcoords[2 * indices[2].texcoord_index + 1],
                        0
                    }
                },

                std::array{
                    cv::Vec3f{
                        attribute.normals[3 * indices[0].normal_index + 0],
                        attribute.normals[3 * indices[0].normal_index + 1],
                        attribute.normals[3 * indices[0].normal_index + 2]
                    },
                    cv::Vec3f{
                        attribute.normals[3 * indices[1].normal_index + 0],
                        attribute.normals[3 * indices[1].normal_index + 1],
                        attribute.normals[3 * indices[1].normal_index + 2]
                    },
                    cv::Vec3f{
                        attribute.normals[3 * indices[2].normal_index + 0],
                        attribute.normals[3 * indices[2].normal_index + 1],
                        attribute.normals[3 * indices[2].normal_index + 2]
                    },
                }
            );

            index += sum;
        }

        objects.push_back(object);
    }
}
