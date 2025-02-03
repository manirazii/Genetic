#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

struct Item {
    int value, weight;
};

class Individual {
public:
    vector<bool> chromosome;
    int fitness;

    Individual(int numItems) : chromosome(numItems), fitness(0) {}

    void calculateFitness(const vector<Item>& items, int maxWeight) {
        int totalValue = 0, totalWeight = 0;
        for (size_t i = 0; i < chromosome.size(); ++i) {
            if (chromosome[i]) {
                totalValue += items[i].value;
                totalWeight += items[i].weight;
            }
        }
        fitness = (totalWeight > maxWeight) ? 0 : totalValue;
    }
};

class GeneticAlgorithm {
    vector<Item> items;
    int maxWeight, populationSize, generations;
    double mutationRate;

public:
    GeneticAlgorithm(const vector<Item>& items, int maxWeight, int populationSize, int generations, double mutationRate)
        : items(items), maxWeight(maxWeight), populationSize(populationSize), generations(generations), mutationRate(mutationRate) {}

    Individual crossover(const Individual& parent1, const Individual& parent2) {
        Individual child(items.size());
        int crossoverPoint = rand() % items.size();
        for (size_t i = 0; i < items.size(); ++i)
            child.chromosome[i] = (i < crossoverPoint) ? parent1.chromosome[i] : parent2.chromosome[i];
        return child;
    }

    void mutate(Individual& individual) {
        for (size_t i = 0; i < individual.chromosome.size(); ++i)
            if ((rand() / double(RAND_MAX)) < mutationRate)
                individual.chromosome[i] = !individual.chromosome[i];
    }

    Individual run() {
        vector<Individual> population(populationSize, Individual(items.size()));
        for (auto& individual : population) {
            for (size_t i = 0; i < items.size(); ++i)
                individual.chromosome[i] = rand() % 2;
            individual.calculateFitness(items, maxWeight);
        }

        for (int gen = 0; gen < generations; ++gen) {
            vector<Individual> newPopulation;
            for (int i = 0; i < populationSize; ++i) {
                Individual parent1 = population[rand() % populationSize];
                Individual parent2 = population[rand() % populationSize];
                Individual child = crossover(parent1, parent2);
                mutate(child);
                child.calculateFitness(items, maxWeight);
                newPopulation.push_back(child);
            }
            population = newPopulation;
        }

        return *max_element(population.begin(), population.end(), [](const Individual& a, const Individual& b) {
            return a.fitness < b.fitness;
        });
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<Item> items = {{60, 10}, {100, 20}, {120, 30}};
    GeneticAlgorithm ga(items, 50, 100, 1000, 0.01);
    Individual bestSolution = ga.run();
    cout << "Best solution found with fitness: " << bestSolution.fitness << "\n";
    for (size_t i = 0; i < bestSolution.chromosome.size(); ++i)
        if (bestSolution.chromosome[i])
            cout << "Item " << i + 1 << " (Value: " << items[i].value << ", Weight: " << items[i].weight << ")\n";
    return 0;
}
