#ifndef LOADER_H
#define LOADER_H

#include "entity.h"

namespace agl {
Entity loadObj(const char *path, bool includeNormals=false, bool includeUVs=false);
}

#endif // LOADER_H
