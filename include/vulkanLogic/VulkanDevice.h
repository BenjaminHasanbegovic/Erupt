#pragma once
#include <iostream>
#include <ostream>
#include <vector>
#include <map>
#include <vulkan/vulkan.h>

class VulkanDevice {
    public:
    explicit VulkanDevice(const VkInstance &instance);
    VulkanDevice(const VkInstance &instance,uint32_t preferredPhysicalDevice);
    ~VulkanDevice();

     void createDevice();
    //Getters
    [[nodiscard]] std::map<uint32_t, std::pair<std::string, VkPhysicalDevice>> getPhysicalDevicesInfo() const {
        uint32_t physicalDeviceCount{0};
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

        std::map<uint32_t, std::pair<std::string, VkPhysicalDevice>> physicalDevicesInfo;

        std::cout << "Found " << physicalDeviceCount << " physical devices:" << std::endl;
        for (const VkPhysicalDevice phDevice : physicalDevices) {
            VkPhysicalDeviceProperties physicalDeviceProperties;
            vkGetPhysicalDeviceProperties(phDevice, &physicalDeviceProperties);

            physicalDevicesInfo.insert({
                physicalDeviceProperties.deviceID,
                {physicalDeviceProperties.deviceName, phDevice}
            });

            std::cout << "(id:" << physicalDeviceProperties.deviceID << ")"
                      << physicalDeviceProperties.deviceName << ", "
                      << physicalDeviceProperties.deviceType << "\n";
        }
        std::cout << std::flush;

        return physicalDevicesInfo;
    };
    [[nodiscard]] VkPhysicalDevice getCurrentPhysicalDevice() const {return physicalDevice;};
    [[nodiscard]] VkDevice getLogicalDevice() const {return logicalDevice;};
private:
    std::string currentPhysicalDeviceName;
    VkPhysicalDevice preferredPhysicalDevice{VK_NULL_HANDLE};
    VkPhysicalDevice physicalDevice{VK_NULL_HANDLE};
    void selectSuitablePhysicalDevice();

    VkDevice logicalDevice{VK_NULL_HANDLE};
    void createLogicalDevice();

    VkQueue graphicsQueue{VK_NULL_HANDLE};
    uint32_t graphicsQueueFamilyIndex{0};

    VkInstance instance{VK_NULL_HANDLE};
};