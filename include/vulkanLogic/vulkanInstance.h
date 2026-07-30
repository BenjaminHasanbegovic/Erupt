#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.h>
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>

class VulkanInstance{
 public:
    VulkanInstance(const std::string& appName,unsigned int appVersion);
    ~VulkanInstance();

    //Getters
    [[nodiscard]] VkInstance getInstance() const {return this->instance;};
 private:
    VkInstance instance{VK_NULL_HANDLE};

};