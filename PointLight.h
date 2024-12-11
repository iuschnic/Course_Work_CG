#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "BaseLight.h"
#include "Geom.h"
#include "Point.h"

class PointLight: public BaseLight
{
public:
    PointLight(const Point &center)
    {
        set_type("light");
        _center = center;
    }
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

    void move_to(const Point &center) {_center = center;}
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
