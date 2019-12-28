#include "loader.h"
#include<fstream>

namespace agl {
Entity loadObj(const char *path, bool includeNormals, bool includeUVs)
{
    Entity e;
    std::ifstream fs(path);
    if(fs.is_open())
    {
        std::string line;
        while(std::getline(fs, line))
            std::cout << line << std::endl;
    }
    return e;
}
}
