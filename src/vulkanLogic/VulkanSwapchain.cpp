#include "../../include/vulkanLogic/VulkanSwapchain.h"

VulkanSwapchain::VulkanSwapchain(VkPhysicalDevice physicalDevice,VkSurfaceKHR surface) {
    std::cout << "Vulkan swapchain initialized" << std::endl;
}

VulkanSwapchain::~VulkanSwapchain() {
    std::cout << "Vulkan swapchain destroyed!" << std::endl;
}