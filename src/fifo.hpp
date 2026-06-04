#pragma once

#ifndef FIFO_HPP
#define FIFO_HPP

#include "policy.hpp"
#include <queue>
#include <unordered_set>
#include <string>

class FIFO : public Policy {
    private:
        FrameCount capacity; // How many physical frames (RAM) we have
        std::queue<PageID> memoryQueue; // Tracks which page was loaded first
        std::unordered_set<PageID> memorySet; // O(1) lookup to check if page is in memory

    public:
        // Constructor requires the frame size
        FIFO(FrameCount numFrames);

        // Overriding the pure virtual functions from Policy
        bool accessPage(PageID pageID) override;
        std::string getName() const override;
};

#endif