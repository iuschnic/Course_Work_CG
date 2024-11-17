#ifndef LOADSOLUTION_H
#define LOADSOLUTION_H

#include <string>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "Object.h"

class LoadSolution
{
public:
    LoadSolution() = default;
    ~LoadSolution() = default;

    std::vector<std::shared_ptr<VisibleObject>> load(std::string &filename)
    {
        std::vector<std::shared_ptr<VisibleObject>> objs;
        std::ifstream file(filename);
        std::istream &stream = file;
        if (!stream)
            return nullptr;
        int n_objs;
        stream >> n_objs;
        if (!stream)
            return objs;
        for (int i = 0; i < n_objs; i++)
        {
            auto sphere = load_sphere(stream);
            std::shared_ptr<Object> obj = std::make_shared<Object>(Object(sphere));
            objs.push_back(obj);
        }
        return objs;
    }

private:
    std::shared_ptr<Sphere> load_sphere(std::istream &stream)
    {
        double x, y, z, mass, radius;
        stream >> x >> y >> z;
        if (!stream)
            return nullptr;
        Point center(x, y, z);
        stream >> radius;
        if (!stream)
            return nullptr;
        mass = 4 * M_PI * (radius ** 3);
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(Sphere(mass, radius, center));
        return sphere;
    }
};

#endif // LOADSOLUTION_H

