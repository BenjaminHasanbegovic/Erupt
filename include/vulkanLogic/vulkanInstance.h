#pragma once
#include <vulkan/vulkan.h>
#include <string>


class VulkanInstance{
 public:
    VulkanInstance(const std::string& appName,unsigned int appVersion);
    ~VulkanInstance();

    //Getters
    [[nodiscard]] VkInstance getInstance() const {return this->instance;};
 private:
    VkInstance instance{VK_NULL_HANDLE};

};