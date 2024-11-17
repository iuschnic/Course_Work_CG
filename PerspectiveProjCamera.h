#ifndef PERSPECTIVEPROJCAMERA_H
#define PERSPECTIVEPROJCAMERA_H

#include "BaseCamera.h"
#include "Geometry.h"

class PProjCamera: public BaseCamera
{
public:
    PProjCamera() : _center(Point(0, 0, 100)), _direction(Point(0, 0, -1)), _type("camera"){}
    PProjCamera(const Point &center) : _center(center), _direction(Point(0, 0, -1)), _type("camera") {}
    PProjCamera(Camera &camera) : _center(camera.get_center()), _direction(camera.get_direction()), _type("camera")
    {
        _id = camera.get_id();
    }
    PProjCamera(Camera &&camera) : _center(camera.get_center()), _direction(camera.get_direction()), _type("camera")
    {
        _id = camera.get_id();
    }
    ~PProjCamera() = default;

    void move(const double dx, const double dy, const double dz) override {_center.move(dx, dy, dz);}
    void rotate(const double ang_x, const double ang_y, const double ang_z) override {}
    void scale(const double kx, const double ky, const double kz) override {}

    Point& get_center() {return _center;}
    Point& get_direction() override {return _direction;}

    Point get_projection(const Point &point) override
    {
        //Возможно сделаю преобразование к координатам камеры
        Point p(point);
        double coef = 1 - point.get_z() / _center.get_z();
        p.set_x(point.get_x() / coef);
        p.set_y(point.get_y() / coef);
        p.set_z(point.get_z() / coef);
        return p;
    }

    /*
    //Устанавливает центр камеры в точку eye, вектор взгляда в (eye - center), вектор оси z камеры в up
    void lookat(Vec3f eye, Vec3f center, Vec3f up) {
        Vec3f z = (eye - center).normalize();
        Vec3f x = (up ^ z).normalize();
        Vec3f y = (z ^ x).normalize();
        Matrix Minv = Matrix::identity(4);
        Matrix Tr = Matrix::identity(4);
        for (int i=0; i<3; i++) {
            Minv[0][i] = x[i];
            Minv[1][i] = y[i];
            Minv[2][i] = z[i];
            Tr[i][3] = -center[i];
        }
        ModelView = Minv*Tr;
    }*/

private:
    Point _center;
    Point _direction;
    //Matrix lookat;
};

#endif // PERSPECTIVEPROJCAMERA_H
