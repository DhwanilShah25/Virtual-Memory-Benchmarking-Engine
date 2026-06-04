#include "../src/simulator.hpp"
#include "../src/fifo.hpp"
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./belady_detector <path_to_cyclic_trace>\n";
        return 1;
    }

    std::string filepath = argv[1];
    Simulator sim;
    
    if (!sim.loadTrace(filepath)) {
        return 1;
    }

    std::cout << "\n============================================\n";
    std::cout << "  HUNTING FOR BELADY'S ANOMALY (FIFO)\n";
    std::cout << "============================================\n\n";

    FaultCount previousFaults = 0;
    bool anomalyFound = false;

    // Test FIFO from 1 frame all the way to 50 frames
    for (FrameCount frames = 1; frames <= 50; frames++) {
        FIFO fifo(frames);
        FaultCount currentFaults = sim.run(&fifo);

        std::cout << "Frames: " << frames << " | Faults: " << currentFaults;

        // If we have more frames, but we get more faults, we caught the anomaly
        if (frames > 1 && currentFaults > previousFaults) {
            std::cout << "ANOMALY DETECTED! Added RAM, but faults increased!";
            anomalyFound = true;
        }
        std::cout << "\n";

        previousFaults = currentFaults;
    }

    if (anomalyFound) {
        std::cout << "\nBelady's Anomaly is real.\n";
    } 
    else {
        std::cout << "\nNo anomaly detected. (Try a different trace pattern).\n";
    }

    return 0;
}