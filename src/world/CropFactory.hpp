// world/CropFactory.hpp
#pragma once
#include "world/Crop.hpp"
#include "world/CropRegistry.hpp"
#include <string>

class CropFactory {
public:
    static Crop* create(const std::string& type);
};
