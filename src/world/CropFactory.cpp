#include "world/CropFactory.hpp"

Crop* CropFactory::create(const std::string& type)
{
    return CropRegistry::create(type);
}
