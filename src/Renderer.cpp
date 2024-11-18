//
// Created by fgolden on 24-11-13.
//

#include "Renderer.h"
#include <random>

void Renderer::run() {
    // draw_edge();
    draw_triangle();

    img.write("output.png");
}

cv::Vec3i Renderer::world_to_screen(const cv::Vec3f& v) const {
    return cv::Vec3i{
        static_cast<int>((v[0] + 1.) * img.width() / 2),
        static_cast<int>((v[1] + 1.) * img.height() / 2),
        static_cast<int>((v[2] + 1.) * screen_depth / 2)
    };
}

cv::Vec3f Renderer::screen_to_world(const cv::Vec3i& v) const {
    return cv::Vec3f{
        static_cast<float>(v[0] * 2) / static_cast<float>(img.width()) - 1.f,
        static_cast<float>(v[1] * 2) / static_cast<float>(img.height()) - 1.f,
        static_cast<float>(v[2] * 2) / static_cast<float>(screen_depth) - 1.f
    };
}

void Renderer::draw_edge() {
    // const auto& attrib = obj.GetAttrib();
    //
    // for (const auto& [name, mesh, lines, points]: obj.GetShapes()) {
    //     const auto& faces = mesh.num_face_vertices;
    //     const auto& indices = mesh.indices;
    //     for (int i = 0, index = 0; i < faces.size(); ++i) {
    //         const int sum = static_cast<int>(faces[i]);
    //
    //         for (int j = 0; j < sum; ++j) {
    //             const float u0 = attrib.vertices[3 * indices[index + j].vertex_index + 0];
    //             const float v0 = attrib.vertices[3 * indices[index + j].vertex_index + 1];
    //
    //             const float u1 = attrib.vertices[3 * indices[index + (j + 1) % sum].vertex_index + 0];
    //             const float v1 = attrib.vertices[3 * indices[index + (j + 1) % sum].vertex_index + 1];
    //
    //             const int x0 = static_cast<int>((u0 + 1.) * (img.width() - 1.) / 2);
    //             const int y0 = static_cast<int>((v0 + 1.) * (img.height() - 1.) / 2);
    //
    //             const int x1 = static_cast<int>((u1 + 1.) * (img.width() - 1.) / 2);
    //             const int y1 = static_cast<int>((v1 + 1.) * (img.height() - 1.) / 2);
    //             img.line(x0, y0, x1, y1, Color(255, 255, 255));
    //         }
    //
    //         index += sum;
    //     }
    // }
}

void Renderer::draw_triangle() {
    // TEST ONLY
    std::mt19937 mt{
        static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count())
    };

    for (auto& object: model.objects) {
        for (auto& triangle: object.triangles) {
            // TEST ONLY
            const Color color(mt() % 255, mt() % 255, mt() % 255);

            auto bbox = triangle.get_bounding_box();
            auto left_bottom = world_to_screen(cv::Vec3f{bbox[0][0], bbox[0][1], 0});
            auto right_top = world_to_screen(cv::Vec3f{bbox[1][0], bbox[1][1], 0});
            for (int x = left_bottom[0]; x <= right_top[0]; ++x) {
                if (x < 0 || x > img.width()) { continue; }

                for (int y = left_bottom[1]; y <= right_top[1]; ++y) {
                    if (y < 0 || y > img.height()) { continue; }

                    auto p = screen_to_world(cv::Vec3i{x, y, 0});
                    if (auto bc = triangle.barycentric(cv::Vec2f{p[0], p[1]}); bc[0] < 0 || bc[1] < 0 || bc[2] < 0) {
                        continue;
                    }
                    img.set(x, y, color);
                }
            }
        }
    }
}
