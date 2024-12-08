// #undef _GLIBCXX_DEBUG                // disable run-time bound checking, etc
// #pragma GCC optimize("Ofast,inline") // Ofast = O3,fast-math,allow-store-data-races,no-protect-parens

// #ifndef __POPCNT__ // not march=generic

// #pragma GCC target("bmi,bmi2,lzcnt,popcnt")                      // bit manipulation
// #pragma GCC target("movbe")                                      // byte swap
// #pragma GCC target("aes,pclmul,rdrnd")                           // encryption
// #pragma GCC target("avx,avx2,f16c,fma,sse3,ssse3,sse4.1,sse4.2") // SIMD

// #endif // end !POPCNT
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <algorithm>
#include <random>
#include <functional>
#include <ctime>
using namespace std;
# define d_angle 15
# define d_power 1
#include "Solution.hpp"

s_point  target;
s_mars map;
s_data hmmm;
float g = 3.711;
float pi = 3.1415;
int negative = 0;
mt19937 _rng;

std::ostream &operator<<(std::ostream &os, const spoint &p)
{
    return os << "(("<< p.x << " " << p.y <<"))";
}

float points_angle(s_point a, s_point b)
{
    return (round(atan2(b.y - a.y, b.x-a.x) * (180/M_PI)));
}

s_action g_action()
{
    return (s_action{(rand()%2 ? -1:1)* (rand()%45), rand()%5});
}

int adjustAngle(int currentAngle, int targetAngle, int maxStep = 15) {
    // Clamp the target angle to the range [-90, 90]
    targetAngle = max(-90, min(90, targetAngle));

    int angleDiff = targetAngle - currentAngle;

    // Determine the adjustment direction and limit it to maxStep
    if (abs(angleDiff) <= maxStep)
        return targetAngle;
    else
        return currentAngle + (angleDiff > 0 ? maxStep : -maxStep);
}

void adjustPower(int &currentPower, int targetPower) {
    // Clamp the target power between 0 and 4
    targetPower = max(0, min(4, targetPower));

    // Adjust power by a maximum of ±1 per turn
    if (currentPower < targetPower) {
        currentPower++;
    } else if (currentPower > targetPower) {
        currentPower--;
    }
}

void calculator(s_data& data, int angle, int power)
{
    if (angle < 0)
        negative++;
    adjustPower(data.p, power);
    data.r = adjustAngle(data.r, angle);
    data.hs =  data.hs + round(data.p *cos(data.r * (pi/180)));
    float thrust_vertical = data.p * sin(data.r * (pi/180));
    float gravity_effect = g;
    data.vs = data.vs + round(gravity_effect -  thrust_vertical);// this will need to be changesd to = data.vs + round(thrust_vertical - gravity_effect ) in the coding game
    data.f = data.f-1;
    data.x =data.x + data.hs;
    data.y =data.y + data.vs;
}

bool collision(s_point &a, s_point &E1, s_point &E2)
{
    double crossProduct = (a.x - E1.x) * (E2.y - E1.y) - (a.y - E1.y) * (E2.x - E1.x);
    if (abs(crossProduct) > 1e-9) return false;  // Tolerance for floating-point error

    // Step 2: Check if point is within the segment's bounding box
    if (a.x < min(E1.x, E2.x) || a.x > max(E1.x, E2.x)) return false;
    if (a.y < min(E1.y, E2.y) || a.y > max(E1.y, E2.y)) return false;
    return true;
}

// int cross_product(const s_point &v1, const s_point &v2) {
//     return v1.x * v2.y - v1.y * v2.x;
// }

// // Function to check if s_point p is above the line segment from a to b
// bool is_above(const s_point &p, const s_point &a, const s_point &b) {
//     return cross_product(p - a, b - a) < 0;
// }


bool collisions_mars(s_data &data, int &debug)
{
    vector<s_point> &shap = map.shap;
    s_point &start = map.LStart;
    s_point &end = map.LEnd;
    int window = 800;
    // convert to match tmy simulation
    s_point s = {data.x, 800 - data.y};
    for (size_t i=1; i < shap.size(); i++)
    {
        if (debug == 0)
            std::cerr << "[" << shap[i-1].x << " " << shap[i-1].y << "]" << " [" << shap[i].x << " " << shap[i].y << "] = " << s <<  "target point start " << start << " targetend point end " << end <<endl;
        if (shap[i-1] != start || shap[i] != end)
        {
            if (collision(s, shap[i-1], shap[i]))
                return true;
            // if  (!is_above(s, shap[i-1], shap[i]))
            //     return true;
        }
    }
    return false;
}

float distance(int x, int y, int i, int j)
{
    return (sqrt((x - i)*(x - i) + (y - j)*(y - j)));
}
// FOR debuging
void debug_s_data(const s_data &data) {
    std::cerr << "Debugging s_data:\n";
    std::cerr << "x: " << data.x << "\n";
    std::cerr << "y: " << data.y << "\n";
    std::cerr << "hs: " << data.hs << "\n";
    std::cerr << "vs: " << data.vs << "\n";
    std::cerr << "f: " << data.f << "\n";
    std::cerr << "r: " << data.r << "\n";
    std::cerr << "p: " << data.p << "\n";
    std::cerr << "---------------------\n";
}

int fitness(const vector<s_action>& actions) {
    int score = 0;
    s_data data = hmmm;
    static int i = 0;

    float last = distance(data.x, data.y, target.x, target.y);
    for (auto [angle, power] : actions) {
        if (i == 0)
        {
            std::cerr<< "before : calculation _power:" << power << " _angle: " <<angle <<std::endl;
            debug_s_data(data);
        }
        calculator(data, angle, power);
        if (collisions_mars(data, i)) {
            score -=1000; // Heavy penalty for crashing
            std::cerr << "Collision detected" << std::endl;
            break;
        }
        // check for the bound limitation
        else if (0> data.x || data.x >=1400 || data.y < 0 || data.y >= 800)// in our simulation limits are 1400 , 800 in coding game 7000, 3000
        {
            score -= 5000;
            break;
        }

        if(i == 0)
        {
            std::cerr << "after update:\n";
            debug_s_data(data);
            std::cerr << "Collisions : " << ((collisions_mars(data, i)) ? "True" : "False") << std::endl;
        }
        // float current = distance(data.x,data.y, target.x,target.y);
        // if (data.x >= map.LStart.x && data.x <= map.LEnd.x && abs(data.y - target.y ) < 1000) {
        //     score -= max(0, abs(data.hs) - 20) * 5;
        //     score -= max(0, abs(data.vs) - 40) * 5;

        //     // s_point a = {data.x, data.y};
        //     score += (data.r == 0)*100;
        //     score += (abs(target.y - data.y) < 10) * 100;
        // }

        // if (data.x >= map.LStart.x && data.x <= map.LEnd.x && abs(data.y - target.y ) < 500) {
        //         //cerr<< "wtf :  << hs: " << abs(data.hs) << " vs: " << abs(data.vs) << endl;
        //     if (abs(data.hs) - 20  > 0 || abs(data.vs) - 40 > 0 || !(data.r == 0) || data.y - target.y  < 0)
        //     {
        //        // cerr << "?\n";
        //         score-=10000;
        //         break;
        //     }
        // }
        // lower the distance the better
        // if (current < last)
        //     score+=20;
        // if (current - last < 1000)
        //     score+=10;
        // else if (current > last && !(data.x >= map.LStart.x && data.x <= map.LEnd.x))
        //     score-=10;
        // else if(data.x >= map.LStart.x && data.x <= map.LEnd.x)
        //     score+=10;
        // last = current;
    }
    if (i == 0)
        std::cerr  << "Score : " << score << std::endl;
    i++;
    return score;
}

int action_size;
vector<s_action> g_lists_actions()
{
    vector<s_action> vec;

    vec.reserve(action_size);
    for (int i = 0; i < action_size; i++)
    {
        vec.push_back(g_action());
    }
    return vec;
}

void mutate(vector<s_action> &actions) {  
    if (actions.empty()) return; // No mutation if the vector is empty  

    // Randomly select an index in the vector to mutate  
    int index = rand() % actions.size();  

    // Randomly decide whether to mutate the angle or the power  
    bool mutate_angle = rand() % 2;  

    if (mutate_angle) {  
        // Mutate the angle: Add a random value between -15 and 15  
        actions[index].angle += (rand() % 31 - 15); // Random value in range [-15, 15]  
        // Clamp the angle to the valid range [-90, 90]  
        actions[index].angle = max(-45, min(45, actions[index].angle));  
    } else {  
        // Mutate the power: Add a random value between -1 and 1  
        actions[index].power += (rand() % 3 - 1); // Random value in range [-1, 1]  
        // Clamp the power to the valid range [0, 4]  
        actions[index].power = max(0, min(4, actions[index].power));  
    }  
}  

// Initialize a random number generator. It's best to initialize this once, possibly globally.  
std::mt19937 rng(std::random_device{}());  

// Function to clamp values within a range  
template <typename T>  
T clamp_value(T value, T min_val, T max_val) {  
    return std::max(min_val, std::min(max_val, value));  
}  

// Refactored generate_initials_actions function  
vector<s_action> generate_initial_actions() {    
    vector<s_action> actions;    
    actions.reserve(action_size);  

    // Distributions for random deltas  
    std::uniform_int_distribution<int> angle_delta_dist(-15, 15);  
    std::uniform_int_distribution<int> power_delta_dist(-1, 1);  

    // Initialize target_angle and target_power based on current state  
    int target_angle = hmmm.r;    
    int target_power = hmmm.p; // Start with a moderate power to control descent    
    // Initialize the first action with target_angle and target_power  
    target_angle = clamp_value(target_angle, -90, 90);  
    target_power = clamp_value(target_power, 0, 4);  
    actions.emplace_back(s_action{target_angle, target_power});  

    // Generate the remaining actions based on the previous action  
    for (int i = 1; i < action_size; ++i) {  
        // Get the previous action  
        s_action prevs_action = actions.back();  

        // Generate random deltas  
        int angle_delta = angle_delta_dist(rng);
        int power_delta = power_delta_dist(rng);

        // Calculate new angle and power  
        int new_angle = clamp_value(prevs_action.angle + angle_delta, -90, 90);
        int new_power = clamp_value(prevs_action.power + power_delta, 0, 4);

        // Append the new action to the list  
        actions.emplace_back(s_action{new_angle, new_power});
    }
    return actions;    
}

//#include <vector>





vector<s_action> cross_over(const vector<s_action> &a, const vector<s_action> &b) {
    if (a.size() != b.size()) {
        throw invalid_argument("Parent vectors must have the same size for crossover.");
    }

    int size = a.size();
    vector<s_action> child(size);

    uniform_real_distribution<> dist(0.0, 1.0); // Random real number between 0 and 1

    for (int i = 0; i < size; i++) {
        float r = dist(_rng); // Generate a random weight
        child[i].power = round(a[i].power * r + b[i].power * (1 - r)); // Weighted average with rounding
        child[i].angle = round(a[i].angle * r + b[i].angle * (1 - r)); // Weighted average with rounding

        // Ensure power and angle remain within valid bounds
        child[i].power = max(0, min(child[i].power, 4));   // Power is clamped between 0 and 4
        child[i].angle = max(-90, min(child[i].angle, 90)); // Angle is clamped between -90 and 90
    }

    return child;
}


// int main()
// {
//     int n; // the number of points used to draw the surface of Mars.
//     cin >> n; cin.ignore();
   
//     map.shap.reserve(30);
//     int lastX = -1, lastY = -1;
//     for (int i = 0; i < n; i++) {
//         int land_x; // X coordinate of a surface point. (0 to 6999)
//         int land_y; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
//         cin >> land_x >> land_y; cin.ignore();
//         if (lastY == land_y && abs(land_x - lastX) >=1000)
//         {
//             // identifie my landing
//             map.LStart.x = lastX, map.LStart.y = lastY;
//             map.LEnd.x = land_x, map.LEnd.y = land_y;
//             cerr<< lastX << " " << lastY << " " << land_x << " " << land_y <<endl;
//         }
//         lastX = land_x,lastY = land_y;
//         map.shap.emplace_back(land_x, land_y);
//     }
//     target = {map.LStart.x + abs(map.LStart.x - map.LEnd.x)/2, map.LStart.y};
//     cerr << target.x << " " << target.y << endl;
//     // game loop

//     int r = 0;
//     action_size = 40;
//     int s = action_size;
//     vector<s_action> actions;
//     // int fetness = 40;
//     while (1) {
//         cin >> hmmm.x >> hmmm.y >> hmmm.hs >> hmmm.vs >> hmmm.f >> hmmm.r >> hmmm.p; cin.ignore();
//         s_point current_pos = {hmmm.x,hmmm.y};
//         cerr << "S:X:Y => " <<  map.LStart.x << "   " <<  map.LStart.y << endl;
//         cerr << "E:X:Y => " <<  map.LEnd.x << "   " <<  map.LEnd.y << endl;

//         if (s == action_size)
//         {
//             s = 0;
//             int population_size = 10;
//             int elites = round(population_size *0.2);
//             GeneticAlgorithm<s_action> gene(generate_initials_actions,fitness, cross_over,mutate,population_size,0.01,elites);
//             actions = gene.evolve(10);
//             cerr <<  ">??" << actions.size()<< "\n";
//         }
//         cerr <<  actions[s].angle << " " << actions[s].power << endl;
//         // calculator(hmmm, actions[s].angle, actions[s].power);
//         cout <<  actions[s].angle << " " << actions[s].power << endl;
//         cerr << "x: " << hmmm.x << "m Y: " << hmmm.y << "m Horizontal Speed: " << hmmm.hs << "m/s Vertical Speed: " << hmmm.vs << "m/s Fuel: " << hmmm.f << "l Angle: " << hmmm.r << "° Power: " << hmmm.p << " (" << hmmm.p << "m/s²)" << endl;
//         cerr << "negative angles :" << negative <<endl;
//         r++;
//         s++;
//     }
// }