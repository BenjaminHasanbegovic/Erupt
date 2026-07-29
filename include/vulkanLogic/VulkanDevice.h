#pragma once
#include <vulkan/vulkan.h>
#include <vector>

class Device {
    public:
    Device();
    Device(unsigned int preferredPhysicalDevice);
    ~Device() = default;
     void selectPhysicalDevice();
     void createLogicalDevice();

    //Getters
    [[nodiscard]] std::vector<VkPhysicalDevice> getPhysicalDevices() {
        uint8_t physicalDeviceCount{0};
        vkEnumeratePhysicalDevices()
    };
private:
  VkPhysicalDevice physicalDevice{VK_NULL_HANDLE};
    VkDevice logicalDevice{VK_NULL_HANDLE};
};