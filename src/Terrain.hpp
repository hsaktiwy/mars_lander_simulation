#ifndef TERRAIN_HPP  
#define TERRAIN_HPP  

#include <vector>  
#include "../MLX42/include/MLX42/MLX42.h"

class Terrain {  
public:
    Terrain();
    void generate();
    void draw(mlx_image_t* mlx);
    int getGroundY(float x) const;
    std::vector<std::pair<int, int> > points;
};  

#endif