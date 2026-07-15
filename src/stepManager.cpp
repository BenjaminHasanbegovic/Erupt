#include <utility>
#include <iostream>
#include "../include/stepManager.h"

EStep::EStep(const StepType type, std::string description) :
    type(type),
    description(std::move(description))
{
    while (EStepManager::steps.contains(EStepManager::pNextStepId)) {
        EStepManager::pNextStepId++;
    }

    this->id = EStepManager::pNextStepId++;
    EStepManager::lastStepId = this->id;

    if (!EStepManager::startingStepId.has_value()) {
        EStepManager::startingStepId = this->id;
    }
}

EStep::~EStep() {
    std::cout << "Step " << static_cast<int>(this->id) << " finished.\n";
    EStepManager::steps.erase(this->id);

    if (EStepManager::startingStepId.has_value() && EStepManager::startingStepId.value() == this->id) {
        EStepManager::pNextStepId = this->id;

        if (EStepManager::steps.contains(EStepManager::pNextStepId) && !EStepManager::steps.empty()) EStepManager::pNextStepId = EStepManager::steps.rbegin()->first + 1;

        EStepManager::startingStepId.reset();
    }
}

void EStepManager::addStep(const StepType type, const std::string &description) {
    auto* pNewStep = new EStep(type, description);
    steps.insert({pNewStep->getId(), pNewStep});
}