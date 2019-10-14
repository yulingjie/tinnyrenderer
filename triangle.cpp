#include <vector>
#include <iostream>
#include "geometry.h"
#include "tgaimage.h"

const int width = 800;
const int height = 800;
void triangle3(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage & image,
        TGAColor color)
{
    if (t0.y==t1.y && t0.y==t2.y) return; // I dont care about degenerate triangles
    // sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!)
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec2i A =               t0 + (t2-t0)*alpha;
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, t0.y+i, color); // attention, due to int casts t0.y+i != A.y
        }
    }

}

void triangle3(Vec3f t0, Vec3f t1, Vec3f t2, TGAImage& image, TGAColor color)
{
    triangle3(Vec2i(t0.x,t0.y),Vec2i(t1.x,t1.y),Vec2i(t2.x,t2.y),image, color);
}

void triangle2(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage & image,
        TGAColor color)
{
    if(t0.y > t1.y) std::swap(t0,t1);
    if(t0.y > t2.y) std::swap(t0,t2);
    if(t1.y > t2.y) std::swap(t1,t2);
    int total_height = t2.y - t0.y;
    for(int y = t0.y; y < t1.y;++y)
    {
        int segment_height = t1.y - t0.y +1;
        float alpha = (float)(y-t0.y)/total_height;
        float beta = (float) (y-t0.y) /segment_height;
        Vec2i A = t0 + (t2-t0)*alpha;
        Vec2i B = t0 + (t1-t0)*beta;
        //image.set(A.x,y,red);
        //image.set(B.x,y,green);
        if (A.x > B.x) std::swap(A,B);
        for(int j = A.x; j < B.x; ++j)
        {
            image.set(j,y,color);
        }
    }
    for(int y = t1.y; y <= t2.y; ++y)
    {
        int segment_height = t2.y - t1.y +1;
        float alpha = (float)(y-t0.y)/total_height;
        float beta = (float)(y-t1.y)/segment_height;
        Vec2i A = t0 + (t2 - t0)*alpha;
        Vec2i B = t1 + (t2 - t1)*beta;
        if(A.x > B.x) std::swap(A,B);
        for(int j= A.x; j <= B.x; ++j)
        {
            image.set(j,y,color);
        }
    }
}
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image,
        TGAColor color)
{
    if(t0.y > t1.y) std::swap(t0,t1);
    if(t0.y > t2.y) std::swap(t0,t2);
    if(t1.y > t2.y) std::swap(t1,t2);
    Vec2i vec[3] = {t0,t1,t2};


    Vec2i& start = vec[0];
    Vec2i* pLeft = NULL;
    Vec2i* pRight = NULL;
    if(vec[1][0] < vec[2][0])
    {
        pLeft = &vec[1];
        pRight= &vec[2];
    }
    else
    {
        pLeft = &vec[2];
        pRight=&vec[1];
    }
    //rasteration
    int dyL = (*pLeft).y- start.y;
    int dxL = (*pLeft).x-start.x;

    int dyR = (*pRight).y- start.y;
    int dxR = (*pRight).x - start.x;

    int derrorL= std::abs(dxL)*2;
    int errorL = 0;

    int derrorR = std::abs(dxR)*2;
    int errorR = 0;

    int xl= start.x;
    int xr = start.x;
    int y = start.y;

    std::cout<<"start = "<<start.x << " "<<start.y<<std::endl;
    std::cout<<"left = "<<(*pLeft).x << " "<<(*pLeft).y<<std::endl;
    for(; y <= (*pLeft).y; ++y)
    {
        float t = (float)(y -start.y)/(float)dyL;
        //std::cout<<"t = " <<t<<std::endl;
        float x = start.x*(1.-t) + (*pLeft).x*t;
        image.set(x,y,color);
        //std::cout<<"x = "<<x << " y = "<<y<<std::endl;
        //image.set(xl,y,color);
        //image.set(xr,y,color);
        //std::cout<<"xl = "<<xl<<" xr = "<<xr<<std::endl;
        //    errorL += derrorL;
        //errorR += derrorR;
        //   if(errorL > dyL)
        //  {
        //     xl += ((*pLeft).x > (start).x ? 1 : -1);
        //    errorL -= dyL *2;
        // }
        //if(errorR > dyR){
        //xr += ((*pRight)[0] > (start)[0] ? 1 : -1);
        //errorR -= dyR *2;
        //}
        //for(int i = xl; i < xr; ++i)
        //{
        //image.set(i,y,color);
        //}
        //        line(xl,y,xr,y,image,color);
    }

    /*
       int gapL = 0;
       int gapR = 0;
       if((*pRight)[1] < (*pLeft)[1])
       {
       dyR = vec[2][1] - (*pRight)[1];
       dxR = vec[2][0] - (*pRight)[0];
       derrorR =  std::abs(dxR)*2;
       errorR = 0;
       gapR = vec[2][0] > (*pRight)[0]  ? 1: -1;
       gapL = vec[2][0] > vec[0][0] ? 1 : -1;
       }
       else{
       dyL = vec[2][1] - (*pLeft)[1];
       dxL = vec[2][0] - (*pLeft)[0];
       derrorL = std::abs(dxL)*2;
       errorL = 0;
       gapR =vec[2][0] > vec[0][0]? 1:-1;
       gapL = vec[2][0] > (*pLeft)[0] ? 1 : -1;
       }
       for(;y<vec[2][1]; ++y)
       {
       image.set(xl,y,color);
       image.set(xr,y,color);
       errorL += derrorL;
       errorR += derrorR;
       if(errorL > dyL)
       {
    //xl += (vec[2][0] > (*pLeft)[0] ?1 : -1);
    xl+=gapL;
    errorL -= dyL *2;
    }
    if(errorR > dyR){
//xr += (vec[2][0] > (*pRight)[0] ? 1 : -1);
xr += gapR;
errorR -= dyR *2;
}
//   line(xl,y,xr,y,image,color);
for(int i =xl; i< xr; ++i )
{
image.set(i,y,color);
}

}

*/
}
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    Vec3f u = cross(s[0], s[1]);
    if (std::abs(u[2])>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}
Vec3f barycentric(Vec2i *pts, Vec2i P){
    Vec3f u = cross(Vec3f(pts[2][0] - pts[0][0],
                pts[1][0]-pts[0][0],
                pts[0][0] - P[0]),
            Vec3f(pts[2][1] - pts[0][1],
                pts[1][1]-pts[0][1],
                pts[0][1]- P[1])
            );
    if (std::abs(u[2]) < 1) return Vec3f(-1,1,1);
    return Vec3f(1.f-(u.x + u.y)/ u.z, u.y / u.z,u.x/u.z);
}
void triangleBary(Vec3f* pts,
        Vec2f *uv_coords,
        float * zbuffer,
        TGAImage & image, TGAColor color,
        TGAImage& diffuse)
{

    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width()-1, image.get_height()-1);
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
    Vec3f P;
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            Vec3f bc_screen  = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            P.z = 0;
            for (int i=0; i<3; i++) P.z += pts[i][2]*bc_screen[i];
            Vec2f uv;
            uv.x= bc_screen.x * uv_coords[0].x
                + bc_screen.y * uv_coords[1].x
                + bc_screen.z * uv_coords[2].x;
            uv.y = bc_screen.x * uv_coords[0].y
            + bc_screen.y * uv_coords[1].y
            + bc_screen.z * uv_coords[2].y;
            float w = diffuse.get_width() * uv.x;
            float h = diffuse.get_height() * uv.y;
            TGAColor c = diffuse.get((int)w,(int)h);
            if (zbuffer[int(P.x+P.y*width)]<P.z) {
                zbuffer[int(P.x+P.y*width)] = P.z;
                image.set(P.x, P.y, c);
            }
        }
    }
}
