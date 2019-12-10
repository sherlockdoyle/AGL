#ifndef BASIC_SHAPE_H
#define BASIC_SHAPE_H

#include "entity.h"

namespace agl {
Entity tetrahedron();
Entity cube(bool calcNorm=true);
Entity cuboid(float x=2, float y=2, float z=2);
Entity octahedron();
Entity dodecahedron();
Entity icosahedron();
Entity sphere(int lat=20, int lng=20);
Entity icosphere(int subdivision=3);
Entity plane(float x=2, float z=2, bool calcNorm=true);
Entity tessellatedPlane(int l=10, int h=10);

void normalizeVertices(Entity &e, float t=1);
void subdivideVertices(Entity &e);
std::vector<GLuint> triangulatePolygon(const std::vector<GLuint> &polygon);
}

#endif // BASIC_SHAPE_H
