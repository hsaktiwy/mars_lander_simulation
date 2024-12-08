#include "mlx_draw_line.hpp"  
#include <cmath> // For abs()  
#include <string.h>

void mlx_draw_line(mlx_image_t* img, int x0, int y0, int x1, int y1, uint32_t color) {  
    int dx = abs(x1 - x0);  
    int dy = abs(y1 - y0);  
    int sx = (x0 < x1) ? 1 : -1;  
    int sy = (y0 < y1) ? 1 : -1;  
    int err = dx - dy;  

    while (true) {  
        // Draw the pixel at the current position  
        if (x0 >= 0 && x0 < (int)img->width && y0 >= 0 && y0 < (int)img->height) {  
            mlx_put_pixel(img, x0, y0, color);  
        }  

        // Check if we've reached the end of the line  
        if (x0 == x1 && y0 == y1) {  
            break;  
        }  

        // Update the error term and the current position  
        int e2 = 2 * err;  
        if (e2 > -dy) {  
            err -= dy;  
            x0 += sx;  
        }  
        if (e2 < dx) {  
            err += dx;  
            y0 += sy;  
        }  
    }  
}

void mlx_image_clear(mlx_image_t *img)
{
    memset(img->pixels, 0, img->width * img->height * sizeof(int32_t));
}