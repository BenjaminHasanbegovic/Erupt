#include "../include/EStepManager.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <thread>

static constexpr size_t maxBranching = 5;

EStep::EStep(const unsigned int id, const EStepType type, std::function<bool()> function, const bool dependent):
id(id), type(type), dependent(dependent), func(std::move(function)) {
    std::cout << "Step added: " << this->id << std::endl;
}

bool EStep::start() {
    std::cout << this->id <<". step started!" << std::endl;
    if (func) {
        this->finishedSuccessfully = func();
        return this->finishedSuccessfully;
    }else {
       std::cout << "Step wasn't given a function!" << std::endl;
        return false;
    }
}

EStep::~EStep() {
    std::cout << "Step finished ";
    if (this->finishedSuccessfully) {
        std::cout << "successfully!";
    } else {
        std::cout << "unsuccessfully";
    }
    std::cout << std::endl;
}

void EStepManager::init() {
    std::cout << "Initializing step manager!\n";

    availableThreads = std::thread::hardware_concurrency();
    std::cout << "Available number of threads is: " << availableThreads << "\n";

    if (availableThreads == 0) {
       EStepManager::multithread = false;
        std::cout << "Not enough threads available, multithreading features are disabled \n";
    }else {
        EStepManager::multithread = true;
            std::cout << "Multithreading features are enabled \n";

    }

    std::cout << "Step manager initialized!" << std::endl;
}

void EStepManager::add(EStepType type, std::function<bool()> function, bool dependent) {
    static unsigned int lastStepId{0};
    if (lastStepId == 0) steps.reserve(maxBranching);

    int trueType = static_cast<int>(type) + static_cast<int>(dependent);
    steps.emplace_back(std::make_unique<EStep>(lastStepId++, static_cast<EStepType>(trueType), std::move(function), dependent));
}

void EStepManager::runAll() {
    std::erase_if(steps, [](const std::unique_ptr<EStep>& step) {
        step->start();
        return true;
    });
}