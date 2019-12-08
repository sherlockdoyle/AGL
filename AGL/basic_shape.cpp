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
        idx[] = {0, 1, 2, 1, 3, 2,  // left
                 4, 6, 5, 5, 6, 7,  // right
                 0, 4, 1, 1, 4, 5,  // bottom
                 3, 7, 6, 2, 3, 6,  // top
                 0, 2, 4, 2, 6, 4,  // back
                 1, 5, 3, 5, 7, 3}; // front
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
Entity plane(float l, float b)
{

}
Entity unitIcosahedron()
{
    Entity e;
#define A .850650808352039932
#define B .525731112119133606
    float verts[] = { 0, A, B,
                      0, A,-B,
                      0,-A, B,
                      0,-A,-B,
                      B, 0, A,
                     -B, 0, A,
                      B, 0,-A,
                     -B, 0,-A,
                      A, B, 0,
                      A,-B, 0,
                     -A, B, 0,
                     -A,-B, 0};
    int idx[] = {0, 1, 8,
                 0, 4, 5,
                 0, 5,10,
                 0, 8, 4,
                 0,10, 1,
                 1, 6, 8,
                 1, 7, 6,
                 1,10, 7,
                 2, 3,11,
                 2, 4, 9,
                 2, 5, 4,
                 2, 9, 3,
                 2,11, 5,
                 3, 6, 7,
                 3, 7,11,
                 3, 9, 6,
                 4, 8, 9,
                 5,11,10,
                 6, 9, 8,
                 7,10,11};
    e.vertices.assign(verts, verts + 36);
    e.indices.assign(idx, idx + 60);
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
Entity unitIcosphere(int subdivision)
{
    Entity e = unitIcosahedron();
    for(int i=0; i<subdivision; ++i)
        subdivideVertices(e);
    normalizeVertices(e);
    return e;
}

void normalizeVertices(Entity &e, float t)
{
    float x, y, z, invSum;
    for(int i=0, l=e.vertices.size()/3; i<l; ++i)
    {
        x = e.vertices[i*3  ];
        y = e.vertices[i*3+1];
        z = e.vertices[i*3+2];
        invSum = t / std::sqrt(x * x + y * y + z * z);
        e.vertices[i*3  ] = x * invSum + (1 - t) * e.vertices[i*3  ];
        e.vertices[i*3+1] = y * invSum + (1 - t) * e.vertices[i*3+1];
        e.vertices[i*3+2] = z * invSum + (1 - t) * e.vertices[i*3+2];
    }
}
void subdivideVertices(Entity &e)
{
    int nv = e.vertices.size() / 3, a, b, c;
    std::vector<int> edges(nv*nv, 0);  // TODO: Improve hashing
    for(int i=0, l=e.indices.size()/3; i<l; ++i)
    {
        a = e.indices[i*3  ];
        b = e.indices[i*3+1];
        c = e.indices[i*3+2];
        edges[a*nv+b] = edges[b*nv+c] = edges[c*nv+a] = 1;
    }
    c = nv;
    for(int i=0; i<nv*nv; ++i)
        if(edges[i])
        {
            a = i / nv;
            b = i % nv;
            e.vertices.push_back((e.vertices[a*3  ] + e.vertices[b*3  ]) / 2);
            e.vertices.push_back((e.vertices[a*3+1] + e.vertices[b*3+1]) / 2);
            e.vertices.push_back((e.vertices[a*3+2] + e.vertices[b*3+2]) / 2);
            edges[i] = c++;
        }
    std::vector<GLuint> newIndices;
    for(int i=0, l=e.indices.size()/3; i<l; ++i)
    {
        a = e.indices[i*3  ];
        b = e.indices[i*3+1];
        c = e.indices[i*3+2];
        newIndices.push_back(a);
        newIndices.push_back(edges[a*nv+b]);
        newIndices.push_back(edges[c*nv+a]);
        newIndices.push_back(b);
        newIndices.push_back(edges[b*nv+c]);
        newIndices.push_back(edges[a*nv+b]);
        newIndices.push_back(c);
        newIndices.push_back(edges[c*nv+a]);
        newIndices.push_back(edges[b*nv+c]);
        newIndices.push_back(edges[a*nv+b]);
        newIndices.push_back(edges[b*nv+c]);
        newIndices.push_back(edges[c*nv+a]);
    }
    e.indices.swap(newIndices);
}
}
