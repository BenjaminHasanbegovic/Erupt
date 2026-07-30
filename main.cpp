#include <chrono>
#include <thread>
#include <iostream>
#include "include/erupt.h"
int main() {
    std::cout << "Erupt test started!" << std::endl;
    static const auto start = std::chrono::high_resolution_clock::now();
    // EInstance* app = new EInstance(false,false,1920,
    //     1080,"Test application",1,
    //     WindowMode::BORDERLESS,-1,18086);

    EInstance* app = new EInstance(false,false,1920,
    1080,"Test application");
        app->start();
       app->end();
    static const std::chrono::duration<double,std::milli> testTime = std::chrono::high_resolution_clock::now() - start;
    std::cout << "Erupt test ended! \n Execution time:" << testTime <<std::endl;
    return 0;
};