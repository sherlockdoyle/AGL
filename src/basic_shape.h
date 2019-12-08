#ifndef BASIC_SHAPE_H
#define BASIC_SHAPE_H

#include<entity.h>

namespace agl {
Entity unitCube();
Entity cuboid(float l=2, float b=2, float h=2);
Entity plane(float l=2, float b=2);
Entity unitIcosahedron();
Entity unitSphere(int lat=20, int lng=20);
Entity unitIcosphere(int subdivision=3);

void normalizeVertices(Entity &e, float t=1);
void subdivideVertices(Entity &e);
}

#endif // BASIC_SHAPE_H
