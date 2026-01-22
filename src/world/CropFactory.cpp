// world/CropFactory.cpp
#include "world/CropFactory.hpp"
#include "world/Crops/Carrot.hpp"
#include "world/Crops/Wheat.hpp"
#include "world/Crops/Corn.hpp"
#include "world/Crops/Potato.hpp"

Crop* CropFactory::create(const std::string& type)
{
    if (type == "carrot") return new Carrot();
    if (type == "wheat")  return new Wheat();
    if (type == "corn")   return new Corn();
    if (type == "potato") return new Potato();
    return nullptr;
}
