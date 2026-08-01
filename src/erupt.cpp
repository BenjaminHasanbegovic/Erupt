#include "../include/erupt.h"

EInstance::EInstance(bool forceWindowSize,bool forceWindowMode,
                     unsigned int windowWidth, unsigned int windowHeight,
                     const std::string& appName,const unsigned int appVersion,WindowMode windowMode,
                     int windowGroupId, const int32_t  preferredPhysicalDevice) {
    this->vkInstance = std::make_unique<VulkanInstance>(appName,appVersion);
    this->vkDevice = std::make_unique<VulkanDevice>(this->vkInstance->getInstance(),static_cast<uint32_t>(preferredPhysicalDevice));
    this->window = std::make_unique<EWindow>(forceWindowSize,forceWindowMode,windowWidth,windowHeight,appName,windowMode,windowGroupId);

    std::cout << "Erupt instance initialized." << std::endl;
};

EInstance::~EInstance() {
    std::cout << "Erupt instance destroyed!" << std::endl;
};

void EInstance::start() {
    std::cout << "Starting Erupt instance..." << std::endl;

    EStepManager::init();
    this->vkDevice->createDevice();
    this->window->create(this->vkInstance->getInstance());
    if ( !this->vkDevice->hasPresentQueue(this->window->getVulkanSurface())) {
        throw std::runtime_error("The physical device doesn't support present queue!");
    }else {
        std::cout << "The physical device supports present queue." << std::endl;
    }
    this->vkSwapchain = std::make_unique<VulkanSwapchain>(this->vkDevice->getCurrentPhysicalDevice(),this->window->getVulkanSurface());
    this->vkAllocator = std::make_unique<VulkanAllocator>(this->vkInstance->getInstance(),this->vkDevice->getCurrentPhysicalDevice(),this->vkDevice->getLogicalDevice());

    std::cout << "Erupt instance started." << std::endl;
};