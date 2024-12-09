#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "BaseLight.h"
#include "Geom.h"
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
    PointLight(const Point &center, const Point &direction, const Point &up)
    {
        set_type("light");
        _center = center;
        _direction = direction;
        _up = up;
        _lookat = look_at(_center, _center + _direction, _up);
    }
    PointLight(PointLight &light) : _center(light.get_center()), _direction(light.get_direction()), _up(light.get_up())
    {
        set_type("light");
        _id = light.get_id();
        _lookat = light.get_look_at();
    }
    PointLight(PointLight &&light) : _center(light.get_center())
    {
        set_type("light");
        _id = light.get_id();
        _lookat = light.get_look_at();
    }
    ~PointLight() = default;

    void move(const double dx, const double dy, const double dz) override {_center.move(dx, dy, dz);}
    void rotate(const double ang_x, const double ang_y, const double ang_z) override {}
    void scale(const double kx, const double ky, const double kz) override {}

    Point get_center()
    {
        return _center;
    }
    Point get_direction()
    {
        return _direction;
    }
    Point get_up()
    {
        return _up;
    }
    Matrix get_look_at()
    {
        return _lookat;
    }

    Pixel get_projection(const Point &point)
    {
        Point pp(point);
        auto m_point = Matrix(pp);
        Matrix to_cam_coords = _lookat * m_point;
        Point p0;
        p0.set_x(to_cam_coords[0][0]);
        p0.set_y(to_cam_coords[1][0]);
        p0.set_z(to_cam_coords[2][0]);
        Pixel p;
        if (fabs(p0.get_z()) < eps)
            p0.set_z(1);
        double coef = 1000 / p0.get_z();
        p.set_x(std::round(p0.get_x() * coef));
        p.set_y(std::round(p0.get_y() * coef));
        p.set_z(p0.get_z());
        return p;
    }

private:
    Point _center;
    Point _direction;
    Point _up;
    Matrix _lookat;
};

#endif // POINTLIGHT_H
