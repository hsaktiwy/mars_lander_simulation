#include "Terrain.hpp"  
#include "../MLX42/include/MLX42/MLX42.h"  
#include "mlx_draw_line.hpp"
#include <cstdlib>  
#include <ctime>
#include <utility> // For std::make_pair  

Terrain::Terrain() {  
    // generate();  
}  

void Terrain::generate() {  
    srand(time(0));  
    points.clear();  

    // Generate random terrain  
    points.emplace_back(0, 800);  
    for (int i = 1; i < 10; i++) {  
        points.emplace_back(i * 140, rand() % 300 + 500);  
    }  
    points.emplace_back(1400, 800);  
}  

void Terrain::draw(mlx_image_t* img) {  
    for (size_t i = 1; i < points.size(); i++) {  
        mlx_draw_line(img,  
                      points[i - 1].first, points[i - 1].second,  
                      points[i].first, points[i].second,  
                      0xFFFFFF); // White color  
    }  
} 

int Terrain::getGroundY(float x) const {  
    for (size_t i = 1; i < points.size(); i++) {  
        if (x >= points[i - 1].first && x <= points[i].first) {  
            float slope = (float)(points[i].second - points[i - 1].second) /  
                          (points[i].first - points[i - 1].first);  
            return points[i - 1].second + slope * (x - points[i - 1].first);  
        }  
    }  
    return 800; // Default ground level  
} 