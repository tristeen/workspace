//
//  statemachine.hpp
//  statemachine
//
//  Created by tristeen on 3/27/17.
//  Copyright © 2017 Tristeen. All rights reserved.
//

#ifndef statemachine_hpp
#define statemachine_hpp

#include <bitset>
#include <vector>

template <size_t MAX>
class StateMachine
{
public:
    StateMachine()
    {
    }
    ~StateMachine()
    {
    }
    bool set(int state)
    {
        if (state >= MAX)
            return false;
        states.set(state, true);
        return true;
    }
    bool reset(int state)
    {
        if (state >= MAX)
            return false;
        states.reset(state);
        return true;
    }
    bool flip(int state)
    {
        if (state >= MAX)
            return false;
        states.flip(state);
        return true;
    }
    bool has(int state)
    {
        if (state >= MAX)
            return false;
        return states[state];
    }
    bool any(std::vector<int> &&sv)
    {
        for (auto i: sv) {
            if (has(i))
                return true;
        }
        return false;
    }
    bool all(std::vector<int> &&sv)
    {
        for (auto i: sv) {
            if (!has(i))
                return false;
        }
        return true;
    } 
private:
    std::bitset<MAX> states;
};


#endif /* statemachine_hpp */
