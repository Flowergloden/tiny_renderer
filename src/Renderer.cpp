//
// Created by fgolden on 24-11-13.
//

#include "Renderer.h"

Renderer::Renderer(const int width, const int height, const std::string& path, Camera camera, bool print_z_buffer)
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

    view_matrix = camera.get_view_matrix().inv();
    is_perspective = camera.projection_type == Camera::ProjectionType::perspective;

    viewport_matrix = cv::Matx44f{
        static_cast<float>(width) / 2, 0, 0, static_cast<float>(width) / 2,
        0, static_cast<float>(height) / 2, 0, static_cast<float>(height) / 2,
        0, 0, screen_depth / 2, screen_depth / 2,
        0, 0, 0, 1
    };

    auto transform_matrix = view_matrix * model_matrix;
    cv::Vec4f right = transform_matrix * cv::Vec4f{camera.position[0] + camera.width / 2, 0, 0, 1};
    cv::Vec4f left = transform_matrix * cv::Vec4f{camera.position[0] - camera.width / 2, 0, 0, 1};
    cv::Vec4f top = transform_matrix * cv::Vec4f{0, camera.position[1] + camera.height / 2, 0, 1};
    cv::Vec4f bottom = transform_matrix * cv::Vec4f{0, camera.position[1] - camera.height / 2, 0, 1};
    cv::Vec4f near = transform_matrix * cv::Vec4f{0, 0, camera.position[2] + camera.near_distance, 1};
    cv::Vec4f far = transform_matrix * cv::Vec4f{0, 0, camera.position[2] + camera.far_distance, 1};

    float l = left[0] / left[3];
    float r = right[0] / right[3];
    float t = top[1] / top[3];
    float b = bottom[1] / bottom[3];
    float n = near[2] / near[3];
    float f = far[2] / far[3];

    if (is_perspective) {
        perspective_matrix =
                cv::Matx44f{
                    n, 0, 0, 0,
                    0, n, 0, 0,
                    0, 0, n + f, -n * f,
                    0, 0, 1, 0
                }.inv();
        transform_matrix = perspective_matrix * transform_matrix;
        left = transform_matrix * left;
        right = transform_matrix * right;
        top = transform_matrix * top;
        bottom = transform_matrix * bottom;
        near = transform_matrix * near;
        far = transform_matrix * far;

        l = left[0] / left[3];
        r = right[0] / right[3];
        t = top[1] / top[3];
        b = bottom[1] / bottom[3];
        n = near[2] / near[3];
        f = far[2] / far[3];
    }

    orthographic_matrix =
            cv::Matx44f{
                2 / (r - l), 0, 0, 0,
                0, 2 / (t - b), 0, 0,
                0, 0, 2 / (n - f), 0,
                0, 0, 0, 1
            }
            * cv::Matx44f{
                1, 0, 0, -(r + l) / 2,
                0, 1, 0, -(t + b) / 2,
                0, 0, 1, -(n + f) / 2,
                0, 0, 0, 1,
            };


    projection_matrix = is_perspective ? perspective_matrix : orthographic_matrix * perspective_matrix;
}

void Renderer::run() {
    // draw_edge();
    draw_triangle();

    img.write("output.png");
}

cv::Vec3i Renderer::world_to_screen(const cv::Vec3f& v) const {
    cv::Vec4f res = viewport_matrix * cv::Vec4f{v[0], v[1], v[2], 1};

    return cv::Vec3i{
        static_cast<int>(res[0] / res[3]), static_cast<int>(res[1] / res[3]), static_cast<int>(res[2] / res[3])
    };
}

cv::Vec3f Renderer::screen_to_world(const cv::Vec3i& v) const {
    cv::Vec4f res = viewport_matrix.inv() * cv::Vec4f{
                        static_cast<float>(v[0]), static_cast<float>(v[1]), static_cast<float>(v[2]), 1
                    };

    return cv::Vec3f{res[0] / res[3], res[1] / res[3], res[2] / res[3]};
}

void Renderer::draw_edge() {
    for (auto& object: model.objects) {
        for (auto& triangle: object.triangles) {
            const auto& points = triangle.get_points();
            img.line(world_to_screen(points[0])[0], world_to_screen(points[0])[1], world_to_screen(points[1])[0],
                     world_to_screen(points[1])[1], Color(255, 255, 255));
            img.line(world_to_screen(points[1])[0], world_to_screen(points[1])[1], world_to_screen(points[2])[0],
                     world_to_screen(points[2])[1], Color(255, 255, 255));
            img.line(world_to_screen(points[2])[0], world_to_screen(points[2])[1], world_to_screen(points[0])[0],
                     world_to_screen(points[0])[1], Color(255, 255, 255));
        }
    }
}

void Renderer::draw_triangle() {
    const cv::Matx44f mvp = projection_matrix * view_matrix * model_matrix;
    for (auto& object: model.objects) {
        for (auto& triangle: object.triangles) {
            auto origin_points = triangle.get_points();
            cv::Vec4f pt1 = mvp * cv::Vec4f{origin_points[0][0], origin_points[0][1], origin_points[0][2], 1.0f};
            cv::Vec4f pt2 = mvp * cv::Vec4f{origin_points[1][0], origin_points[1][1], origin_points[1][2], 1.0f};
            cv::Vec4f pt3 = mvp * cv::Vec4f{origin_points[2][0], origin_points[2][1], origin_points[2][2], 1.0f};
            Triangle transformed_triangle{
                std::array{
                    cv::Vec3f{
                        pt1[0] / pt1[3],
                        pt1[1] / pt1[3],
                        pt1[2] / pt1[3],
                    },
                    cv::Vec3f{
                        pt2[0] / pt2[3],
                        pt2[1] / pt2[3],
                        pt2[2] / pt2[3],
                    },
                    cv::Vec3f{
                        pt3[0] / pt3[3],
                        pt3[1] / pt3[3],
                        pt3[2] / pt3[3],
                    }
                },
                std::array{triangle.get_tex_coors()},
                std::array{triangle.get_normals()},
            };
            auto normals = transformed_triangle.get_normals();
            auto points = transformed_triangle.get_points();

            // BUG: some faces unexpectedly culled
            // // back-face culling
            // if (normals[0].dot(normalize(points[0] - camera.position)) < 0
            //     && normals[1].dot(normalize(points[1] - camera.position)) < 0
            //     && normals[2].dot(normalize(points[2] - camera.position)) < 0) {
            //     continue;
            // }

            auto bbox = transformed_triangle.get_bounding_box();
            auto left_bottom = world_to_screen(cv::Vec3f{bbox[0][0], bbox[0][1], 0});
            auto right_top = world_to_screen(cv::Vec3f{bbox[1][0], bbox[1][1], 0});
            for (int x = left_bottom[0]; x <= right_top[0]; ++x) {
                if (x < 0 || x >= img.width()) { continue; }

                for (int y = left_bottom[1]; y <= right_top[1]; ++y) {
                    if (y < 0 || y >= img.height()) { continue; }

                    // calculate barycentric coordinates with world position
                    auto p = screen_to_world(cv::Vec3i{x, y, 0});
                    auto bc = transformed_triangle.barycentric(cv::Vec2f{p[0], p[1]});
                    if (bc[0] < 0 || bc[1] < 0 || bc[2] < 0) { continue; }

                    // calculate z value used by z-buffer
                    p[2] = 0;
                    for (int i = 0; i < 3; ++i) {
                        p[2] += points[i][2] * bc[i];
                    }

                    if (z_buffer[y][x] < p[2]) {
                        z_buffer[y][x] = p[2];

                        if (print_z_buffer) {
                            const auto z = (p[2] - camera.near_distance + screen_depth / 2) / screen_depth;
                            const Color color{
                                static_cast<uint8_t>(z * 255),
                                static_cast<uint8_t>(z * 255),
                                static_cast<uint8_t>(z * 255),
                            };
                            img.set(x, y, color);
                        } else {
                            // interpolation uv
                            const auto& tex_coors = transformed_triangle.get_tex_coors();
                            cv::Vec3f uv = tex_coors[0] * bc[0] + tex_coors[1] * bc[1] + tex_coors[2] * bc[2];
                            const auto tex_color = texture.get(
                                static_cast<int>(uv[0] * static_cast<float>(texture.width())),
                                static_cast<int>(uv[1] * static_cast<float>(texture.height())));

                            // interpolation normal
                            cv::Vec3f n = normalize(normals[0] * bc[0] +
                                                    normals[1] * bc[1] +
                                                    normals[2] * bc[2]);
                            const float intensity = n.dot(light_dir);
                            if (intensity < 0) continue;

                            const Color color(static_cast<uint8_t>(intensity * static_cast<float>(tex_color[2])),
                                              static_cast<uint8_t>(intensity * static_cast<float>(tex_color[1])),
                                              static_cast<uint8_t>(intensity * static_cast<float>(tex_color[0])));
                            img.set(x, y, color);
                        }
                    }
                }
            }
        }
    }
}
