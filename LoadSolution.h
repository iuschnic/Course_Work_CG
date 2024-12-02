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
            return {};
        int n_objs;
        stream >> n_objs;
        if (!stream)
            return objs;
        for (int i = 0; i < n_objs; i++)
        {
            //auto sphere = load_sphere(stream);
            //std::shared_ptr<Object> obj = std::make_shared<Object>(Object(sphere));
            auto obj = load_obj(stream);
            objs.push_back(obj);
        }
        //std::cout << "loaded all objs" << std::endl;
        return objs;
    }

private:
    std::shared_ptr<Object> load_obj(std::istream &stream)
    {
        //std::cout << "loading" << std::endl;
        double x, y, z, vx, vy, vz, mass, radius;
        stream >> x >> y >> z;
        if (!stream)
            return nullptr;
        Point center(x, y, z);
        stream >> vx >> vy >> vz;
        if (!stream)
            return nullptr;
        Point speed(vx, vy, vz);
        stream >> radius;
        if (!stream)
            return nullptr;
        mass = 4 * M_PI * pow(radius, 3) * 0.0001;
        radius = fabs(radius);
        std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(Sphere(mass, radius, center));
        std::shared_ptr<Object> obj = std::make_shared<Object>(Object(sphere));
        obj->set_speed(speed);
        //std::cout << "loaded" << std::endl;
        return obj;
    }
};

#endif // LOADSOLUTION_H

