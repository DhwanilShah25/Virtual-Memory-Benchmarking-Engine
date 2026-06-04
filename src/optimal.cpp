#include "optimal.hpp"

Optimal::Optimal(FrameCount numFrames, const std::vector<PageID>& fullTrace)
    : capacity(numFrames), trace(fullTrace), currentIndex(0) {}

bool Optimal::accessPage(PageID pageID) {
    // Cache Hit
    if (memorySet.find(pageID) != memorySet.end()) {
        currentIndex++; // Advance time
        return false;   // No fault
    }

    // Page Fault! We need a victim.
    if (memorySet.size() >= capacity) {
        PageID victim = 0;
        size_t farthestDistance = 0;

        // Look into the future for every page currently in RAM
        for (PageID pageInMemory : memorySet) {
            size_t distance = 0;
            bool usedAgain = false;

            // Scan the remaining trace to find when it's next used
            for (size_t i = currentIndex + 1; i < trace.size(); i++) {
                distance++;
                if (trace[i] == pageInMemory) {
                    usedAgain = true;
                    break;
                }
            }

            // If a page is not used again, evict it instantly
            if (!usedAgain) {
                victim = pageInMemory;
                break; 
            }

            // Track the page that is used furthest in the future
            if (distance > farthestDistance) {
                farthestDistance = distance;
                victim = pageInMemory;
            }
        }
        
        memorySet.erase(victim); // Evict the optimal victim
    }

    // Insert the new page
    memorySet.insert(pageID);
    currentIndex++; // Advance time
    return true;    // A page fault occurred
}

std::string Optimal::getName() const {
    return "Optimal (Clairvoyant)";
}