//
// Created by fgolden on 24-11-13.
//

#include "Renderer.h"

void Renderer::run() {
    // draw_edge();
    draw_triangle();

    img.write("output.png");
}

cv::Vec3i Renderer::world_to_screen(const cv::Vec3f& v) const {
    return cv::Vec3i{
        static_cast<int>((v[0] + 1.) * (img.width() - 1.) / 2),
        static_cast<int>((v[1] + 1.) * (img.height() - 1.) / 2),
        static_cast<int>((v[2] + 1.) * (screen_depth - 1.) / 2)
    };
}

cv::Vec3f Renderer::screen_to_world(const cv::Vec3i& v) const {
    return cv::Vec3f{
        static_cast<float>(v[0] * 2) / static_cast<float>(img.width() - 1.) - 1.f,
        static_cast<float>(v[1] * 2) / static_cast<float>(img.height() - 1.) - 1.f,
        static_cast<float>(v[2] * 2) / static_cast<float>(screen_depth - 1.) - 1.f
    };
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
    const auto projection_matrix = is_perspective ? orthographic_matrix * perspective_matrix : orthographic_matrix;
    const cv::Matx44f mvp = projection_matrix.inv() * view_matrix.inv() * model_matrix.inv();
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

            // back-face culling
            if (normals[0].dot(normalize(points[0] - camera_position)) < 0
                && normals[1].dot(normalize(points[1] - camera_position)) < 0
                && normals[2].dot(normalize(points[2] - camera_position)) < 0) {
                continue;
            }

            auto bbox = transformed_triangle.get_bounding_box();
            auto left_bottom = world_to_screen(cv::Vec3f{bbox[0][0], bbox[0][1], 0});
            auto right_top = world_to_screen(cv::Vec3f{bbox[1][0], bbox[1][1], 0});
            for (int x = left_bottom[0]; x <= right_top[0]; ++x) {
                if (x < 0 || x >= img.width()) { continue; }

                for (int y = left_bottom[1]; y <= right_top[1]; ++y) {
                    if (y < 0 || y >= img.height()) { continue; }

                    // calculate barycentric coordinates with world positon
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
