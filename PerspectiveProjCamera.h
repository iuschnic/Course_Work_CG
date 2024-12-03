#ifndef PERSPECTIVEPROJCAMERA_H
#define PERSPECTIVEPROJCAMERA_H

#include "BaseCamera.h"
#include "Geom.h"

class PProjCamera: public BaseCamera
{
public:
    PProjCamera()
    {
        _center = Point(0, 0, 0);
        _direction = Point(0, 0, -1);
        _up = Point(0, 1, 0);
        set_type("camera");
        _lookat = look_at(_direction, _center, _up);
        //Point up = Point(1, 0, 0);
        //_lookat = Matrix();
        //_lookat = look_at(_center, _direction, up);
    }
    PProjCamera(const Point &center)
    {
        _center = center;
        _direction = Point(0, 0, -1);
        _up = Point(0, 1, 0);
        set_type("camera");
        _lookat = look_at(_direction, _center, _up);
        //Point up = Point(1, 0, 0);
        //_lookat = Matrix();
        //_lookat = look_at(_center, _direction, up);
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

    Point& get_center() {return _center;}
    Point& get_direction() override {return _direction;}
    Point& get_up() {return _up;}
    Matrix get_look_at() {return _lookat;}

    Point get_projection(const Point &point) override
    {
        //printf("In getproj\n");
        //Возможно сделаю преобразование к координатам камеры
        //Point up = Point(1, 0, 0);
        //_lookat = look_at(_direction, _center, Point(1, 0, 0));
        //std::cout << lookat;
        Point pp(point);
        auto m_point = Matrix(pp);
        Matrix to_cam_coords = _lookat * m_point;
        Point p0;
        p0.set_x(to_cam_coords[0][0]);
        p0.set_y(to_cam_coords[1][0]);
        p0.set_z(to_cam_coords[2][0]);
        Point p;
        double coef = 1 - p0.get_z() / _center.get_z();
        p.set_x(p0.get_x() / coef - _center.get_x());
        p.set_y(p0.get_y() / coef - _center.get_y());
        p.set_z(p0.get_z() / coef);
        return p;

        /*Point p(point);
        double coef = 1 - point.get_z() / _center.get_z();
        p.set_x(point.get_x() / coef - _center.get_x());
        p.set_y(point.get_y() / coef - _center.get_y());
        p.set_z(point.get_z() / coef);
        return p;*/
    }

private:
    Point _center;
    Point _direction;
    Point _up;
    Matrix _lookat;
};

#endif // PERSPECTIVEPROJCAMERA_H
