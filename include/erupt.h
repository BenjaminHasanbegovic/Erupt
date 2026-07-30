#pragma once

#include "EStepManager.h"
#include "vulkanLogic/VulkanDevice.h"
#include "vulkanLogic/vulkanInstance.h"
#include "vulkanLogic/VMA/vulkanAllocator.h"
#include "window.h"
class EWindow;
class VulkanInstance;
class VulkanDevice;
class VulkanAllocator;

#include <memory>
#include <string>

struct EStepManager;

class EInstance {
    public:
    EInstance(bool forceWindowSize,bool forceWindowMode,
        unsigned int windowWidth = 0, unsigned int windowHeight = 0,
         const std::string& appName = "",unsigned int appVersion = 0,WindowMode windowMode = DEFAULT_WINDOW_MODE,
         int windowGroupId = -1,int32_t preferredPhysicalDevice = -1); //If the value is 0,-1 or empty => default value
    ~EInstance();

    void start();
    void end()const;

private:
    std::unique_ptr<VulkanInstance> vkInstance{nullptr};
    std::unique_ptr<VulkanDevice> vkDevice{nullptr};
    std::unique_ptr<VulkanAllocator> vkAllocator{nullptr};
    std::unique_ptr<EWindow> window{nullptr};
    EStepManager* stepManager{nullptr};
};
