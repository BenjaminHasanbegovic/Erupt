#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

class VulkanAllocator {
public:
    VulkanAllocator(const VkInstance& instance,const VkPhysicalDevice& physicalDevice,const VkDevice& logicalDevice);
    ~VulkanAllocator();
    private:
    VmaAllocator allocator{VK_NULL_HANDLE};
};