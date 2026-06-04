#pragma once

#ifndef CLOCK_HPP
#define CLOCK_HPP

#include "policy.hpp"
#include "types.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class Clock : public Policy {
    private:
        FrameCount capacity;
        size_t clockHand; // The pointer sweeping through our circular array

        // Simulating physical memory frames and their hardware reference bits
        std::vector<PageID> frames;
        std::vector<bool> referenceBits;

        // O(1) lookup to find exactly which index a page is sitting at
        std::unordered_map<PageID, size_t> pageMap;

    public:
        Clock(FrameCount numFrames);
        bool accessPage(PageID pageID) override;
        std::string getName() const override;
};

#endif