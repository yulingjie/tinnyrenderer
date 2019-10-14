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
/*
    int ybuffer[width];
    for(int i = 0; i < width ; ++i)
    {
        ybuffer[i] = std::numeric_limits<int>::min();
    }
    rasterize(Vec2i(20,34),Vec2i(744,400),render, red,ybuffer);
    rasterize(Vec2i(120,434),Vec2i(444,600),render, green,ybuffer);
    rasterize(Vec2i(330,463),Vec2i(594,200),render,blue,ybuffer);
    render.flip_vertically();
    render.write_tga_file("render.tga");
    */
    //TGAImage image(width,height,TGAImage::RGB);

    //Vec2i t0[3]={Vec2i(10,70), Vec2i(50,160), Vec2i(70,80)};
    //Vec2i t1[3] = {Vec2i(180,50), Vec2i(150,1),Vec2i(70,180)};
    //Vec2i t2[3] = {Vec2i(180,150),Vec2i(120,160),Vec2i(130,180)};

    //line(t0[0],t0[1],image, green);
    //    line2(t0[0],t0[2],image,green);
    //triangle(t0[0],t0[1],t0[2],image, red);
    //triangle(t1[0],t1[1],t1[2],image, white);
    //triangle(t2[0],t2[1],t2[2],image, green);
    /*for(int i =0; i < 1000000;++i)
      {
      line(13, 20, 80, 40, image, white);
      line(20, 13, 40, 80, image, red);
      line(80, 40, 13, 20, image, red);

      }
      */

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
void DrawGeomWithLight(int argc, char** argv, TGAImage& image)
{
    /*
    TGAImage frame(width,height,TGAImage::RGB);
    Model * model = NULL;
    model = new Model("obj/african_head/african_head.obj");
    Vec3f light_dir(0,0,-1.0);
    for(int i =0; i < model->nfaces(); ++i)
    {
        std::vector<int> face = model->face(i);
        Vec3f screen_coord[3];
        Vec3f world_coords[3];
        for(int j =0; j < 3; ++j)
        {
            Vec3f v= model->vert(face[j]);
            screen_coord[j] = Vec3f((v.x +1.)*width/2.,
                    (v.y+1.)*height/2.,v.z);
            world_coords[j] = v;
        }
           triangle2(screen_coord[0],
           screen_coord[1],
           screen_coord[2],
           frame,
           TGAColor(rand() %255,rand() %255, rand() %255, 255) );
        Vec3f n =cross((world_coords[2]-world_coords[0]),
                (world_coords[1]-world_coords[0]));
        n.normalize();
        float intensity = n*light_dir;
        if(intensity > 0)
        {
            triangle3(screen_coord[0], screen_coord[1],
                    screen_coord[2],frame,
                    TGAColor(intensity*255,
                        intensity*255,intensity*255,255)
                    );
        }
        //n.normalize();
    }
    //Vec2i pts[3] = {Vec2i(10,10), Vec2i(100,30),Vec2i(190,160)};
    //triangleBary(pts, frame, TGAColor(255,0,0));
    frame.flip_vertically();
    frame.write_tga_file("output.tga");
*/
}
Vec3f world2screen(Vec3f v)
{
    return Vec3f(int((v.x +1.) * width /2. + .5),
            int((v.y +1.)*height /2. + .5),v.z);
}
void DrawGeomWithZBuff(int argc, char** argv )
{
    /*
       Model * model;
       if (2==argc) {
       model = new Model(argv[1]);
       } else {
       model = new Model("obj/african_head/african_head.obj");
       }

       float *zbuffer = new float[width*height];
       for(int i =width * height; i >=0 ; --i)
       {
       zbuffer[i] = -std::numeric_limits<float>::max();
       }

       TGAImage image(width, height, TGAImage::RGB);
       for (int i=0; i<model->nfaces(); i++) {
       std::vector<int> face = model->face(i);
       Vec3f pts[3];
       for (int i=0; i<3; i++) pts[i] = world2screen(model->vert(face[i]));
       TGAColor tc(rand()%255, rand()%255, rand()%255, 255);
       triangleBary(pts, zbuffer, image,tc);
    //triangle3(pts[0],pts[1],pts[2],image,tc);
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    */
    TGAImage diffuse;

    diffuse.read_tga_file("obj/african_head/african_head_diffuse.tga");
    diffuse.flip_vertically();
    std::cout<<"width = "<<diffuse.get_width()<<" height = "<<diffuse.get_height()<<std::endl;
    TGAImage frame(width,height,TGAImage::RGB);
    Model * model = NULL;
    model = new Model("obj/african_head/african_head.obj");
    Vec3f light_dir(0,0,-1.0);
    Matrix Projection = Matrix::identity(4);
    Matrix ViewPort = viewport(width/ 8, height/ 8, width * 3/4, height * 3/4);
    Projection[3][2] = -1.f/camera.z;
    float *zbuffer = new float[width*height];
    for(int i = width * height-1; i >=0 ; i--)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }
    for(int i =0; i < model->nfaces(); ++i)
    {
        std::vector<int> face = model->face(i);
        Vec3f screen_coord[3];
        Vec3f world_coords[3];
        Vec2f uv_coords[3];
        for(int j =0; j < 3; ++j)
        {

            Vec2f uv= model->uv(i,j);
            uv_coords[j] = uv;
            Vec3f v= model->vert(face[j]);
            screen_coord[j] =m2v(ViewPort * Projection*v2m(v));
            world_coords[j] = v;
        }

        Vec3f n =cross((world_coords[2]-world_coords[0]),
                (world_coords[1]-world_coords[0]));
        n.normalize();
        float intensity = n*light_dir;
        if(intensity > 0)
        {
            triangleBary(screen_coord,
                    uv_coords,
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

