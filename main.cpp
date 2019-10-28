#include "line.h"
#include <vector>
#include"model.h"
#include "geometry.h"
#include "triangle.h"
#include "tgaimage.h"
#include<iostream>
#include<cmath>
const int width = 800;
const int height = 800;
const int depth = 255;

const TGAColor white = TGAColor(255,255,255,255);
const TGAColor red = TGAColor(255,0,0,255);
const TGAColor green = TGAColor(0,255,0,255);
const TGAColor blue = TGAColor(0,0,255,255);

Vec3f camera(0,0,3);
Vec3f light_dir(0,0,-1);
Model * model = NULL;
int *zbuffer = NULL;

void rasterize(Vec2i p0,Vec2i p1, TGAImage& image, TGAColor color,int ybuffer[]);

void DrawGeom(int argc, char** argv, TGAImage& image);
void DrawScene()
{
    TGAImage scene(width, height,TGAImage::RGB);

    line(Vec2i(20,34),Vec2i(744,400),scene,red);
    line(Vec2i(120,434),Vec2i(444,400),scene,green);
    line(Vec2i(330,463),Vec2i(594,200),scene,blue);

    line(Vec2i(10,10),Vec2i(790,10),scene, white);

    scene.flip_vertically();
    scene.write_tga_file("scene.tga");


}
Vec3f m2v(Matrix m)
{
    return Vec3f(m[0][0]/ m[3][0], m[1][0]/m[3][0],m[2][0]/m[3][0]);
}

Matrix v2m(Vec3f v)
{
    Matrix m(4,1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.0f;
    return m;
}
Matrix viewport(int x, int y, int w, int h)
{
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w/2.f;
    m[1][3] = y + h/2.f;
    m[2][3] = depth /2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth /2.f;
    return m;
}
void DrawGeomWithZBuff(int argc, char** argv);
int main(int argc, char** argv)
{


    DrawGeomWithZBuff(argc, argv);
    return 0;
}
void rasterize(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color,int ybuffer[])
{
    if(p0.x > p1.x)
    {
        std::swap(p0,p1);
    }
    for(int x= p0.x;x <= p1.x; ++x)
    {
        float t = (x- p0.x)/(float)(p1.x-p0.x);
        int y = p0.y *(1.-t) + p1.y *t;
        if(ybuffer[x] < y)
        {
            ybuffer[x] = y;
            image.set(x,0,color);
        }
    }
}

Vec3f world2screen(Vec3f v)
{
    return Vec3f(int((v.x +1.) * width /2. + .5),
            int((v.y +1.)*height /2. + .5),v.z);
}
void triangleT(Vec3i t0, Vec3i t1, Vec3i t2,
     //   Vec2i uv0, Vec2i uv1, Vec2i uv2,
        TGAImage &image, float intensity, int *zbuffer) {
    if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
    if (t0.y>t1.y) { std::swap(t0, t1); //std::swap(uv0, uv1);
    }
    if (t0.y>t2.y) { std::swap(t0, t2); //std::swap(uv0, uv2);
    }
    if (t1.y>t2.y) { std::swap(t1, t2);// std::swap(uv1, uv2);
    }

    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec3i A   =               t0  + Vec3f(t2-t0  )*alpha;
        Vec3i B   = second_half ? t1  + Vec3f(t2-t1  )*beta : t0  + Vec3f(t1-t0  )*beta;
      //  Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
       // Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;
       // if (A.x>B.x) { std::swap(A, B); std::swap(uvA, uvB); }
        for (int j=A.x; j<=B.x; j++) {
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            Vec3i   P = Vec3f(A) + Vec3f(B-A)*phi;
        //    Vec2i uvP =     uvA +   (uvB-uvA)*phi;
            int idx = P.x+P.y*width;
            if (zbuffer[idx]<P.z) {
                zbuffer[idx] = P.z;
         //       TGAColor color = model->diffuse(uvP);
                TGAColor color = white;
                image.set(P.x, P.y, TGAColor(color.r*intensity, color.g*intensity, color.b*intensity));
            }
        }
    }
}

void DrawGeomWithZBuff(int argc, char** argv )
{
   TGAImage diffuse;

    diffuse.read_tga_file("obj/african_head/african_head_diffuse.tga");
    diffuse.flip_vertically();
    std::cout<<"width = "<<diffuse.get_width()<<" height = "<<diffuse.get_height()<<std::endl;
    TGAImage frame(width,height,TGAImage::RGB);
    model = new Model("obj/african_head/african_head.obj");
    Vec3f light_dir(0,0,-1.0);
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort = viewport(width/ 8, height/ 8, width * 3/4, height * 3/4);
    Projection[3][2] = -1.f/camera.z;
    //float* fzbuffer = new float[width* height];
    zbuffer = new int[width*height];
    for(int i = width * height-1; i >=0 ; i--)
    {
       zbuffer[i] = -std::numeric_limits<int>::max();
    }
    /*
    Vec3i screen_coord[3];
    Vec3f world_coords[3];
    Vec2i uv_coords[3];
    Vec3f vertices[3] = {
        {-0.000114,-0.76357,0.23426},
        {0.000284,-0.81913,0.28684},
        {0.11738,-0.79278,0.30677}
    };
    for(int j =0; j < 3; ++j)
    {
        Vec3f v = vertices[j];
        screen_coord[j] = m2v(ViewPort* Projection * v2m(v));
        world_coords[j] =v;
    }
    //triangleT(screen_coord[0],screen_coord[1],screen_coord[2],
          //frame, 0.5f,zbuffer );
    triangleBaryT(screen_coord,
     //       uv_coords,
            zbuffer,
            frame,
            TGAColor(0.5f*255,
                0.5f*255,
                0.5f*255,
                255)
            ,
            diffuse
            );

*/

       for(int i =0; i < model->nfaces(); ++i)
       {
       std::vector<int> face = model->face(i);
       Vec3i screen_coord[3];
       Vec3f world_coords[3];
       Vec2i uv_coords[3];
       for(int j =0; j < 3; ++j)
       {

       Vec2i uv= model->uv(i,j);
       uv_coords[j] = uv;
       Vec3f v= model->vert(face[j]);
       screen_coord[j] =m2v(ViewPort * Projection*v2m(v));
       world_coords[j] = v;
       }

       Vec3f n =(world_coords[2]-world_coords[0])^
       (world_coords[1]-world_coords[0]);
       n.normalize();
       float intensity = n*light_dir;
 //      if(intensity > 0)
       {

       triangleBaryT(screen_coord,
//       uv_coords,
       zbuffer,
       frame,
       TGAColor(intensity*255,
       intensity*255,
       intensity*255,
       255)
       ,
       diffuse
       );


       }

       }

    frame.flip_vertically();
    frame.write_tga_file("output.tga");

}

void DrawGeom(int argc, char** argv, TGAImage& image)
{
    Model* model=NULL;
    if(2== argc)
    {
        model = new Model(argv[1]);
    }
    else
    {

        model = new Model("obj/african_head/african_head.obj");
    }


    for(int i =0; i < model->nfaces();++i)
    {
        std::vector<int> face =  model->face(i);
        for(int j =0; j< 3;++j)
        {
            Vec3f v0= model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);

            int x0 =(v0.x + 1.)*width/2.0;
            int y0=(v0.y +1.)*height/2.0;
            int x1=(v1.x + 1.)*width/2.0;
            int y1=(v1.y+1.)*height/2.0;
            line(x0,y0,x1,y1,image, white);
        }
    }
    delete model;
}

