#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "BaseLight.h"
/*
class DirectedLight: public BaseLight
{
public:
    DirectedLight() : _center(Point(0, 0, 0)), _direction(Point(0, 0, 1)){}
    DirectedLight(const Point &center) : _center(center), _direction(Point(0, 0, 1)) {}
    DirectedLight(const Point &center, const Point &direction) : _center(center), _direction(direction) {}
    DirectedLight(DirectedLight &light) : _center(light.get_center()), _direction(light.get_direction())
    {
        _id = light.get_id();
    }
    DirectedLight(DirectedLight &&light) : _center(light.get_center()), _direction(light.get_direction())
    {
        _id = light.get_id();
    }
    ~DirectedLight() = default;

    void move(const double dx, const double dy, const double dz) override {_center.move(dx, dy, dz);}
    void rotate(const double ang_x, const double ang_y, const double ang_z) override
    {
        Point c(0, 0, 0);
        _direction.rotate(c, ang_x, ang_y, ang_z);
    }
    void scale(const double kx, const double ky, const double kz) override {}

private:
    Point _center;
    Point _direction;
};*/

class PointLight: public BaseLight
{
public:
    PointLight() : _center(Point(0, 0, 0)){
        set_type("light");
    }
    PointLight(const Point &center) : _center(center) {
        set_type("light");
    }
    /*PointLight(const Point &&center) : _center(center) {
        set_type("light");
    }*/
    PointLight(PointLight &light) : _center(light.get_center())
    {
        set_type("light");
        _id = light.get_id();
    }
    PointLight(PointLight &&light) : _center(light.get_center())
    {
        set_type("light");
        _id = light.get_id();
    }
    ~PointLight() = default;

    void move(const double dx, const double dy, const double dz) override {_center.move(dx, dy, dz);}
    void rotate(const double ang_x, const double ang_y, const double ang_z) override {}
    void scale(const double kx, const double ky, const double kz) override {}

    Point get_center()
    {
        return _center;
    }

private:
    Point _center;
};

#endif // POINTLIGHT_H
