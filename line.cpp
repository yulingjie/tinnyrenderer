#include "model.h"
#include <cmath>
#include <iostream>
#include "line.h"

const int width = 800;
const int height = 800;


void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color)
{
    line(t0[0], t0[1], t1[0],t1[1],image, color);
}
void line2(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color)
{
    if(t0.x > t1.x)
    {
        std::swap(t0,t1);
    }
    int dy = t1.y - t0.y;
    int dx = t1.x - t0.x;
    std::cout<<" dy = "<<dy << " dx = "<<dx<<std::endl;
    int derror2 = std::abs(dy) *2;
    int error2 = 0;
    int y = t0.y;
    for(int x = t0.x; x <= t1.x; ++x )
    {
        std::cout<<" x = "<<x << " y = "<<y<<std::endl;
        image.set(x,y,color);
        error2 += derror2;
        if(error2 > dx)
        {
            y+= (t1.y > t0.y ? 1 : -1);
            error2 -= dx*2;
        }

    }
}
void line(int x0, int y0,
        int x1,
        int y1, TGAImage& image,TGAColor color )
{
    bool bsteep = false;
    if(std::abs(x0-x1) < std::abs(y0-y1))
    {
        std::swap(x0,y0);
        std::swap(x1,y1);
        bsteep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0,x1);
        std::swap(y0,y1);
    }
    int dy = y1-y0;
    int dx = x1-x0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    if (bsteep)
    {
        for(int x= x0;x <= x1; ++x)
        {
            image.set(y,x,color);
            error2 += derror2;
            if (error2 > dx)
            {
                y += (y1 > y0 ? 1: -1);
                error2 -= dx*2;
            }
        }
    }
    else
    {
        for(int x= x0;x <= x1; ++x)
        {
            image.set(x,y,color);
            error2 += derror2;
            if (error2 > dx)
            {
                y += (y1 > y0 ? 1: -1);
                error2 -= dx*2;
            }
        }
    }

}


