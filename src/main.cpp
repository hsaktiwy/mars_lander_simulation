#include "Game.hpp"  
#include <iostream>

int main() {  
    try {  
        Game game;
        std::vector<std::pair<int, int>> terrainPoints = {  
            {0, int((1 - 100/3000.0)*800)},      // Point 1  
            {int((1000/7000.0)*1400.0), int((1 - 500/3000.0)*800)},   // Point 2  
            {int((1500/7000.0)*1400), int((1 - 1500/3000.0)*800)},  // Point 3  
            {int((3000/7000.0)*1400), int((1 - 1000/3000.0)*800)},  // Point 4  
            {int((4000/7000.0)*1400), int((1 - 150/3000.0)*800)},   // Point 5  
            {int((5500/7000.0)*1400), int((1 - 150/3000.0)*800)},   // Point 6  
            {int((6999/7000.0)*1400), int((1 - 800/3000.0)*800)}    // Point 7
        };
        game.lander_x = int((2500/7000.0)*1400);
        game.lander_y = int((1 - 2700/3000.0)*800);
        game.run(terrainPoints, 0, 0);
    } catch (const std::exception& e) {  
        std::cerr << "Error: " << e.what() << std::endl;  
        return 1;  
    }  
    return 0;  
}  

/*

*/