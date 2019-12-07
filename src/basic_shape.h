#ifndef BASIC_SHAPE_H
#define BASIC_SHAPE_H

#include<entity.h>

namespace agl {
Entity unitCube();
Entity cuboid(float l=2, float b=2, float h=2);
Entity unitSphere(int lat=20, int lng=20);
}

#endif // BASIC_SHAPE_H
