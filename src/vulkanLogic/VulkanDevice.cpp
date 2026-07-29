#include "../../include/vulkanLogic/VulkanDevice.h"

VulkanDevice::VulkanDevice(const VkInstance &instance):instance(instance) {
    std::cout << "Vulkan device initialized." << std::endl;
};

VulkanDevice::VulkanDevice(const VkInstance &instance, const uint32_t preferredPhysicalDevice):instance(instance) {
    std::cout << "Vulkan device initialized." << std::endl;
    const auto physicalDevicesMap = getPhysicalDevicesInfo();

    if (const auto physicalDeviceLocation = physicalDevicesMap.find(preferredPhysicalDevice);
        physicalDeviceLocation != physicalDevicesMap.end()) {
        this->currentPhysicalDeviceName = physicalDeviceLocation->second.first;
        this->preferredPhysicalDevice= physicalDeviceLocation->second.second; // <--- Here is your device!

        std::cout << "Successfully retrieved the requested physical device : " <<  this->currentPhysicalDeviceName << std::endl;
    } else {
        std::cerr << "Device ID not found in the map!" << std::endl;
    }
};

VulkanDevice::~VulkanDevice() {
    vkDestroyDevice(logicalDevice, nullptr);
    logicalDevice = VK_NULL_HANDLE;
    std::cout << "Vulkan device destroyed!" << std::endl;
}

void VulkanDevice::createDevice() {
    std::cout << "Creating Vulkan device!" << std::endl;
    selectSuitablePhysicalDevice();
    createLogicalDevice();
    std::cout << "Created vulkan device." << std::endl;
}

void VulkanDevice::selectSuitablePhysicalDevice() {
    if (preferredPhysicalDevice != VK_NULL_HANDLE) {
        physicalDevice = preferredPhysicalDevice;
        std::cout << "Selected suitable physical device: " << currentPhysicalDeviceName << std::endl;
        return;
    }

    uint32_t physicalDeviceCount{0};
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

    if (physicalDeviceCount == 0) {
        std::cerr << "Failed to find GPUs with Vulkan support!" << std::endl;
        return;
    }

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

    unsigned int highestScore{0};
    VkPhysicalDevice bestDevice{VK_NULL_HANDLE};

    std::cout << "Searching for suitable physical device: "<< std::endl;
    for (const auto& currentDevice : physicalDevices) {
        unsigned int currentScore{0};

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(currentDevice, &deviceProperties);
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(currentDevice, &deviceFeatures);
        std::cout << "Checking " << "(" << deviceProperties.deviceID << ")" << deviceProperties.deviceName << ":" << std::flush;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(currentDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(currentDevice, &queueFamilyCount, queueFamilies.data());
        bool hasGraphicsQueue = false;

        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                hasGraphicsQueue = true;
                std::cout << "found a graphics queue, "<< std::flush;
                break;
            }
        }
        if (!hasGraphicsQueue || !deviceFeatures.geometryShader) {
            std::cout << "missing geometry shader feature or there is no graphics queue, "<< std::flush;
            continue;
        }
        std::cout << "found the geometry shader feature, "<< std::flush;

        currentScore += deviceProperties.limits.maxImageDimension2D;
        std::cout << "the max texture size is: " << deviceProperties.limits.maxImageDimension2D  << ", "<< std::flush;

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            currentScore += 1000;
            std::cout << "the GPU is a discrete device" << std::flush;
        }else {
            std::cout << "the GPU is an integrated device" << std::flush;
        }
        std::cout << " Check for " << deviceProperties.deviceName << "is over. Final score is:" << currentScore << std::endl;
        if (currentScore > highestScore) {
            highestScore = currentScore;
            bestDevice = currentDevice;
            currentPhysicalDeviceName = deviceProperties.deviceName;
        }
    }

    if (bestDevice == VK_NULL_HANDLE) {
        std::cerr << "Failed to find a suitable GPU that meets the 3D rendering requirements!" << std::endl;
    }else {
        physicalDevice = bestDevice;
        std::cout << "Selected suitable physical device: " << currentPhysicalDeviceName << std::endl;
    }
};

void VulkanDevice::createLogicalDevice() {
    if (physicalDevice == VK_NULL_HANDLE) {
        std::cerr << "Cannot create logical device: No physical device selected!" << std::endl;
        return;
    }
    std::cout << "Creating logical device..." << std::endl;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    bool foundGraphicsQueue = false;
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueFamilyIndex = i;
            foundGraphicsQueue = true;
            break;
        }
    }
    if (!foundGraphicsQueue) {
        std::cerr << "Critical Error: Selected GPU does not have a graphics queue!" << std::endl;
        return;
    }

    constexpr float queuePriority{1.0f};
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.geometryShader = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = nullptr;
    createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        std::cerr << "Failed to create logical device!" << std::endl;
        return;
    }
    std::cout << "Logical device successfully created!" << std::endl;

    vkGetDeviceQueue(logicalDevice, graphicsQueueFamilyIndex, 0, &graphicsQueue);
    std::cout << "Graphics queue handle successfully retrieved!" << std::endl;
}
