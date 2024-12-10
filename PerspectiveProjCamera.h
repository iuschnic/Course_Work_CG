#ifndef PERSPECTIVEPROJCAMERA_H
#define PERSPECTIVEPROJCAMERA_H

#include "BaseCamera.h"
#include "Geom.h"
#define eps 10e-3

class PProjCamera: public BaseCamera
{
public:
    PProjCamera()
    {
        _center = Point(0, 0, 0);
        _direction = Point(0, 0, -1);
        _up = Point(0, 1, 0);
        set_type("camera");
        _lookat = look_at(_center, _center + _direction, _up);
    }
    PProjCamera(const Point &center, const Point &direction, const Point &up)
    {
        _center = center;
        _direction = direction;
        _up = up;
        set_type("camera");
        _lookat = look_at(_center, _center + _direction, _up);
    }
    PProjCamera(PProjCamera &camera) : _center(camera.get_center()), _direction(camera.get_direction()), _up(camera.get_up())
    {
        set_type("camera");
        _id = camera.get_id();
        _lookat = camera.get_look_at();
    }
    PProjCamera(PProjCamera &&camera) : _center(camera.get_center()), _direction(camera.get_direction()), _up(camera.get_up())
    {
        set_type("camera");
        _id = camera.get_id();
        _lookat = camera.get_look_at();
    }
    ~PProjCamera() = default;

    void move(const double dx, const double dy, const double dz) override
    {
        _center.move(dx, dy, dz);
        //Point up = Point(1, 0, 0);
        _lookat = look_at(_center, _direction, _up);
    }
    void rotate(const double ang_x, const double ang_y, const double ang_z) override
    {
        _direction.rotate(_center, ang_x, ang_y, ang_z);
        _lookat = look_at(_center, _direction, _up);
    }
    void scale(const double kx, const double ky, const double kz) override {}

    void move_forward()
    {
        Point norm(_direction);
        norm.normalize();
        _center.move(norm.get_x(), norm.get_y(), norm.get_z());
        _lookat = look_at(_center, _center + _direction, _up);
    }
    void move_backward()
    {
        Point norm(_direction);
        norm *= -1;
        norm.normalize();
        _center.move(norm.get_x(), norm.get_y(), norm.get_z());
        _lookat = look_at(_center, _center + _direction, _up);
    }
    void move_right()
    {
        Point norm = _up * _direction;
        norm.normalize();
        _center.move(norm.get_x(), norm.get_y(), norm.get_z());
        _lookat = look_at(_center, _center + _direction, _up);
    }
    void move_left()
    {
        Point norm = _direction * _up;
        norm.normalize();
        _center.move(norm.get_x(), norm.get_y(), norm.get_z());
        _lookat = look_at(_center, _center + _direction, _up);
    }
    void move_up()
    {
        Point norm = _up;
        norm.normalize();
        _center.move(norm.get_x(), norm.get_y(), norm.get_z());
        _lookat = look_at(_center, _center + _direction, _up);
    }
    void move_down()
    {
        Point norm = _up * -1;
        norm.normalize();
        _center.move(norm.get_x(), norm.get_y(), norm.get_z());
        _lookat = look_at(_center, _center + _direction, _up);
    }
    void rotate_right()
    {
        Point c = Point(0, 0, 0);
        _direction.rotate(c, 0, 1, 0);
        _lookat = look_at(_center, _center + _direction, _up);
    }
    void rotate_left()
    {
        Point c = Point(0, 0, 0);
        _direction.rotate(c, 0, -1, 0);
        _lookat = look_at(_center, _center + _direction, _up);
    }

    Point get_center() override {return _center;}
    Point get_direction() override {return _direction;}
    Point get_up() {return _up;}
    Matrix get_look_at() {return _lookat;}

    Pixel get_projection(const Point &point) override
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

#endif // PERSPECTIVEPROJCAMERA_H
