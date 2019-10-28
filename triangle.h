#include <vector>
#include "geometry.h"
#include "tgaimage.h"

Vec3f barycentric(Vec2i* pts, Vec2i P);
void triangleBary(Vec3f *pts,
        Vec2i *uvs,
        float*zbuffer,
        TGAImage & image,
        TGAColor color,
        TGAImage& uv);
void triangle3(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image,TGAColor color);
void triangle2(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage & image, TGAColor color);
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color);
void triangle3(Vec3f t0, Vec3f t1, Vec3f t2, TGAImage& image, TGAColor color);

void triangleBaryT(Vec3i* pts,
        //Vec2i* uv_coords,
        int *zbuffer,
        TGAImage& image,
        TGAColor color,
        TGAImage& uv);
