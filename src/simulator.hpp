#pragma once

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "policy.hpp"
#include "types.hpp"
#include <vector>
#include <string>

class Simulator {
private:
    std::vector<PageID> memoryTrace;

public:
    // Reads the Python-generated text file into the memoryTrace vector
    bool loadTrace(const std::string& filepath);

    // Returns vector of Occurrence of certain Page for Optimal Algorithm
    const std::vector<PageID>& getTrace() const { return memoryTrace; }

    // Runs the loaded trace against a specific algorithm and returns total page faults
    FaultCount run(Policy* algorithm);
};

#endif