#include "../../include/vulkanLogic/vulkanInstance.h"



VulkanInstance::VulkanInstance(const std::string& appName, const unsigned int appVersion) {
    std::cout << "Creating Vulkan instance for " << appName << std::endl;

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName.c_str();
    appInfo.applicationVersion = appVersion;
    appInfo.pEngineName = "Erupt Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize glfw");
    }else {
        std::cout << "glfw initialized!" << std::endl;
        glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    }
    uint32_t glfwExtensionCount{0};
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.pNext = nullptr;

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        std::cerr << "Failed to create Vulkan instance!\n";
    } else {
        std::cout << "Created Vulkan instance of " << appName << std::endl;
    }
}

VulkanInstance::~VulkanInstance() {
    if (instance != VK_NULL_HANDLE) {
        vkDestroyInstance(instance, nullptr);
        instance = VK_NULL_HANDLE;
        std::cout << "Destroyed Vulkan instance!" << std::endl;
    }
}