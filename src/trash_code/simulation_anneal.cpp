
/*
int fitness(const vector<s_action>& actions) {  
    int score = 0;  
    s_data real = hmmm;  
    float lastDistance = distance(real.x, real.y, target.x, target.y);

    for (auto [angle, power] : actions) {
        calculator(real, angle, power);

        if (collisions_mars(real)) {  
            score -= 1000; // Heavy penalty for crashing  
            break;  
        }

        float currentDistance = distance(real.x, real.y, target.x, target.y);
        score -= 5 * abs(real.hs); // Penalize horizontal speed
        score -= 10 * abs(real.vs - 5); // Penalize vertical speed deviation from a safe threshold
        if (abs(target.y - real.y) < 100)
            score += (real.r == 0)*10;
        if (currentDistance < lastDistance) {
            score += 20; // Reward for closing distance to target
        } else {  
            score -= 10; // Penalty for increasing distance
        }  

        lastDistance = currentDistance;
    }  

    return score;
}

int simulation(vector<s_action>& obj, s_data data)
{
    random_device rd;  // Obtain a random number from hardware
    mt19937 eng(rd()); // Seed the generator
    uniform_real_distribution<> distr(0.0, 1.0); // Define the range
    vector<s_action> bestSolution = obj;
    vector<s_action> currentSolution = obj;
    int closer,far, collision;
    float coolDownRate = 0.99;
    float best_fitness = fitness(currentSolution, data, closer,far, collision);
    float curr_fitness = best_fitness;

    int calculation = 100000;
    float temperatore = 10000 *  0.1;
    int update = 0;
    int risk = 0;
    cerr<< "best fitness : " << best_fitness<<endl;
    s_point current = {data.x, data.y};
    float p = points_angle(target, current);
    int degree = (p * (180/M_PI));
    cerr << degree <<endl;
    //cerr << p << "\n";
    int pos = 1;
    if (p > 90)
        pos= -1;
    for(int x = calculation; x > 0; x--)
    {
        // randomization logic 
        // ...
        s_action ra = gs_action();
        int r2 = rand()%currentSolution.size();
        currentSolution[r2] = ra;
        // Calculate our soluation fitness after it n
        float fitness2 = fitness(currentSolution, data, closer,far, collision);

        // identify if we are going with the worst solution or we will drop it or tack it in case it is better then the previous one
        // if (fitness2 < curr_fitness)
        // {
        //     double acceptanceProb = exp((curr_fitness - fitness2) / temperatore);
        //     if (acceptanceProb > distr(eng)) {
        //         risk++;
        //         curr_fitness = fitness2;
        //     }
        //     // else
        //     // {
        //     //     // reverse our changes as we will keep the previous solution swap, suffle , inverse , ...
        //     //     // ..
                
        //     // }
        // }
        // else
        // if(fitness2)
        // {
        if (fitness2 > best_fitness)
        {
            update++;
            bestSolution = currentSolution, best_fitness = fitness2;
        }
        //     curr_fitness = fitness2;
        // }

        // drop our temperatore to drop our probability of accepting worst solution if they appear
        //cerr<< "closer : " << closer <<  " far :" << far << " collision: " <<collision<<endl; 
        temperatore *= coolDownRate;
    }
    obj = bestSolution;
    cerr << "Updates : " << update << " " << ", risks: " << risk << " best fitness : " <<best_fitness <<endl;
    return best_fitness;
}
*/

/*
vector<s_action> generate_initials_actions() {  
    vector<s_action> actions;  
    int target_angle = 0;  
    int target_power = 3; // Start with a moderate power to control descent  

    // Calculate initial angle to steer towards the landing zone horizontally  
    if (hmmm.x < map.LStart.x) {  
        target_angle = -15; // Adjust to the right  
    } else if (hmmm.x > map.LEnd.x) {  
        target_angle = 15; // Adjust to the left  
    }  

    // Adjust power based on vertical speed and altitude  
    if (hmmm.vs < -40) {  
        target_power = 4; // Increase thrust to slow down descent  
    } else if (hmmm.vs > -10) {  
        target_power = 3; // Decrease thrust to accelerate descent  
    }  

    // Generate a sequence of actions based on the initial estimates  
    for (int i = 0; i < action_size; i++) { // Assume 30 steps to land  
        actions.push_back({s_action{target_angle, target_power}});  
        // Gradually reduce the angle to stabilize as it approaches the landing zone  
        target_angle = (int)(target_angle * 0.9);  
    }  

    return actions;  
}

*/

/*
int fitness(vector<s_action>& actions, s_data &real)
{
    int score = 0;
    s_data data = real;
    for (auto [angle, power]: actions)
    {
        calculator(data, angle, power);
        score += (-32) * collisions_mars(data);
        if (data.x >= map.LStart.x && data.x<= map.LEnd.x)
        {
            score += (abs(data.hs) <= 20)*100 - (abs(data.hs) > 20)*100;
            score += (abs(data.vs) <= 40)*100 - (abs(data.vs) > 40)*100;
            s_point a = {data.x, data.y};
            score += 100 - abs(points_angle(target,a) - 90);
            score += (abs(target.y - data.y) < 10)*100;
        }
    }
    return (score);
}
*/