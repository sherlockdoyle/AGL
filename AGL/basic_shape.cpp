#include "basic_shape.h"
#include<queue>

namespace agl {
Entity tetrahedron()
{
    int verts[] = { 1, 1, 1,
                    1,-1,-1,
                   -1, 1,-1,
                   -1,-1, 1},
        idx[] = {0, 1, 2,
                 0, 2, 3,
                 0, 3, 1,
                 1, 3, 2};
    Entity e;
    e.vertices.assign(verts, verts + 12);
    e.indices.assign(idx, idx + 12);
    return e;
}
Entity cube(bool calcNorm)
{
    Entity e;
    if(calcNorm)
    {
        int verts[] = {-1,-1,-1,    -1,-1,-1,    -1,-1,-1,
                       -1,-1, 1,    -1,-1, 1,    -1,-1, 1,
                       -1, 1,-1,    -1, 1,-1,    -1, 1,-1,
                       -1, 1, 1,    -1, 1, 1,    -1, 1, 1,
                        1,-1,-1,     1,-1,-1,     1,-1,-1,
                        1,-1, 1,     1,-1, 1,     1,-1, 1,
                        1, 1,-1,     1, 1,-1,     1, 1,-1,
                        1, 1, 1,     1, 1, 1,     1, 1, 1},
            norm[] = {-1, 0, 0,     0,-1, 0,     0, 0,-1,
                      -1, 0, 0,     0,-1, 0,     0, 0, 1,
                      -1, 0, 0,     0, 1, 0,     0, 0,-1,
                      -1, 0, 0,     0, 1, 0,     0, 0, 1,
                       1, 0, 0,     0,-1, 0,     0, 0,-1,
                       1, 0, 0,     0,-1, 0,     0, 0, 1,
                       1, 0, 0,     0, 1, 0,     0, 0,-1,
                       1, 0, 0,     0, 1, 0,     0, 0, 1},
            idx[] = { 0, 3, 6,     3, 9, 6,  // left
                     12,18,15,    15,18,21,  // right
                      1,13, 4,     4,13,16,  // bottom
                     10,22,19,     7,10,19,  // top
                      2, 8,14,     8,20,14,  // back
                      5,17,11,    17,23,11}; // front
        e.vertices.assign(verts, verts + 72);
        e.normals.assign(norm, norm + 72);
        e.indices.assign(idx, idx + 36);
    }
    else
    {
        int verts[] = {-1,-1,-1,    -1,-1, 1,    -1, 1,-1,    -1, 1, 1,
                        1,-1,-1,     1,-1, 1,     1, 1,-1,     1, 1, 1},
            idx[] = {0, 1, 2,     1, 3, 2,  // left
                     4, 6, 5,     5, 6, 7,  // right
                     0, 4, 1,     1, 4, 5,  // bottom
                     3, 7, 6,     2, 3, 6,  // top
                     0, 2, 4,     2, 6, 4,  // back
                     1, 5, 3,     5, 7, 3}; // front
        e.vertices.assign(verts, verts + 24);
        e.indices.assign(idx, idx + 36);
    }
    return e;
}
Entity cuboid(float x, float y, float z)
{
    x /= 2; y /= 2; z /= 2;
    Entity e = cube();
    float verts[] = {-x,-y,-z,    -x,-y, z,    -x, y,-z,    -x, y, z,
                      x,-y,-z,     x,-y, z,     x, y,-z,     x, y, z};
    e.vertices.assign(verts, verts + 24);
    return e;
}
Entity octahedron()
{
    int verts[] = {-1, 0, 0,     1, 0, 0,
                    0,-1, 0,     0, 1, 0,
                    0, 0,-1,     0, 0, 1},
        idx[] = {0, 4, 2,     1, 2, 4,     2, 5, 0,     1, 5, 2,
                 0, 3, 4,     1, 4, 3,     0, 5, 3,     1, 3, 5};
    Entity e;
    e.vertices.assign(verts, verts + 18);
    e.indices.assign(idx, idx + 24);
    return e;
}
Entity dodecahedron()
{
#define P 1.618033988749894848204586834365638117720309179805762862135
#define Q 0.618033988749894848204586834365638117720309179805762862135
    Entity e;
    float verts[] = { 1, 1, 1,     1, 1,-1,     1,-1, 1,     1,-1,-1,
                     -1, 1, 1,    -1, 1,-1,    -1,-1, 1,    -1,-1,-1,
                      0, Q, P,     0, Q,-P,     0,-Q, P,     0,-Q,-P,
                      Q, P, 0,     Q,-P, 0,    -Q, P, 0,    -Q,-P, 0,
                      P, 0, Q,     P, 0,-Q,    -P, 0, Q,    -P, 0,-Q};
    e.vertices.assign(verts, verts + 60);
    std::vector<GLuint> v = triangulatePolygon(std::vector<GLuint>{ 0,16, 2,10, 8});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{ 0, 8, 4,14,12});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{16,17, 1,12, 0});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{ 1, 9,11, 3,17});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{ 1,12,14, 5, 9});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{ 2,13,15, 6,10});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{13, 3,17,16, 2});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{ 3,11, 7,15,13});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{ 4, 8,10, 6,18});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{14, 5,19,18, 4});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{ 5,19, 7,11, 9});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    v = triangulatePolygon(std::vector<GLuint>{15, 7,19,18, 6});
    e.indices.insert(e.indices.end(), v.begin(), v.end());
    return e;
}
Entity icosahedron()
{
    Entity e;
    float verts[] = { 0, P, 1,     0, P,-1,     0,-P, 1,     0,-P,-1,
                      1, 0, P,    -1, 0, P,     1, 0,-P,    -1, 0,-P,
                      P, 1, 0,     P,-1, 0,    -P, 1, 0,    -P,-1, 0};
    int idx[] = {0, 1, 8,     0, 4, 5,     0, 5,10,     0, 8, 4,     0,10, 1,
                 1, 6, 8,     1, 7, 6,     1,10, 7,     2, 3,11,     2, 4, 9,
                 2, 5, 4,     2, 9, 3,     2,11, 5,     3, 6, 7,     3, 7,11,
                 3, 9, 6,     4, 8, 9,     5,11,10,     6, 9, 8,     7,10,11};
    e.vertices.assign(verts, verts + 36);
    e.indices.assign(idx, idx + 60);
    return e;
}
Entity sphere(int lat, int lng)
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
Entity icosphere(int subdivision)
{
    Entity e = icosahedron();
    for(int i=0; i<subdivision; ++i)
        subdivideVertices(e);
    normalizeVertices(e);
    return e;
}
Entity plane(float x, float z, bool calcNorm)
{
    x /= 2; z /= 2;
    Entity e;
    float verts[] = {-x, 0,-z,    -x, 0, z,     x, 0,-z,     x, 0, z};
    int idx[] = {0, 1, 2,     1, 3, 2};
    e.vertices.assign(verts, verts + 12);
    e.indices.assign(idx, idx + 6);
    if(calcNorm)
    {
        int norm[] = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};
        e.normals.assign(norm, norm + 12);
    }
    return e;
}
Entity tessellatedPlane(int l, int h)
{
    float x = l / 2., z = h / 2.;
    Entity e;
    for(float i=-x; i<=x; ++i)
        for(float j=-z; j<=z; ++j)
        {
            e.vertices.push_back(i);
            e.vertices.push_back(0);
            e.vertices.push_back(j);
        }
    for(int i=0; i<l; ++i)
        for(int j=0; j<h; ++j)
        {
            e.indices.push_back( i    * (h+1) + j  );
            e.indices.push_back( i    * (h+1) + j+1);
            e.indices.push_back((i+1) * (h+1) + j  );
            e.indices.push_back( i    * (h+1) + j+1);
            e.indices.push_back((i+1) * (h+1) + j+1);
            e.indices.push_back((i+1) * (h+1) + j  );
        }
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
std::vector<GLuint> triangulatePolygon(const std::vector<GLuint> &polygon)
{
    std::queue<GLuint, std::deque<GLuint>> q(std::deque<GLuint>(polygon.begin(), polygon.end()));
    std::vector<GLuint> indices;
    while(q.size() > 2)
    {
        indices.push_back(q.front());
        q.push(q.front());
        q.pop();
        indices.push_back(q.front());
        q.pop();
        indices.push_back(q.front());
    }
    return indices;
}
}
