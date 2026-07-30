#include "../../../include/vulkanLogic/VMA/vulkanAllocator.h"

#include <iostream>
#include <ostream>

VulkanAllocator::VulkanAllocator(const VkInstance& instance,const VkPhysicalDevice& physicalDevice,const VkDevice& logicalDevice) {
    std::cout << "Initializing Vulkan allocator" << std::endl;
    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
    allocatorCreateInfo.physicalDevice = physicalDevice;
    allocatorCreateInfo.device = logicalDevice;
    allocatorCreateInfo.instance = instance;

    if (vmaCreateAllocator(&allocatorCreateInfo, &this->allocator) != VK_SUCCESS) {
        throw std::runtime_error("Failed to initialize Vulkan Memory Allocator!");
    }else {
        std::cout << "Successfully initialized Vulkan Memory Allocator!" << std::endl;
    }
};

VulkanAllocator::~VulkanAllocator() {
    vmaDestroyAllocator(this->allocator);
    allocator = VK_NULL_HANDLE;
  std::cout << "Destroyed Vulkan allocator!" << std::endl;
};