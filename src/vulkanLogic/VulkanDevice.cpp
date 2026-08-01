#include "../../include/vulkanLogic/VulkanDevice.h"

VulkanDevice::VulkanDevice(const VkInstance &instance, const uint32_t preferredPhysicalDevice):instance(instance) {
    std::cout << "Initializing Vulkan device ..." << std::endl;

    if (preferredPhysicalDevice != -1) {
        std::cout << "Locating the requested physical device." << std::endl;

        const auto physicalDevicesMap = getPhysicalDevicesInfo();
        const auto physicalDeviceLocation = physicalDevicesMap.find(preferredPhysicalDevice);
        if ( physicalDeviceLocation != physicalDevicesMap.end()){}
        //Execrate the needed data from the provided map
        this->currentPhysicalDeviceName = physicalDeviceLocation->second.first;
        this->preferredPhysicalDevice= physicalDeviceLocation->second.second;

        std::cout << "Successfully located the requested physical device : " <<  this->currentPhysicalDeviceName << std::endl;
    }

    std::cout << "Vulkan device initialized." << std::endl;
};

VulkanDevice::~VulkanDevice() {
    vkDestroyDevice(logicalDevice, nullptr);
    logicalDevice = VK_NULL_HANDLE;
    std::cout << "Vulkan device destroyed!" << std::endl;
}

bool VulkanDevice::hasPresentQueue(const VkSurfaceKHR surface) {
    std::cout << "Checking for physical device present queue support.";

    VkBool32 presentSupport{false};
    for (uint32_t i = 0; i < physicalDeviceCount; i++) {
       vkGetPhysicalDeviceSurfaceSupportKHR(currentPhysicalDevice, i, surface, &presentSupport);
        if (presentSupport) {
           presentQueueFamilyIndex = i;
            break;
       }
   }
    return presentSupport;
};

void VulkanDevice::createDevice() {
    std::cout << "Creating Vulkan device ..." << std::endl;
    selectSuitablePhysicalDevice();
    createLogicalDevice();
    std::cout << "Created vulkan device." << std::endl;
}

void VulkanDevice::selectSuitablePhysicalDevice() {
    //Use the preferred physical device
    if (preferredPhysicalDevice != VK_NULL_HANDLE) {
        currentPhysicalDevice = preferredPhysicalDevice;
        std::cout << "Selected suitable physical device: " << currentPhysicalDeviceName << std::endl;
        return;
    }
//Search for a suitable physical device
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    if (physicalDeviceCount == 0) {
        throw std::runtime_error("Failed to find physical devices with Vulkan support!");
        return;
    }
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());


    //Evaluate/score physical devices
    unsigned int highestScore{0};
    VkPhysicalDevice bestDevice{VK_NULL_HANDLE};
    std::cout << "Searching for suitable physical device: "<< std::endl;
    for (const auto& currentDevice : physicalDevices) {
        unsigned int currentScore{0};

        //Get physical device properties
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(currentDevice, &deviceProperties);
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(currentDevice, &deviceFeatures);
        std::cout << "Checking " << "(" << deviceProperties.deviceID << ")" << deviceProperties.deviceName << ":" << std::flush;

        //Get graphics queue support
        uint32_t queueFamilyCount{0};
        vkGetPhysicalDeviceQueueFamilyProperties(currentDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(currentDevice, &queueFamilyCount, queueFamilyProperties.data());
        bool hasGraphicsQueue = false;
        int i{0};

        for (const auto& queueFamily : queueFamilyProperties) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsQueueFamilyIndex = i;
                hasGraphicsQueue = true;
                std::cout << "found a graphics queue, "<< std::flush;
            }
            ++i;
        }
        if (!hasGraphicsQueue || !deviceFeatures.geometryShader) {
           currentScore = 0;
           throw std::runtime_error("Missing geometry shader feature or there is no graphics queue!");
        }else {
            std::cout << "found the geometry shader feature, "<< std::flush;
        }

        currentScore += deviceProperties.limits.maxImageDimension2D;
        std::cout << "the max texture size is: " << deviceProperties.limits.maxImageDimension2D  << ", "<< std::flush;

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            currentScore += 1000;
            std::cout << "the GPU is a discrete device" << std::flush;
        }else {
            std::cout << "the GPU is an integrated device" << std::flush;
        }
        std::cout << " Check for: " << "\"" <<deviceProperties.deviceName << "\" " << "is over. Final score is:" << currentScore << std::endl;

        //Final scoring
        if (currentScore > highestScore) {
            highestScore = currentScore;
            bestDevice = currentDevice;
            currentPhysicalDeviceName = deviceProperties.deviceName;
        }
    }

    if (bestDevice == VK_NULL_HANDLE) {
       throw std::runtime_error("Failed to find a suitable physical device!");
    }else {
        currentPhysicalDevice = bestDevice;
        std::cout << "Selected suitable physical device: " << "\"" <<currentPhysicalDeviceName << "\"" << std::endl;
    }
};

void VulkanDevice::createLogicalDevice() {
    if (currentPhysicalDevice == VK_NULL_HANDLE) {
        std::cerr << "Cannot create logical device: No physical device selected!" << std::endl;
        return;
    }
    std::cout << "Creating logical device..." << std::endl;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(currentPhysicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(currentPhysicalDevice, &queueFamilyCount, queueFamilies.data());

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

    if (vkCreateDevice(currentPhysicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        std::cerr << "Failed to create logical device!" << std::endl;
        return;
    }
    std::cout << "Logical device successfully created!" << std::endl;

    vkGetDeviceQueue(logicalDevice, graphicsQueueFamilyIndex, 0, &graphicsQueue);
    std::cout << "Graphics queue handle successfully retrieved!" << std::endl;
}
