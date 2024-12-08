// #include "Game.hpp"  
// #include "mlx_draw_line.hpp"
// #include <iostream> 

// using namespace std;

// Game::Game() : gameOver(false) {  
//     mlx = mlx_init(1400, 800, "Mars Lander Emulator", true);  
//     if (!mlx)  
//         throw std::runtime_error("Failed to initialize MLX42");  

//     // Create an image for drawing  
//     img = mlx_new_image(mlx, 1400, 800);  
//     if (!img)  
//         throw std::runtime_error("Failed to create image");  
//     mlx_image_clear(img);
//     mlx_image_to_window(mlx, img, 0, 0);  
// }  

// void Game::processInput() {  
//     if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))  
//         mlx_close_window(mlx);  
//     if (mlx_is_key_down(mlx, MLX_KEY_LEFT))  
//         lander.rotate(lander.getRotation() - 5);  
//     if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))  
//         lander.rotate(lander.getRotation() + 5);  
//     if (mlx_is_key_down(mlx, MLX_KEY_UP))  
//         lander.applyThrust(4.0f);  
//     if (mlx_is_key_down(mlx, MLX_KEY_DOWN))  
//         lander.applyThrust(0.0f);  
// }  

// void Game::update(float dt) {  
//     if (gameOver)  
//         return;  

//     lander.update(dt);  

//     // Check for collision
//     if (lander.checkCollision(terrain.getGroundY(lander.getX()))) {  
//         gameOver = true;  
//         std::cout << "Game Over! You crashed.\n";  
//     }
// }  

// void Game::render() {  
//     // mlx_image_clear(img); // Clear the image with black  

//     // Draw terrain  
//     terrain.draw(img);  

//     // Draw lander  
//     mlx_put_pixel(img, lander.getX(), lander.getY(), 0xFFFFFF);  
// }  

// // Static function for the loop hook  
// static void game_loop(void* param) {  
//     Game* game = static_cast<Game*>(param);  

//     // Calculate delta time  
//     clock_t startTime = clock();  
//     clock_t currentTime = clock();
//     float dt=(float)(currentTime - startTime) / CLOCKS_PER_SEC;
//     while (dt < 0.1)
//     {
//         dt = (float)(currentTime - startTime) / CLOCKS_PER_SEC;
//         currentTime = clock();
//     }
//     // cout << "dt : " << dt <<endl;

//     // Process input, update, and render  
//     game->processInput();  
//     game->update(dt);  
//     game->render();  
// }

// void Game::run() {  
//     // Register the static function as the loop hook 
//     startTime = clock();
//     mlx_loop_hook(mlx, game_loop, this);  

//     // Start the MLX42 loop  
//     mlx_loop(mlx);  
//     mlx_terminate(mlx);  
// } 
#include "Game.hpp"  
#include "mlx_draw_line.hpp"  
#include <iostream>  
#include <cmath>  
#include <stdexcept>  
#include <vector>  
#include <random>  
#include <functional>  
#include "Solution.hpp" // Include your genetic algorithm header  

using namespace std;  

Game::Game() : gameOver(false), currentActionIndex(0), actionSize(40) {  
    mlx = mlx_init(1400, 800, "Mars Lander Emulator", true);  
    if (!mlx)  
        throw std::runtime_error("Failed to initialize MLX42");  

    // Create an image for drawing  
    img = mlx_new_image(mlx, 1400, 800);  
    if (!img)  
        throw std::runtime_error("Failed to create image");  
    mlx_image_clear(img);  
    mlx_image_to_window(mlx, img, 0, 0);  

    // Initialize the action sequence  
    // generateActions(); 
    // intializate the map 
}  

void Game::generateActions() {  
    // Generate actions using your solution
    hmmm.f = 500,
    hmmm.hs = 0,
    hmmm.p = 0,
    hmmm.r = 0,
    hmmm.vs = 0,
    hmmm.x = lander_x,
    hmmm.y = lander_y;
    int populationSize = 100;  
    int elites = round(populationSize * 0.2);  

    // Initialize the genetic algorithm  
    GeneticAlgorithm<s_action> gene(  
        generate_initial_actions, // Function to generate initial actions  
        fitness,                  // Fitness function  
        cross_over,               // Crossover function  
        mutate,                   // Mutation function  
        populationSize,           // Population size  
        0.01,                     // Mutation rate  
        elites                    // Number of elites  
    );  
    cerr<< "wala" <<endl;
    // Evolve the population to generate the best sequence of actions  
    actions = gene.evolve(gen_limit);
    action_list = gene.best_in_generation;

    // Reset the current action index  
    currentActionIndex = 0;  
}  

void Game::processInput() {
    if (gameOver)
    {
        if (currect_generation+1 < gen_limit)
        {
            currect_generation++;
            cout << "GENERATION : " << currect_generation <<endl;
            gameOver = false;
            // re init the lander initial possition
            this->lander.x = this->lander_x;
            this->lander.y = this->lander_y;
            this->lander.thrust = 0;
            this->lander.fuel = 500;
            this->lander.vx = 0;
            this->lander.vy = 0;
            this->lander.rotation = 0;
            currentActionIndex = 0;
        }
        return;
    }

    // Check if we need to generate a new sequence of actions  
    if (currentActionIndex >= actions.size()) {
        // action_size = 100;
        // if (currect_generation == 0)
        // generateActions();  
        currentActionIndex = 0;
    }
    // Get the current action  
    // s_action currentAction = actions[currentActionIndex];
    //cout <<"hmm" << currect_generation << " " << currentActionIndex <<endl;
    s_action currentAction = action_list[currect_generation][currentActionIndex];

    // Apply the action to the lander  
    lander.rotate(currentAction.angle);
    lander.applyThrust(currentAction.power);
    // Move to the next action  
    currentActionIndex++;
    // cout << currentAction.angle << " " << currentAction.power << endl;
}  

void Game::update(float dt) {  
    if (gameOver)
    {
        return;
    }  

    // Update the lander's state  
    lander.update(dt);  
    if (currect_generation == 0)
    {
        this->lander.debug();
    }
    // Check for collision  
    if (lander.checkCollision(terrain.getGroundY(lander.getX()))) {  
        gameOver = true;  
        std::cout << "Game Over! You crashed.\n";  
    }  

    // Check for successful landing  
    // if (lander.checkLandingConditions()) {  
    //     gameOver = true;  
    //     std::cout << "Landing successful!\n";  
    // }  
}  

void Game::render() {  
    // Clear the image  
    // mlx_image_clear(img);  

    // Draw the terrain  
    terrain.draw(img);  

    // Draw the lander  
    mlx_put_pixel(img, lander.getX(), lander.getY(), 0xFFFFFFFF);
    // mlx_image_t* text = mlx_put_string(mlx, to_string(currect_generation).c_str(), 0, 0); 
}  

// Static function for the loop hook  
static void game_loop(void* param) {  
    Game* game = static_cast<Game*>(param);  

    // Calculate delta time  
    clock_t lastTime = clock();  
    clock_t currentTime = clock();  
    float dt = (float)(currentTime - lastTime) / CLOCKS_PER_SEC;
    // while (dt < 0.05)
    // {
    //     currentTime = clock();
    //     dt = (float)(currentTime - lastTime) / CLOCKS_PER_SEC;
    // }
    // Process input, update, and render
    game->processInput();  
    game->update(dt);  
    game->render();  
}  

void Game::run(std::vector<std::pair<int, int> > &terrainPoints, int power, int angle) {
    this->terrainPoints = terrainPoints;
    this->init_power= power;
    this->init_angle= angle;
    this->terrain.points = terrainPoints;
    this->lander.x = lander_x;
    this->lander.y = lander_y;
    this->lander.fuel = 500;
    gen_limit = 10;
    currect_generation = 0;
    action_size = 100;
    // inite the map
    map.shap.reserve(30);
    int lastX = -1, lastY = -1;
    for (size_t i = 0; i < terrainPoints.size(); i++) {
        int land_x = terrainPoints[i].first; // X coordinate of a surface point. (0 to 6999)
        int land_y = 800 - terrainPoints[i].second; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
        if (lastY == land_y)
        {
            // identifie my landing
            map.LStart.x = lastX, map.LStart.y = lastY;
            map.LEnd.x = land_x, map.LEnd.y = land_y;
            cout<< lastX << " " << lastY << " " << land_x << " " << land_y <<endl;
        }
        lastX = land_x,lastY = land_y;
        map.shap.emplace_back(land_x, land_y);
    }
    target = {map.LStart.x + abs(map.LStart.x - map.LEnd.x)/2, map.LStart.y};
    cout << target.x << " " << target.y << endl; 

    generateActions();
    cerr << "to the game loop\n";
    // Register the static function as the loop hook  
    mlx_loop_hook(mlx, game_loop, this);  

    // Start the MLX42 loop  
    mlx_loop(mlx);  
    mlx_terminate(mlx);  
}  