#include "basic_shape.h"

namespace agl {
Entity unitCube()
{
    int verts[] = {-1, -1, -1,
                   -1, -1,  1,
                   -1,  1, -1,
                   -1,  1,  1,
                    1, -1, -1,
                    1, -1,  1,
                    1,  1, -1,
                    1,  1,  1},
        idx[] = {2, 0, 1, 2, 1, 3,  // left
                 4, 5, 7, 4, 7, 6,  // right
                 0, 4, 5, 0, 5, 1,  // bottom
                 2, 7, 6, 2, 3, 7,  // top
                 0, 6, 4, 0, 2, 6,  // back
                 1, 5, 7, 1, 7, 3}; // front
    Entity e;
    e.vertices.assign(verts, verts + 24);
    e.indices.assign(idx, idx + 36);
    return e;
}
Entity cuboid(float l, float b, float h)
{
    l /= 2; b /= 2; h /= 2;
    Entity e = unitCube();
    float verts[] = {-l, -b, -h,
                     -l, -b,  h,
                     -l,  b, -h,
                     -l,  b,  h,
                      l, -b, -h,
                      l, -b,  h,
                      l,  b, -h,
                      l,  b,  h};
    e.vertices.assign(verts, verts + 24);
    return e;
}
Entity unitSphere(int lat, int lng)
{
    Entity e;
    float xy, z,
          latStep = 2 * AGL_PI / lat,
          lngInv = 1. / lng,
          latAngle, lngAngle;
    for(int i=0; i<=lng; ++i)
    {
        lngAngle = AGL_PI * (0.5 - i * lngInv);
        xy = std::cos(lngAngle);
        z = std::sin(lngAngle);
        for(int j=0; j<=lat; ++j)
        {
            latAngle = j * latStep;
            e.vertices.push_back(xy * std::cos(latAngle));
            e.vertices.push_back(xy * std::sin(latAngle));
            e.vertices.push_back(z);
        }
    }
    int k1, k2;
    for(int i=0; i<lng; ++i)
    {
        k1 = i * (lat + 1);
        k2 = k1 + lat + 1;
        for(int j=0; j<lat; ++j, ++k1, ++k2)
        {
            if(i != 0)
            {
                e.indices.push_back(k1);
                e.indices.push_back(k2);
                e.indices.push_back(k1 + 1);
            }
            if(i != lng - 1)
            {
                e.indices.push_back(k1 + 1);
                e.indices.push_back(k2);
                e.indices.push_back(k2 + 1);
            }
        }
    }
    return e;
}
}
