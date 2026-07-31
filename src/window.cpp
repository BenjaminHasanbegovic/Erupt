#include "../include/window.h"

EWindow::EWindow(const bool forceWindowSize, const bool forceWindowMode, const unsigned int width, const unsigned int height,
                 const std::string& title,const WindowMode windowMode,
                 const int windowGroupId)
    : groupId(windowGroupId),
      title(title.empty() ?  DEFAULT_WINDOW_TITLE : title),
      size{width <= 0 ? DEFAULT_WINDOW_WIDTH : width, height <= 0 ? DEFAULT_WINDOW_HEIGHT : height},
      forceWindowSize(forceWindowSize),
      mode(windowMode),
      forceWindowMode(forceWindowMode) {
    if (this->forceWindowSize) {
        this->mode = WindowMode::WINDOWED;
        this->forceWindowMode = true;
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        std::cout << "Window is set to forced size\nThe window mode is forcefully set to WINDOWED\n";
    };
}

EWindow::~EWindow() {
    vkDestroySurfaceKHR(surfaceInstance,surface,nullptr);
    std::cout << "Surface is destroyed!" << std::endl;

    glfwDestroyWindow(window);
    window = nullptr;
    std::cout << "Window is destroyed!" << std::endl;

    glfwTerminate();
    std::cout << "Terminated GLFW!" << std::endl;
}

void EWindow::create(const VkInstance& vkInstance) {
    surfaceInstance = vkInstance;
    window = glfwCreateWindow(static_cast<int>(this->size[0]), static_cast<int>(this->size[1]),
        title.c_str(), nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }else {
        std::cout << "Created glfw window.\n" << "Window initialized,title: " << this->title << "\nCreating Vulkan window surface!" <<std::endl;

        if (glfwCreateWindowSurface(surfaceInstance,window,nullptr,&surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan window surface!");
        }else {
            std::cout << "Created Vulkan window surface!" << std::endl;
        }
    }
};