#include "../src/simulator.hpp"
#include "../src/fifo.hpp"
#include "../src/lru.hpp"
#include "../src/clock.hpp" 
#include "../src/optimal.hpp" 
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./engine <path_to_trace_file>\n";
        return 1;
    }

    std::string filepath = argv[1];
    Simulator sim;
    
    std::cout << "Booting Virtual Memory Simulator...\n";
    if (!sim.loadTrace(filepath)) return 1;

    // Grab a reference to the trace we just loaded
    const std::vector<PageID>& futureTrace = sim.getTrace();

    int frameSize = 10;
    std::cout << "\n--- Starting Benchmark [Frames: " << frameSize << "] ---\n";

    FIFO fifo(frameSize);
    std::cout << fifo.getName() << " Faults: " << sim.run(&fifo) << "\n";

    LRU lru(frameSize);
    std::cout << lru.getName() << " Faults:  " << sim.run(&lru) << "\n";

    Clock clock_algo(frameSize);
    std::cout << clock_algo.getName() << " Faults: " << sim.run(&clock_algo) << "\n";

    Optimal optimal(frameSize, futureTrace);
    std::cout << clock_algo.getName() << " Faults: " << sim.run(&optimal) << "\n";

    std::cout << "--------------------------------------\n";

    return 0;
}