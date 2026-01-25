#pragma once
#include <functional>
#include <string>
#include <vector>

class Crop;

struct CropInfo
{
    std::string id;
    std::string label;
    std::string iconPath;
    int sellValue = 0;
    std::function<Crop *()> create;
};

class CropRegistry
{
public:
    static const std::vector<CropInfo> &all();
    static Crop *create(const std::string &id);
    static const CropInfo *find(const std::string &id);
};
