#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <string>

#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080
#define DEFAULT_WINDOW_TITLE "Erupt app"
#define DEFAULT_WINDOW_MODE WindowMode::WINDOWED

enum class WindowMode : int8_t {
    OVERLAY = 0, //The window will be overlaid with the users desktop
  FULLSCREEN = 1, //Window takes 100% of the screen
    WINDOWED = 2, //The size of the window is variable and the title bar is showing
    BORDERLESS = 3 //The size of the window is variable, but the title bar isn't shown
};

class EWindow {
    public:
    explicit EWindow(bool forceWindowSize,bool forceWindowMode,
        unsigned int width = DEFAULT_WINDOW_WIDTH, unsigned int height = DEFAULT_WINDOW_HEIGHT,
       const std::string& title = DEFAULT_WINDOW_TITLE,WindowMode windowMode = DEFAULT_WINDOW_MODE,
         int windowGroupId = 0);
    ~EWindow();

    void create(const VkInstance& vkInstance);

private:
    int groupId{0};
    std::string title{DEFAULT_WINDOW_TITLE};

    GLFWwindow* window{nullptr};
    VkSurfaceKHR surface;

    unsigned int size[2]{DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT};
    bool forceWindowSize{false};
    WindowMode mode{DEFAULT_WINDOW_MODE};
    bool forceWindowMode{false};
};
