#include <functional>
#include <thread>
#include <atomic>

struct Search {
    long numSeeds;
    int numThreads;
    std::array<int, 9> startSeed; // The first entry indicates the length of the seed
    long seedsProcessed = 0;
    long highScore = 0;
    long printDelay = 100000;
    std::function<int(Instance)> filter;
    std::atomic<bool> found{false}; // Atomic flag to signal when a solution is found
    std::array<int, 9> foundSeed;   // Store the found seed

    void searching_thread(int ID) {
        std::array<int, 9> seed = startSeed;
        for (int i = 0; i < ID; i++) {
            nextSeed(seed);
        }
        for (int i = 0; i < (numSeeds - ID) / numThreads; i++) {
            if (found.load()) return; // Exit if another thread found a valid seed

            Instance inst(seedToString(seed));
            long score = filter(inst);
            if (score > 0 && !found.load()) {
                found.store(true);
                foundSeed = seed;
                highScore = score;
                std::cout << "Found seed: " << seedToString(seed) << " (" << score << ")" << std::endl;
                return; // Exit thread after finding the solution
            }
            seedsProcessed++;
            if (seedsProcessed % printDelay == 0) {
                std::cout << seedsProcessed << " seeds searched" << std::endl;
            };
            for (int i = 0; i < numThreads; i++) {
                nextSeed(seed);
            }
        }
    }

    std::string search() {
        std::vector<std::thread> threads;
        for (int i = 0; i < numThreads; ++i) {
            // Bind the member function with the instance of the class
            auto bound_thread_func = std::bind(&Search::searching_thread, this, i);
            // Use a lambda function to capture the bound function and start a thread
            threads.emplace_back([bound_thread_func]() { bound_thread_func(); });
        }
        for (std::thread& t : threads) {
            t.join();
        }
        return seedToString(foundSeed); // Return the found seed
    }

    Search(std::function<int(Instance)> f) {
        filter = f;
        startSeed = stringToSeed("");
        numThreads = 1;
        numSeeds =  2318107019761;
    };
    Search(std::function<int(Instance)> f, int t) {
        filter = f;
        startSeed = stringToSeed("");
        numThreads = t;
        numSeeds =  2318107019761;
    };
    Search(std::function<int(Instance)> f, int t, int n) {
        filter = f;
        startSeed = stringToSeed("");
        numThreads = t;
        numSeeds = n;
    };
    Search(std::function<int(Instance)> f, std::string seed, int t, int n) {
        filter = f;
        startSeed = stringToSeed(seed);
        numThreads = t;
        numSeeds = n;
    };
};
