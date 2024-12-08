#ifndef SOLUTION_HPP  
#define SOLUTION_HPP  

#include <iostream>  
#include <string>  
#include <vector>  
#include <algorithm>  
#include <cmath>  
#include <random>  
#include <functional>  
#include <ctime>  
using namespace std;  

#define d_angle 15  
#define d_power 1  

//Struct and Type Definitions  
typedef struct spoint {  
    int x, y;
    spoint(){x=0, y=0;}
    spoint(int x_val, int y_val) : x(x_val), y(y_val) {}
    bool operator==(const spoint &a) const {  
        return this->x == a.x && this->y == a.y;  
    }  
    bool operator!=(const spoint &a) const {  
        return !(*this == a);  
    }
    struct spoint operator-(const struct spoint &other) const {
        struct spoint s = {x - other.x, y - other.y};
        return s;
    }
} s_point;  
std::ostream &operator<<(std::ostream &os, const spoint &p);


typedef struct s_map {  
    vector<s_point> shap;  
   s_point LStart;  
    s_point LEnd;  
} s_mars;  

typedef struct sdata {  
    int x;  
    int y;  
    int hs; // horizontal speed  
    int vs; // vertical speed  
    int f;  // fuel  
    int r;  // rotation angle  
    int p;  // thrust power  
} s_data;  

typedef struct Action {  
    int angle;  
    int power;  
} s_action;  

// Global Variables (Declared as extern)  
extern s_point target;  
extern s_mars map;  
extern s_data hmmm;  
extern float g;  
extern float pi;  
extern int action_size;  

// Function Declarations  
s_action g_action();  
int adjustAngle(int currentAngle, int targetAngle, int maxStep);  
void adjustPower(int &currentPower, int targetPower);  
void calculator(s_data &data, int angle, int power);  
bool collision(s_point &a, s_point &E1, s_point &E2);  
bool collision_mars(s_data &data);  
float distance(int x, int y, int i, int j);  
int fitness(const vector<s_action> &actions);  
vector<s_action> g_list_actions();  
void mutate(vector<s_action> &actions);  
vector<s_action> generate_initial_actions();  
vector<s_action> cross_over(const vector<s_action> &a, const vector<s_action> &b);  

template <typename Gene>
class GeneticAlgorithm {
private:
    using Individual = vector<Gene>;
    using Population = vector<Individual>;

    function<Individual()> generateIndividual;
    function<int(const Individual&)> fitnessFunction;
    function<Individual(const Individual&, const Individual&)> crossoverFunction;
    function<void(Individual&)> mutationFunction;

    int populationSize;
    double mutationRate;
    int eliteSize;

    mt19937 rng;

public:
    vector<Individual> best_in_generation;
    GeneticAlgorithm(
        function<Individual()> genIndividual,
        function<int(const Individual&)> fitFunc,
        function<Individual(const Individual&, const Individual&)> crossFunc,
        function<void(Individual&)> mutFunc,
        int popSize = 100,
        double mutRate = 0.01,
        int eliteCount = 2
    ) : generateIndividual(genIndividual),
        fitnessFunction(fitFunc),
        crossoverFunction(crossFunc),
        mutationFunction(mutFunc),
        populationSize(popSize),
        mutationRate(mutRate),
        eliteSize(eliteCount),
        rng(time(0))
    {}

    Individual evolve(int generations) {
        Population population = initializePopulation();

        for (int gen = 0; gen < generations; ++gen) {
            //cerr << gen << endl;
            vector<pair<int, Individual> > rankedPopulation = rankPopulation(population);
            Population newPopulation = selectElites(rankedPopulation);
            //
                display_cerr(rankedPopulation);
                cerr << "totale marge: " << marge_of_fitness(rankedPopulation) <<endl;
            //
            while (newPopulation.size() < populationSize) {
                Individual parent1 = selectParent(rankedPopulation);
                Individual parent2 = selectParent(rankedPopulation);
                Individual child = crossoverFunction(parent1, parent2);
                if (uniform_real_distribution<>(0, 1)(rng) < mutationRate) {
                    mutationFunction(child);
                }
                newPopulation.push_back(child);
            }
            population = newPopulation;
            best_in_generation.push_back(getBestIndividual(population));
        }
        //cerr<< "out" <<endl;
        return getBestIndividual(population);
        // return population[0];
    }

private:

    void display_cerr(const vector<pair<int, Individual> > &generation)
    {
        int i =0;
        for (auto [fit, actions]:generation)
        {
            i++;
            cerr<< "[" << i << "] = " << fit <<" : ";
            cerr << "{";
            for(auto v : actions)
            {
                cerr << "["<<v.angle << "," <<v.power<< "]" << ",";
            }
            cerr<< "}\n";
        }
    }
    int marge_of_fitness(const vector<pair<int, Individual> >&generation)
    {
        long totale = 0;

        for (auto [fit,actions]: generation)
        {
            totale = totale + fit;
        }
        if (generation.size() != 0)
            totale = totale/(int)generation.size();
        return totale;
    }

    Population initializePopulation() {
        Population population;
        population.reserve(populationSize);
        for (int i = 0; i < populationSize; ++i) {
            population.push_back(generateIndividual());
        }
        return population;
    }

    vector<pair<int, Individual> > rankPopulation(const Population& population) {
        vector<pair<int, Individual> > ranked;
        ranked.reserve(populationSize);
        for (const auto& individual : population) {
            ranked.emplace_back(fitnessFunction(individual), individual);
        }
        sort(ranked.begin(), ranked.end(),[](const std::pair<int, std::vector<Gene>>& a, const std::pair<int, std::vector<Gene>>& b) {
                return a.first > b.first; // Descending order
            });
        return ranked;
    }

    Population selectElites(const vector<pair<int, Individual> >& ranked) {
        Population elites;
        elites.reserve(eliteSize);
        for (int i = 0; i < eliteSize && i < ranked.size(); ++i) {
            elites.push_back(ranked[i].second);
        }
        return elites;
    }

    Individual selectParent(const vector<pair<int, Individual> >& ranked) {
        return ranked[rand()%ranked.size()].second;
    }

    Individual getBestIndividual(const Population& population) {
        return *max_element(population.begin(), population.end(),
            [this](const Individual& a, const Individual& b) {
                return fitnessFunction(a) > fitnessFunction(b);
            });
    }
};
#endif  