#include "clock.hpp"

Clock::Clock(FrameCount numFrames) : capacity(numFrames), clockHand(0) {}

bool Clock::accessPage(PageID pageID) {
    // Check if the page is already in memory (Page Hit)
    auto it = pageMap.find(pageID);
    if (it != pageMap.end()) {
        // flip the bit to 1
        referenceBits[it->second] = true;
        return false; // No page fault
    }

    // Page Fault! We need to bring it into memory. If memory is full, we must find a victim.
    if (frames.size() == capacity) {
        while (true) {
            // Check the frame the clock hand is currently pointing at
            if (referenceBits[clockHand] == true) {
                // Flip bit to 0, move hand forward
                referenceBits[clockHand] = false;
                clockHand = (clockHand + 1) % capacity;
            } 
            else {
                // Found a victim
                PageID victim = frames[clockHand];
                pageMap.erase(victim);

                // Insert the new page at the victim's location
                frames[clockHand] = pageID;
                referenceBits[clockHand] = true;
                pageMap[pageID] = clockHand;

                // Move hand forward for the next future replacement
                clockHand = (clockHand + 1) % capacity;
                break; // Exit the loop
            }
        }
    } 
    else {
        // Memory is not full yet, just append to our frames
        frames.push_back(pageID);
        referenceBits.push_back(true); // New pages start with bit = 1
        pageMap[pageID] = frames.size() - 1;
    }

    return true; // A page fault occurred
}

std::string Clock::getName() const {
    return "Clock (Hardware Approximation)";
}