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
    /*Point(const Pixel &p)
    {
        _x = p.get_x();
        _y = p.get_y();
        _z = p.get_z();
    }
    Point(const Pixel &&p)
    {
        _x = p.get_x();
        _y = p.get_y();
        _z = p.get_z();
    }*/
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
    Point operator+(const Point &p)
    {
        Point tmp(_x + p.get_x(), _y + p.get_y(), _z + p.get_z());
        return tmp;
    }
    Point operator+(const Point &&p)
    {
        Point tmp(_x + p.get_x(), _y + p.get_y(), _z + p.get_z());
        return tmp;
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
    Point operator-(const Point &p)
    {
        Point tmp(_x - p.get_x(), _y - p.get_y(), _z - p.get_z());
        return tmp;
    }
    Point operator-(const Point &&p)
    {
        Point tmp(_x - p.get_x(), _y - p.get_y(), _z - p.get_z());
        return tmp;
    }
    Point operator*(const Point &&p)
    {
        Point tmp;
        tmp.set_x(_y * p.get_z() - _z * p.get_y());
        tmp.set_y(_z * p.get_x() - _x * p.get_z());
        tmp.set_z(_x * p.get_y() - _y * p.get_x());
        return tmp;
    }
    Point operator*(const Point &p)
    {
        Point tmp;
        tmp.set_x(_y * p.get_z() - _z * p.get_y());
        tmp.set_y(_z * p.get_x() - _x * p.get_z());
        tmp.set_z(_x * p.get_y() - _y * p.get_x());
        return tmp;
    }
    Point &operator*=(const double num)
    {
        _x *= num;
        _y *= num;
        _z *= num;
        return *this;
    }
    Point operator*(const double num)
    {
        return Point(_x * num, _y * num, _z * num);
    }
    Point &operator*=(const int num)
    {
        _x *= num;
        _y *= num;
        _z *= num;
        return *this;
    }
    Point operator*(const int num)
    {
        return Point(_x * num, _y * num, _z * num);
    }
    Point &operator/=(const double num)
    {
        _x /= num;
        _y /= num;
        _z /= num;
    }
    Point operator/(const double num)
    {
        return Point(_x / num, _y / num, _z / num);
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

    void normalize()
    {
        double len = pow(pow(_x, 2) + pow(_y, 2) + pow(_z, 2), 0.5);
        _x /= len;
        _y /= len;
        _z /= len;
    }

private:
    double _x;
    double _y;
    double _z;
};

class Pixel
{
public:
    Pixel() = default;
    Pixel(const int x, const int y, const double z, const double intensity)
    {
        _x = x;
        _y = y;
        _z = z;
        _intensity = intensity;
    }
    Pixel(const double x, const double y, const double z, const double intensity)
    {
        _x = x;
        _y = y;
        _z = z;
        _intensity = intensity;
    }
    Pixel(const Pixel &p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
        _intensity = p.get_intensity();
    }
    Pixel(const Pixel &&p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
        _intensity = p.get_intensity();
    }
    Pixel(const Point &p)
    {
        _x = p.get_x();
        _y = p.get_y();
        _z = p.get_z();
        _intensity = 1;
    }
    Pixel(const Point &&p)
    {
        _x = p.get_x();
        _y = p.get_y();
        _z = p.get_z();
        _intensity = 1;
    }
    Pixel &operator=(const Pixel &p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
        _intensity = p._intensity;
        return *this;
    }
    Pixel &operator=(const Pixel &&p)
    {
        _x = p._x;
        _y = p._y;
        _z = p._z;
        _intensity = p._intensity;
        return *this;
    }
    Pixel &operator+=(const Pixel &p)
    {
        _x += p._x;
        _y += p._y;
        return *this;
    }
    Pixel &operator+=(const Pixel &&p)
    {
        _x += p._x;
        _y += p._y;
        return *this;
    }
    Pixel operator+(const Pixel &p)
    {
        Pixel tmp(_x + p.get_x(), _y + p.get_y(), _z, 1);
        return tmp;
    }
    Pixel operator+(const Pixel &&p)
    {
        Pixel tmp(_x + p.get_x(), _y + p.get_y(), _z, 1);
        return tmp;
    }
    Pixel &operator-=(const Pixel &p)
    {
        _x -= p._x;
        _y -= p._y;
        return *this;
    }
    Pixel &operator-=(const Pixel &&p)
    {
        _x -= p._x;
        _y -= p._y;
        return *this;
    }
    Pixel operator-(const Pixel &p)
    {
        Pixel tmp(_x - p.get_x(), _y - p.get_y(), _z, 1);
        return tmp;
    }
    Pixel operator-(const Pixel &&p)
    {
        Pixel tmp(_x - p.get_x(), _y - p.get_y(), _z, 1);
        return tmp;
    }
    Pixel &operator*=(const double num)
    {
        _x *= num;
        _y *= num;
        return *this;
    }
    Pixel operator*(const double num)
    {
        Pixel tmp(_x * num, _y * num, _z, 1);
        return tmp;
    }
    ~Pixel() = default;

    int get_x() const {return _x;}
    int get_y() const {return _y;}
    double get_z() const {return _z;}
    double get_intensity() const {return _intensity;}
    void set_x(const double x) {_x = x;}
    void set_y(const double y) {_y = y;}
    void set_z(const double z) {_z = z;}
    void set_intensity(const double intensity) {_intensity = intensity;}

    bool operator==(const Pixel &p) {return this->is_equal(p);}
    bool is_equal(const Pixel &p)
    {
        return (_x == p._x) && (_y == p._y);
    }

    bool operator!=(const Pixel &p) {return this->is_not_equal(p);}
    bool is_not_equal(const Pixel &p)
    {
        return (_x != p._x) || (_y != p._y);
    }

private:
    int _x;
    int _y;
    double _z;
    double _intensity;
};

#endif // POINT_H

