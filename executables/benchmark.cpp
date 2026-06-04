#include "../src/simulator.hpp"
#include "../src/fifo.hpp"
#include "../src/lru.hpp"
#include "../src/clock.hpp"
#include "../src/optimal.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./benchmark <path_to_trace> <path_to_output_csv>\n";
        return 1;
    }

    std::string traceFile = argv[1];
    std::string outputFile = argv[2];
    Simulator sim;
    
    if (!sim.loadTrace(traceFile)) return 1;

    std::ofstream csv(outputFile);
    csv << "FrameSize,FIFO,LRU,Clock,Optimal\n"; 

    std::cout << "Blasting Engine from 1 to 100 Frames...\n";

    // Grab the trace to feed to our clairvoyant algorithm
    const std::vector<PageID>& trace = sim.getTrace();

    for (FrameCount frames = 1; frames <= 100; frames++) {
        FIFO fifo(frames);
        LRU lru(frames);
        Clock clock(frames);
        Optimal optimal(frames, trace);

        FaultCount f_faults = sim.run(&fifo);
        FaultCount l_faults = sim.run(&lru);
        FaultCount c_faults = sim.run(&clock);
        FaultCount o_faults = sim.run(&optimal); 

        // Output all four metrics
        csv << frames << "," << f_faults << "," << l_faults << "," 
            << c_faults << "," << o_faults << "\n";
    }

    csv.close();
    std::cout << "Benchmarking complete. Data exported to " << outputFile << "\n";

    return 0;
}