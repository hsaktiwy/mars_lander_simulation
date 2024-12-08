#ifndef LANDER_HPP  
#define LANDER_HPP  

class Lander {  
public:  
    Lander();  
    void update(float dt);  
    void applyThrust(float power);  
    void rotate(float angle);  
    bool checkCollision(int groundY);  
    bool checkLandingConditions() const;
    void debug() const;
    // Getters  
    float getX() const { return x; }  
    float getY() const { return y; }  
    float getRotation() const { return rotation; }  
    float getFuel() const { return fuel; }  
    float x, y;         // Position  
    float fuel;         // Remaining fuel 
    float vx, vy;       // Velocity  
    float rotation;     // Rotation angle (-90 to 90 degrees)  
    float thrust;       // Thrust power (0 to 4)  
private:  
    const float GRAVITY = 3.711f; // Mars gravity (m/s²)  
};  

#endif  