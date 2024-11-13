//
// Created by 13270 on 24-10-9.
//
#include <tiny_obj_loader.h>
#include "Image.h"

int main() {
    // Image img(128, 128, Color(255, 0, 0, 255));
    Image img("test2.png");
    img.set(0, 0, Color(255, 0, 0));
    img.write("test3.png");

    Image lineImg(100, 100, Color(255, 255, 255));

    lineImg.line(40, 90, 10, 10, Color(255, 0, 0));
    lineImg.line(10, 10, 90, 10, Color(0, 255, 0));
    lineImg.line(90, 10, 60, 90, Color(0, 0, 255));

    lineImg.write("line_test.png");

    Image meshImg(4096, 4096, Color(0, 0, 0));

    if (auto reader = tinyobj::ObjReader(); reader.ParseFromFile("african_head.obj")) {
        const auto& attrib = reader.GetAttrib();
        const auto& faces = reader.GetShapes()[0].mesh.num_face_vertices;
        const auto& indices = reader.GetShapes()[0].mesh.indices;

        for (int i = 0, index = 0; i < faces.size(); ++i) {
            const int sum = static_cast<int>(faces[i]);

            for (int j = 0; j < sum; ++j) {
                const float u0 = attrib.vertices[3 * indices[index + j].vertex_index + 0];
                const float v0 = attrib.vertices[3 * indices[index + j].vertex_index + 1];

                const float u1 = attrib.vertices[3 * indices[index + (j + 1) % sum].vertex_index + 0];
                const float v1 = attrib.vertices[3 * indices[index + (j + 1) % sum].vertex_index + 1];

                const int x0 = static_cast<int>((u0 + 1.) * (meshImg.width() - 1.) / 2);
                const int y0 = static_cast<int>((v0 + 1.) * (meshImg.height() - 1.) / 2);

                const int x1 = static_cast<int>((u1 + 1.) * (meshImg.width() - 1.) / 2);
                const int y1 = static_cast<int>((v1 + 1.) * (meshImg.height() - 1.) / 2);
                meshImg.line(x0, y0, x1, y1, Color(255, 255, 255));
            }

            index += sum;
        }
    }

    meshImg.write("mesh_test.png");

    return 0;
}
