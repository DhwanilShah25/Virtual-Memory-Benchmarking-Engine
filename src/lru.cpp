#include "lru.hpp"
#include "types.hpp"

LRU::LRU(FrameCount numFrames) : capacity(numFrames) {}

bool LRU::accessPage(PageID pageID) {
    // Check if the page is already in memory (Page Hit)
    if (memoryMap.find(pageID) != memoryMap.end()) {
        
        // We pluck it from its current position and move it to the front
        memoryList.erase(memoryMap[pageID]);
        memoryList.push_front(pageID);
        
        // Update the map to point to the new location at the front
        memoryMap[pageID] = memoryList.begin();
        
        return false; // No page fault
    }

    // Page Fault! 
    if (memoryMap.size() >= capacity) {
        // Evict the Least Recently Used page (which sits at the back of the list)
        PageID leastRecentlyUsed = memoryList.back();
        memoryList.pop_back();              
        memoryMap.erase(leastRecentlyUsed);
    }

    // Insert the new page at the front
    memoryList.push_front(pageID);
    memoryMap[pageID] = memoryList.begin();

    return true; // page fault 
}

std::string LRU::getName() const {
    return "LRU (O(1) Optimized)";
}