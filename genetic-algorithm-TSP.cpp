//travelling salesman problem, solve with genetic algorithm
//mani-razi jan.2025

#include <iostream>
#include <vector>
#include<cmath>
#include<random>
#include<ctime>

using namespace std;

const int city_number = 5;
const int popul_size = 100;
const int gener = 1000;
const double meaow = 0.04;
double distances[city_number][city_number] = {
    {0, 10, 15, 20, 25},
    {10, 0, 35, 25, 30},
    {15, 35, 0, 30, 20},
    {20, 25, 30, 0, 15},
    {25, 30, 20, 15, 0}
}
;

class Individual {
public:
    vector<int> deltax;
    double fitness;

    Individual() {
        for (int i = 0; i < city_number; ++i) deltax.push_back(i);
        random_shuffle(deltax.begin(), deltax.end());
        fitness = calculateFitness();
    }

    double calculateFitness() {
        double totalDistance = 0;
        for (int i = 0; i < deltax.size(); ++i)
            totalDistance += distances[deltax[i]][deltax[(i + 1) % deltax.size()]];
        return totalDistance;
    }
}
;

Individual crossover(const Individual& p1, const Individual& p2) {
    Individual child;
    int start = rand() % city_number, end = rand() % city_number;
    if (start > end) swap(start, end);
    vector<bool> visited(city_number, false);
    for (int i = start; i <= end; ++i) {
        child.deltax[i] = p1.deltax[i];
        visited[p1.deltax[i]] = true;
    }
    int j = 0;
    for (int i = 0; i < city_number; ++i) {
        if (!visited[p2.deltax[i]]) {
            while (j >= start && j <= end) j++;
            child.deltax[j++] = p2.deltax[i];
        }
    }
    child.fitness = child.calculateFitness();
    return child;
}

void mutate(Individual& ind) {
    for (int i = 0; i < city_number; ++i)
        if ((rand() / (double)RAND_MAX) < meaow)
            swap(ind.deltax[i], ind.deltax[rand() % city_number]);
}

int main() {
    srand(time(0));
    vector<Individual> population(popul_size);
    for (int gen = 0; gen < gener; ++gen) {
        sort(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
        });
        vector<Individual> newPopulation;
        for (size_t i = 0; i < population.size() / 2; i += 2) {
            Individual child1 = crossover(population[i], population[i + 1]);
            Individual child2 = crossover(population[i + 1], population[i]);
            mutate(child1); mutate(child2);
            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }
        population = newPopulation;

 
        cout << "Generation " << gen << ": Best fitness = " << population[0].fitness << " | Best deltax: ";
        for (int city : population[0].deltax) {
            cout << city << " ";
        }
        cout << endl;
    }
    return 0;
}
