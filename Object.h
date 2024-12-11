#ifndef OBJECT_H
#define OBJECT_H

#include "Sphere.h"
#include "Point.h"
#include "VisibleObject.h"
#include <memory>
#include <vector>

class Object: public VisibleObject
{
public:
    Object() = delete;
    Object(const std::shared_ptr<Sphere> &sphere)
    {
        _spheres.push_back(sphere);
        _mass_center = sphere->get_center();
        _mass = sphere->get_mass();
        _speed = Point(0, 0, 0);
        _type = "object";
    }
    ~Object() = default;

    void add_object(const std::shared_ptr<Object> &obj)
    {
        for (const auto &sp : obj->get_spheres())
            _spheres.push_back(sp);
        double mass_new = obj->get_mass();
        Point center_new = obj->get_mass_center();
        Point speed_new = obj->get_speed();

        Point center = (_mass_center * _mass + center_new * mass_new) / (_mass + mass_new);
        _speed = (speed_new * mass_new + _speed * _mass) / (mass_new + _mass);
        _mass_center = center;
        _mass += obj->get_mass();
    }

    std::vector<std::shared_ptr<Sphere>> get_spheres()
    {
        return _spheres;
    }
    Point get_mass_center() const
    {
        return _mass_center;
    }
    Point get_speed() const
    {
        return _speed;
    }
    void add_speed(const Point &speed)
    {
        _speed += speed;
    }
    void set_speed(const Point &speed)
    {
        _speed = speed;
    }
    double get_mass() const
    {
        return _mass;
    }
    void move(const double dx, const double dy, const double dz) override
    {
        for (const auto &sp : _spheres)
            sp->move(dx, dy, dz);
        _mass_center.move(dx, dy, dz);
    }

    void scale(const double kx, const double ky, const double kz) override {}
    void rotate(const double ang_x, const double ang_y, const double ang_z) override {}

    bool check_intersection(const std::shared_ptr<Object> &obj)
    {
        std::vector<std::shared_ptr<Sphere>> spheres_obj = obj->get_spheres();
        for (const auto &sp1: _spheres)
            for (const auto &sp2: spheres_obj)
                if (sp1->check_intersection(*sp2))
                    return true;
        return false;
    }


private:
    std::vector<std::shared_ptr<Sphere>> _spheres;
    Point _mass_center;
    Point _speed;
    double _mass;
    //_speed - скорость тела в конкретный момент времени, является вектором (начало - 000)
};

#endif // OBJECT_H
