#ifndef LOADSOLUTION_H
#define LOADSOLUTION_H

#include <string>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <random>
#include <time.h>
#include "Object.h"

#define ep 0.5

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
            auto obj = load_obj(stream);
            if (!obj)
                return {};
            objs.push_back(obj);
        }
        return objs;
    }

    std::vector<std::shared_ptr<VisibleObject>> load(const int &n_objs, const double &min_r, const double &max_r,
                                                    const double &min_s, const double &max_s)
    {
        srand(time(NULL));
        std::vector<std::shared_ptr<VisibleObject>> objs;
        std::vector<double> r_spheres;
        if (n_objs <= 0 || min_r >= max_r || min_s >= max_s || min_s < 0 || min_r < 0)
            return {};
        for (int i = 0; i < n_objs; i++)
        {
            double r = rand_double(min_r, max_r); //радиус
            if (r < ep)
                r = ep;
            double m = 4 * M_PI * pow(r, 3) * 0.0001; //масса
            double s = rand_double(min_s, max_s); //скорость
            double x, y, z; //координаты центра
            x = rand_double(-100, 100);
            y = rand_double(-100, 100);
            z = rand_double(-100, 100);
            Point c = Point(x, y, z);
            //Несколько раз производится попытка сгенерировать сферу, не пересекающую и не лежащую внутри других
            int flag1 = 1;
            for (int j = 0; j < 10; j++)
            {
                int flag2 = 1;
                int t = 0;
                //Проверяется относительное положение новой сферы относительно всех остальных
                for (const auto &obj: objs)
                {
                    auto object = std::static_pointer_cast<Object>(obj);
                    Point c_obj = object->get_mass_center();
                    double dist = pow(pow(c_obj.get_x() - x, 2) + pow(c_obj.get_y() - y, 2) + pow(c_obj.get_z() - z, 2), 0.5);
                    //если новая сфера пересекает какую то другую, перегенерируются координаты новой сферы
                    if (dist < r + r_spheres[t])
                    {
                        x = rand_double(-100, 100);
                        y = rand_double(-100, 100);
                        z = rand_double(-100, 100);
                        c = Point(x, y, z);
                        flag2 = 0;
                        break;
                    }
                }
                //Если сфера не пересекается с другими сферами
                if (flag2)
                {
                    std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(Sphere(m, r, c));
                    std::shared_ptr<Object> obj = std::make_shared<Object>(Object(sphere));
                    Point speed;
                    double a, b; //углы для рандомной генерации положения вектора скорости в сферической системе координат
                    a = rand_double(0, 360) * M_PI / 180;
                    b = rand_double(0, 360) * M_PI / 180;
                    speed.set_x(s * cos(a) * cos(b));
                    speed.set_y(s * sin(a) * cos(b));
                    speed.set_z(s * sin(b));
                    obj->set_speed(speed);
                    objs.push_back(obj);
                    flag1 = 0;
                    r_spheres.push_back(r);
                    break;
                }
            }
            //если все попытки генерации сферы были неудачными, заносим как есть
            if (flag1)
            {
                std::shared_ptr<Sphere> sphere = std::make_shared<Sphere>(Sphere(m, r, c));
                std::shared_ptr<Object> obj = std::make_shared<Object>(Object(sphere));
                Point speed;
                double a, b; //углы для рандомной генерации положения вектора скорости в сферической системе координат
                a = rand_double(0, 360) * M_PI / 180;
                b = rand_double(0, 360) * M_PI / 180;
                speed.set_x(r * cos(a) * cos(b));
                speed.set_y(r * sin(a) * cos(b));
                speed.set_z(r * sin(b));
                obj->set_speed(speed);
                objs.push_back(obj);
                flag1 = 0;
                r_spheres.push_back(r);
            }
            else
            {
                flag1 = 1;
            }
        }
        return objs;
    }

private:
    std::shared_ptr<Object> load_obj(std::istream &stream)
    {
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
        return obj;
    }

    double rand_double(double dmin, double dmax)
    {
        double f = (double) rand() / RAND_MAX;
        return dmin + f * (dmax - dmin);
    }
};

#endif // LOADSOLUTION_H

