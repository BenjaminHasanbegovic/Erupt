#include "../include/erupt.h"
#include "../include/window.h"

#include <iostream>
#include <ostream>

EInstance::EInstance(bool forceWindowSize,bool forceWindowMode,
                     unsigned int windowWidth, unsigned int windowHeight,
                     const std::string& appName,int8_t windowMode,
                     int windowGroupId) {
this->window = std::make_unique<EWindow>(forceWindowSize,forceWindowMode,windowWidth,windowHeight,appName,windowMode,windowGroupId);
};

EInstance::~EInstance() {
  std::cout << "Erupt instance destroyed" << std::endl;
};