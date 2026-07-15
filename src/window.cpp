#include "../include/window.h"
#include <iostream>


EWindow::EWindow(const bool forceWindowSize, const bool forceWindowMode,
                 unsigned int width, unsigned int height,
                 const std::string& title,const int8_t windowMode,
                 const int windowGroupId)
    : groupId(windowGroupId),
      title(title.empty() ?  DEFAULT_WINDOW_TITLE : title),
      size{width <= 0 ? DEFAULT_WINDOW_WIDTH : width, height <= 0 ? DEFAULT_WINDOW_HEIGHT : height},
      forceWindowSize(forceWindowSize),
      mode(windowMode == -1 ? DEFAULT_WINDOW_MODE : static_cast<WindowMode>(windowMode)),
      forceWindowMode(forceWindowMode)

{
    if (this->forceWindowSize) {
        this->mode = WindowMode::WINDOWED;
        this->forceWindowMode = true;
        std::cout << "Window is set to forced size and the window mode is forcefully set to WINDOWED\n";
    };

    std::cout << "Window created successfully,title: " << this->title << std::endl;
}

EWindow::~EWindow() {
  std::cout << "Window is destroyed\n";
}