import numpy as np
import random

num_strategies = 10
num_generations = 50
num_parents_mating = 5
mutation_rate = 0.1

def initialize_population():
    return np.random.rand(num_strategies, 5)

def fitness_function(population):
    fitness_scores = []
    for strategy in population:
        score = simulate_game(strategy)
        fitness_scores.append(score)
    return np.array(fitness_scores)

def simulate_game(strategy):
    return np.sum(strategy)

def select_mating_pool(population, fitness):
    parents = np.empty((num_parents_mating, population.shape[1]))
    for parent_num in range(num_parents_mating):
        max_fitness_idx = np.where(fitness == np.max(fitness))
        parents[parent_num, :] = population[max_fitness_idx[0][0], :]
        fitness[max_fitness_idx[0][0]] = -99999999999
    return parents

def crossover(parents):
    offspring = np.empty((num_strategies - parents.shape[0], parents.shape[1]))
    for k in range(offspring.shape[0]):
        parent1_idx = k % parents.shape[0]
        parent2_idx = (k + 1) % parents.shape[0]
        crossover_point = random.randint(1, parents.shape[1] - 1)
        offspring[k, 0:crossover_point] = parents[parent1_idx, 0:crossover_point]
        offspring[k, crossover_point:] = parents[parent2_idx, crossover_point:]
    return offspring

def mutation(offspring):
    for idx in range(offspring.shape[0]):
        if random.random() < mutation_rate:
            random_value = np.random.rand(1)
            offspring[idx, random.randint(0, offspring.shape[1] - 1)] += random_value
    return offspring

def genetic_algorithm():
    population = initialize_population()
    for generation in range(num_generations):
        fitness = fitness_function(population)
        parents = select_mating_pool(population, fitness)
        offspring_crossover = crossover(parents)
        offspring_mutation = mutation(offspring_crossover)
        population[0:parents.shape[0], :] = parents
        population[parents.shape[0]:, :] = offspring_mutation
    return population

final_population = genetic_algorithm()
print("Final Population:\n", final_population)

best_fitness = fitness_function(final_population)
best_strategy_idx = np.argmax(best_fitness)
print("Best Strategy:", final_population[best_strategy_idx])
print("Best Fitness Score:", best_fitness[best_strategy_idx])
