#include "lander.hpp"  
#include <cmath>  
#include <iostream>
#include <algorithm>

Lander::Lander() : x(700), y(100), vx(0), vy(0), rotation(0), thrust(0), fuel(1000) {}

void Lander::debug() const {
    std::cerr << "Mars Lander State:\n"
                << "  Position: (" << x << ", " << y << ")\n"
                << "  Fuel: " << fuel << " units\n"
                << "  Velocity: (vx=" << vx << ", vy=" << vy << ")\n"
                << "  Rotation: " << rotation << " degrees\n"
                << "  Thrust: " << thrust << " units\n"
                << "-----------------------------------\n";
}

void Lander::update(float dt) { 
    static int i = 0;
    // Apply gravity  
    vy +=GRAVITY;
    dt = 0.1;
    // Apply thrust if there's fuel  
    if (fuel > 0 && thrust > 0) {  
        float rad = rotation * (M_PI / 180.0f);  
        vx += thrust * sin(rad);  
        vy = vy - thrust * cos(rad);  
        // fuel -= thrust;  
    }
    // Update position  
    x += round(round(vx)*0.1);  
    y += round(round(vy)*0.1);
    //std::cerr << "[round " << i << "] vx : " << round(vx) << " vy: " << round(vy) << " x: "  << x <<  " y: " << y <<std::endl;
    i++;
}  

void Lander::applyThrust(float power) {  
    // if (power >= 0 && power <= 4)  
    //     thrust = power;
    //std::cout << "before Update["<< power <<"]" <<" Trust :" << thrust <<std::endl;
    if (power >= 0 && power <= 4)
    {
        if (power > thrust)
        {
            thrust +=1;
            fuel--;
        }
        else if(power <  thrust)
        {
            thrust -=1;
        }
        else if(power == thrust)
            fuel--;
    }
    else
    {
        // std::cerr << "invalide power\n" << std::endl;
    }
    // std::cout << "Trust :" << thrust <<std::endl;
}  
// rotation -7 angle 15
void Lander::rotate(float angle) {  
    int anglediff = angle - rotation;
    if (abs(anglediff) <= 15)
        rotation = angle;
    else if (abs(anglediff) != 0)
        rotation += (anglediff > 0 ? 15 : -15);
    //std::cout << "rotation angle :" << rotation <<std::endl;
}  

bool Lander::checkCollision(int groundY) {
    if (x <=0 || x >= 1400)
        return true;
    return y >= groundY || y <= 0;  
}

bool Lander::checkLandingConditions() const {
    // Check if the lander is in a vertical position
    if (rotation != 0) {  
        return false;  
    }  

    // Check if the vertical speed is within the limit  
    if (std::abs(vy) > 40) {  
        return false;  
    }  

    // Check if the horizontal speed is within the limit  
    if (std::abs(vx) > 20) {  
        return false;  
    }  

    return true; // Landing is successful  
} 