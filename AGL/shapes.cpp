#include "shapes.h"
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
    int verts[] = {-1,-1,-1,    -1,-1, 1,    -1, 1,-1,    -1, 1, 1,
                    1,-1,-1,     1,-1, 1,     1, 1,-1,     1, 1, 1},
        idx[] = {0, 1, 2,     1, 3, 2,  // left
                 4, 6, 5,     5, 6, 7,  // right
                 0, 4, 1,     1, 4, 5,  // bottom
                 3, 7, 6,     2, 3, 6,  // top
                 0, 2, 4,     2, 6, 4,  // back
                 1, 5, 3,     5, 7, 3}; // front
    if(calcNorm)
    {
        for(int i=0; i<8; ++i)
            for(int j=0; j<3; ++j)
            {
                e.vertices.push_back(verts[i*3  ]);
                e.vertices.push_back(verts[i*3+1]);
                e.vertices.push_back(verts[i*3+2]);
                for(int k=0; k<3; ++k)
                    e.normals.push_back(j==k ? verts[i*3+k] : 0);
            }
        for(int i=0; i<3; ++i)
            for(int j=0; j<12; ++j)
                e.indices.push_back(idx[i*12+j]*3+i);
    }
    else
    {
        e.vertices.assign(verts, verts + 24);
        e.indices.assign(idx, idx + 36);
    }
    return e;
}
Entity cuboid(float x, float y, float z, bool calcNorm)
{
    x /= 2; y /= 2; z /= 2;
    Entity e = cube(calcNorm);
    for(int i=0; i<72; i+=3)
    {
        e.vertices[i  ] *= x;
        e.vertices[i+1] *= y;
        e.vertices[i+2] *= z;
    }
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
Entity sphere(int lat, int lng, bool calcNorm)
{
    Entity e;
    float xy, x, y, z,
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
            x = xy * std::cos(latAngle);
            y = xy * std::sin(latAngle);
            e.vertices.push_back(x);
            e.vertices.push_back(y);
            e.vertices.push_back(z);
            if(calcNorm)
            {
                e.normals.push_back(x);
                e.normals.push_back(y);
                e.normals.push_back(z);
            }
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
Entity icosphere(int subdivision, bool calcNorm)
{
    Entity e = icosahedron();
    for(int i=0; i<subdivision; ++i)
        subdivideVertices(e);
    normalizeVertices(e);
    if(calcNorm)
        e.normals.assign(e.vertices.begin(), e.vertices.end());
    return e;
}
Entity cylinder(float r, float h, int strips)  // TODO: Add normals
{
    Entity e;
    float step = 2 * AGL_PI / strips, ang = 0, x, y;
    h /= 2;
    std::vector<GLuint> upper, lower;
    for(int i=0; i<strips; ++i, ang += step)
    {
        x = std::cos(ang);
        y = std::sin(ang);
        e.vertices.push_back(x*r);
        e.vertices.push_back(-h);
        e.vertices.push_back(y*r);
        e.vertices.push_back(x*r);
        e.vertices.push_back(h);
        e.vertices.push_back(y*r);
        if(i > 0)
        {
            e.indices.push_back(2*(i-1)  );
            e.indices.push_back(2* i   +1);
            e.indices.push_back(2*(i-1)+1);
            e.indices.push_back(2*(i-1)  );
            e.indices.push_back(2* i     );
            e.indices.push_back(2* i   +1);
        }
        lower.push_back(2*i  );
        upper.push_back(2*i+1);
    }
    e.indices.push_back(2*(strips-1)  );
    e.indices.push_back(             1);
    e.indices.push_back(2*(strips-1)+1);
    e.indices.push_back(2*(strips-1)  );
    e.indices.push_back(             0);
    e.indices.push_back(             1);
    lower = triangulatePolygon(lower);
    e.indices.insert(e.indices.end(), lower.begin(), lower.end());
    upper = triangulatePolygon(upper);
    e.indices.insert(e.indices.end(), upper.begin(), upper.end());
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
void calcNormals(Entity &e, bool perFace)
{
    int l = e.vertices.size();
    float ax = 0, ay = 0, az = 0, x, y, z, invSum;
    e.normals.clear();
    for(int i=0; i<l; i+=3)
    {
        ax += e.vertices[i  ];
        ay += e.vertices[i+1];
        az += e.vertices[i+2];
    }
    ax /= l; ay /= l; az /= l;
    for(int i=0; i<l; i+=3)
    {
        x = e.vertices[i  ] - ax;
        y = e.vertices[i+1] - ay;
        z = e.vertices[i+2] - az;
        invSum = 1 / std::sqrt(x * x + y * y + z * z);
        e.normals.push_back(x * invSum);
        e.normals.push_back(y * invSum);
        e.normals.push_back(z * invSum);
    }
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
