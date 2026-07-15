#pragma once
#include <memory>
#include <string>
class EWindow;
struct  EStepManager;
class EInstance {
    public:
    EInstance(bool forceWindowSize,bool forceWindowMode,
        unsigned int windowWidth = 0, unsigned int windowHeight = 0,
         const std::string& appName = "",int8_t windowMode = 0,
         int windowGroupId = -1); //If value 0,-1 or empty => default value
    ~EInstance();
private:
std::unique_ptr<EWindow> window{nullptr};

    static EStepManager* stepManager;
};
