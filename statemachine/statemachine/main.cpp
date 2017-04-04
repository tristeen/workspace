//
//  main.cpp
//  statemachine
//
//  Created by tristeen on 3/27/17.
//  Copyright © 2017 Tristeen. All rights reserved.
//

#include <iostream>
#include "statemachine.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    StateMachine<32> sm;
    std::cout << std::boolalpha;
    std::cout << sm.has(1) << std::endl;
    sm.set(1);
    std::cout << sm.has(1) << std::endl;

    std::vector<int> ss = {1, 2, 3};
    std::cout << sm.any(std::move(ss)) << std::endl;
    
    std::cout << "Hello, World!\n";
    return 0;
}
