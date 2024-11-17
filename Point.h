#ifndef POINT_H
#define POINT_H

#include <cmath>

//double to_radians(const double &angle) {return angle * M_PI / 180;}

class Point
{
public:
    Point() = default;
    Point(const double x, const double y, const double z)
    {
        _x = x;
        _y = y;
        _z = z;
    }
    Point(const Point &p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
    }
    Point(const Point &&p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
    }
    Point &operator=(const Point &p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
        return *this;
    }
    Point &operator=(const Point &&p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
        p.~Point();
        return *this;
    }
    Point &operator+=(const Point &p)
    {
        _x += p._x;
        _y += p._y;
        _z += p._z;
        return *this;
    }
    Point &operator+=(const Point &&p)
    {
        _x += p._x;
        _y += p._y;
        _z += p._z;
        p.~Point();
        return *this;
    }
    Point &operator-=(const Point &p)
    {
        _x -= p._x;
        _y -= p._y;
        _z -= p._z;
        return *this;
    }
    Point &operator-=(const Point &&p)
    {
        _x -= p._x;
        _y -= p._y;
        _z -= p._z;
        p.~Point();
        return *this;
    }
    Point &operator*=(const double num)
    {
        _x *= num;
        _y *= num;
        _z *= num;
    }
    Point &operator/=(const double num)
    {
        _x /= num;
        _y /= num;
        _z /= num;
    }
    ~Point() = default;

    double get_x() const {return _x;}
    double get_y() const {return _y;}
    double get_z() const {return _z;}
    void set_x(const double x) {_x = x;}
    void set_y(const double y) {_y = y;}
    void set_z(const double z) {_z = z;}

    bool operator==(const Point &p) {return this->is_equal(p);}
    bool is_equal(const Point &p)
    {
        return (_x == p._x) && (_y == p._y) && (_z = p._z);
    }

    bool operator!=(const Point &p) {return this->is_not_equal(p);}
    bool is_not_equal(const Point &p)
    {
        return (_x != p._x) || (_y != p._y) || (_z != p._z);
    }

    void move(const double &dx, const double &dy, const double &dz)
    {
        _x += dx;
        _y += dy;
        _z += dz;
    }

    void rotate_x(const Point &center, const double &angle)
    {
        double y1 = center._y + (_y - center._y) * cos(angle) + (center._z - _z) * sin(angle);
        double z1 = center._z + (_y - center._y) * sin(angle) + (_z - center._z) * cos(angle);
        _y = y1;
        _z = z1;
    }

    void rotate_y(const Point &center, const double &angle)
    {
        double x1 = center._x + (_x - center._x) * cos(angle) + (_z - center._z) * sin(angle);
        double z1 = center._z + (center._x - _x) * sin(angle) + (_z - center._z) * cos(angle);
        _x = x1;
        _z = z1;
    }

    void rotate_z(const Point &center, const double &angle)
    {
        double x1 = center._x + (_x - center._x) * cos(angle) + (center._y - _y) * sin(angle);
        double y1 = center._y + (_x - center._x) * sin(angle) + (_y - center._y) * cos(angle);
        _x = x1;
        _y = y1;
    }

    void rotate(const Point &center, const double &ang_x, const double &ang_y, const double &ang_z)
    {
        double x_rad, y_rad, z_rad;
        x_rad = ang_x * M_PI / 180;
        y_rad = ang_y * M_PI / 180;
        z_rad = ang_z * M_PI / 180;
        this->rotate_x(center, x_rad);
        this->rotate_y(center, y_rad);
        this->rotate_z(center, z_rad);
    }

    void scale_x(const Point &center, const double &kx)
    {
        _x = center._x + (_x - center._x) * kx;
    }

    void scale_y(const Point &center, const double &ky)
    {
        _y = center._y + (_y - center._y) * ky;
    }

    void scale_z(const Point &center,const double &kz)
    {
        _z = center._z + (_z - center._z) * kz;
    }

    void scale(const Point &center, const double &kx, const double &ky, const double &kz)
    {
        this->scale_x(center, kx);
        this->scale_y(center, ky);
        this->scale_z(center, kz);
    }

private:
    double _x;
    double _y;
    double _z;
};

#endif // POINT_H

