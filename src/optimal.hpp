#pragma once

#ifndef OPTIMAL_HPP
#define OPTIMAL_HPP

#include "policy.hpp"
#include "types.hpp"
#include <vector>
#include <unordered_set>
#include <string>

class Optimal : public Policy {
    private:
        FrameCount capacity;
        const std::vector<PageID>& trace; // A reference to the future
        size_t currentIndex;              // Our current position in time
        std::unordered_set<PageID> memorySet; 

    public:
        Optimal(FrameCount numFrames, const std::vector<PageID>& fullTrace);
        bool accessPage(PageID pageID) override;
        std::string getName() const override;
};

#endif