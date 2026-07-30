#include "../include/erupt.h"
#include "EStepManager.h"
#include <iostream>

EInstance::EInstance(bool forceWindowSize,bool forceWindowMode,
                     unsigned int windowWidth, unsigned int windowHeight,
                     const std::string& appName,const unsigned int appVersion,WindowMode windowMode,
                     int windowGroupId,int32_t  preferredPhysicalDevice) {

    this->vkInstance = std::make_unique<VulkanInstance>(appName,appVersion);

    if (preferredPhysicalDevice == -1) {
        this->vkDevice = std::make_unique<VulkanDevice>(this->vkInstance->getInstance());
    }else {
        this->vkDevice = std::make_unique<VulkanDevice>(this->vkInstance->getInstance(),static_cast<uint32_t>(preferredPhysicalDevice));
    }


    this->window = std::make_unique<EWindow>(forceWindowSize,forceWindowMode,windowWidth,windowHeight,appName,windowMode,windowGroupId);
};

EInstance::~EInstance() {
    std::cout << "Erupt instance destroyed!" << std::endl;
};

void EInstance::start() {
    EStepManager::init();
    std::cout << "Erupt instance started" << std::endl;
    this->vkDevice->createDevice();
    this->window->create(this->vkInstance->getInstance());
    this->vkAllocator = std::make_unique<VulkanAllocator>(this->vkInstance->getInstance(),this->vkDevice->getCurrentPhysicalDevice(),this->vkDevice->getLogicalDevice());
};

void EInstance::end() const {
delete this;
}