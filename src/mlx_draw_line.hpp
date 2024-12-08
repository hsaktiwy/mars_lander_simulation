#ifndef DRAW_LINE_HPP  
#define DRAW_LINE_HPP  

#include "MLX42/MLX42.h"  

// Function to draw a line between two points  
void mlx_draw_line(mlx_image_t* img, int x0, int y0, int x1, int y1, uint32_t color);  
void mlx_image_clear(mlx_image_t *img);
#endif  