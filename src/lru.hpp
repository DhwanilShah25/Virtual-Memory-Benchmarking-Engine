#pragma once

#ifndef LRU_HPP
#define LRU_HPP

#include "policy.hpp"
#include "types.hpp"
#include <list>
#include <unordered_map>
#include <string>

class LRU : public Policy {
    private:
        FrameCount capacity;
        
        // The Doubly Linked List: Front = Most Recently Used, Back = Least Recently Used
        std::list<PageID> memoryList; 
        
        // The Hash Map: Key = PageID, Value = Pointer (iterator) to that page in the Linked List
        std::unordered_map<PageID, std::list<PageID>::iterator> memoryMap;

    public:
        LRU(FrameCount numFrames);
        bool accessPage(PageID pageID) override;
        std::string getName() const override;
};

#endif