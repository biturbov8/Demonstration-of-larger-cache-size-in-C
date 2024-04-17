#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CACHE_SIZE 2048  // Maximum cache size 
#define ARRAY_SIZE 10000     // Size of the data array
#define ITERATIONS 100000   // Number of iterations for measurements

typedef struct {
    int value;
    int age;  // for LRU replacement
} CacheEntry;

CacheEntry cache[MAX_CACHE_SIZE];
int cache_size; // Actual cache size, adjustable within MAX_CACHE_SIZE
int num_misses;
int hit_time;  // Cumulative sum of hit times
int total_time; // Cumulative sum of total access times

// Simulate memory access time:
int memory_access_time() {
    return rand() % 100 + 10; // Random value between 10 and 109 nanoseconds
}

// Simulate cache access time:
int cache_access_time() {
    return 1; // Fixed cost, assuming significantly faster than memory
}

// Implement Least Recently Used (LRU) replacement policy:
void update_lru(int accessed_index) {
    for (int i = 0; i < cache_size; i++) {
        if (i != accessed_index) {
            cache[i].age++;
        } else {
            cache[i].age = 0; // Most recently accessed, reset age
        }
    }
}

// Simulate a cache access for a given address:
int cache_access(int address) {
    for (int i = 0; i < cache_size; i++) {
        if (cache[i].value == address) { // Cache hit
            hit_time += cache_access_time();
            update_lru(i);
            return cache[i].value;
        }
    }

    // Cache miss
    num_misses++;
    int evicted_index = -1;

    // Find victim for LRU replacement:
    int max_age = -1;
    for (int i = 0; i < cache_size; i++) {
        if (cache[i].age > max_age) {
            max_age = cache[i].age;
            evicted_index = i;
        }
    }

    // Load data from memory and update cache:
    total_time += memory_access_time();
    cache[evicted_index].value = address;
    cache[evicted_index].age = 0;

    return address;
}

int main() {
    srand(time(NULL)); // Seed random number generator

    printf("Cache Sizes\tMiss Rate\tAvg. Hit Time\tAvg. Access Time\n");

    for (cache_size = 16; cache_size <= MAX_CACHE_SIZE; cache_size *= 2) {
        num_misses = 0;
        hit_time = 0;
        total_time = 0;

        int data_array[ARRAY_SIZE];
        for (int i = 0; i < ARRAY_SIZE; i++) {
            data_array[i] = i; // Initialize data array
        }

        // Simulate cache behavior:
        for (int i = 0; i < ITERATIONS; i++) {
            int index = rand() % ARRAY_SIZE; // Randomly access data array
            cache_access(data_array[index]);
        }

        double miss_rate = (double)num_misses / ITERATIONS;
        double avg_hit_time = (double)hit_time / (ITERATIONS - num_misses);
        double avg_access_time = (double)total_time / ITERATIONS;

        printf("%d\t\t%.2f%%\t\t\t%.2f ns\t\t\t%.2f ns\n",
               cache_size, miss_rate * 100, avg_hit_time, avg_access_time);
    }

    return 0;
}
