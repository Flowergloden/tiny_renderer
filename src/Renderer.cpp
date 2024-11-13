//
// Created by fgolden on 24-11-13.
//

#include "Renderer.h"

void Renderer::run(const std::string& path) {
    if (obj.ParseFromFile(path)) {
        draw_edge();
    }

    img.write("output.png");
}

void Renderer::draw_edge() {
    const auto& attrib = obj.GetAttrib();

    for (const auto& [name, mesh, lines, points]: obj.GetShapes()) {
        const auto& faces = mesh.num_face_vertices;
        const auto& indices = mesh.indices;
        for (int i = 0, index = 0; i < faces.size(); ++i) {
            const int sum = static_cast<int>(faces[i]);

            for (int j = 0; j < sum; ++j) {
                const float u0 = attrib.vertices[3 * indices[index + j].vertex_index + 0];
                const float v0 = attrib.vertices[3 * indices[index + j].vertex_index + 1];

                const float u1 = attrib.vertices[3 * indices[index + (j + 1) % sum].vertex_index + 0];
                const float v1 = attrib.vertices[3 * indices[index + (j + 1) % sum].vertex_index + 1];

                const int x0 = static_cast<int>((u0 + 1.) * (img.width() - 1.) / 2);
                const int y0 = static_cast<int>((v0 + 1.) * (img.height() - 1.) / 2);

                const int x1 = static_cast<int>((u1 + 1.) * (img.width() - 1.) / 2);
                const int y1 = static_cast<int>((v1 + 1.) * (img.height() - 1.) / 2);
                img.line(x0, y0, x1, y1, Color(255, 255, 255));
            }

            index += sum;
        }
    }
}
