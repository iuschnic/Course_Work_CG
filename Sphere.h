#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>
#include "Point.h"

#define SEGMENTS_PARALLEL 40
#define SEGMENTS_MERIDIAN 40

class Sphere
{
public:
    Sphere() = default;
    //Конструктор, строящий мэш сферы по ее центру и радиусу
    Sphere(const double &mass, const int &radius, const Point &center) : _center(Point(0, 0, 0)), _radius(radius), _mass(mass)
    {
        double delta_a = 1.0 * M_PI / SEGMENTS_PARALLEL;
        double delta_b = 2.0 * M_PI / SEGMENTS_MERIDIAN;
        //double alpha = -M_PI / 2.0 + delta_a;
        double alpha = 0;
        double beta = 0;
        std::vector<Point> cur_points;
        Point up_pole(0, 0, radius);
        Point down_pole(0, 0, -radius);
        //Заносим в массив точек нижний полюс сферы
        _points.push_back(up_pole);
        /*_points.push_back(Point(-100, -100, -100));
        _points.push_back(Point(100, -100, -100));
        _points.push_back(Point(100, 100, -100));
        _points.push_back(Point(-100, 100, -100));
        _faces.push_back({0, 1, 2, 3});*/
        //std::cout << "down pole " << down_pole.get_x() << " " << down_pole.get_y() << " " << down_pole.get_z() << std::endl;
        for (int i = 1; i <= SEGMENTS_PARALLEL + 1; ++i)
        {
            beta = 0;
            cur_points.clear();
            int size = _points.size();
            //Получение точек на одной параллели, занесение в cur_points
            //std::cout << "alpha " << alpha << std::endl;
            for (int j = 0; j < SEGMENTS_MERIDIAN; ++j)
            {
                double x = radius * sin(alpha) * cos(beta);
                double y = radius * sin(alpha) * sin(beta);
                double z = radius * cos(alpha);
                Point cur(x, y, z);
                cur_points.push_back(cur);
                beta += delta_b;
                //std::cout << "Point circle " << i << " " << cur.get_x() << " " << cur.get_y() << " " << cur.get_z() << std::endl;
            }
            /*for (auto &p: _points)
            {
                std::cout << "Point circle " << i << " " << p.get_x() << " " << p.get_y() << " " << p.get_z() << std::endl;
            }*/
            //Если это первая построенная параллель, гранями будут треугольники, содержащие нижний полюс
            if (i == 1)
            {
                //OK
                //Нижний полюс уже занесен в массив, поэтому нужно учитывать смещение
                //Заносим в массив граней (грань = массив точек, принадлежащих грани)
                for (int t = 0; t < cur_points.size() - 1; t++)
                {
                    std::vector<int> face = {0, t + 1, t + 2};
                    _faces.push_back(face);
                    //std::cout << "face: " << 0 << " " << t + 1 << " " << t + 2 << std::endl;
                }
                //Занесение грани между первой и последней точкой параллели
                std::vector<int> face = {0, 1, cur_points.size()};
                _faces.push_back(face);
                //std::cout << "faces: " << _faces.size() << std::endl;
            }
            //_points.insert(_points.end(), cur_points.begin(), cur_points.end());
            //Если это последняя построенная параллель, гранями будут треугольники, содержащие верхний полюс
            else if (i == SEGMENTS_PARALLEL + 1)
            {
                _points.push_back(down_pole);
                size++;
                for (int t = 0; t < cur_points.size() - 1; t++)
                {
                    std::vector<int> face = {size - 1, size - 1 - cur_points.size() + t, size + t - cur_points.size()};
                    _faces.push_back(face);
                    //std::cout << "face: " << 0 << " " << t + 1 << " " << t + 2 << std::endl;
                }
                //Занесение грани между первой и последней точкой параллели
                std::vector<int> face = {size - 1, size, size - 1 + cur_points.size()};
                _faces.push_back(face);
                //break;
            }
            //Если это обычная параллель посередине, гранями будут четырехугольники состоящие из вершин двух параллелей
            else
            {
                //ОК
                for (int t = 0; t < cur_points.size() - 1; t++)
                {
                    std::vector<int> face = {size + t, size + 1 + t, size + 1 + t - cur_points.size(), size + t - cur_points.size()};
                    //std::vector<int> face = {size + t, size + 1 + t};
                    _faces.push_back(face);
                }
                std::vector<int> face = {size, size - cur_points.size(), size - 1, size - 1 + cur_points.size()};
                //std::vector<int> face = {size, size + cur_points.size() - 1};
                _faces.push_back(face);
            }
            //Занесение всех точек меридиана в массив точек
            _points.insert(_points.end(), cur_points.begin(), cur_points.end());
            alpha += delta_a;
        }
        move(center.get_x(), center.get_y(), center.get_z());
    }

    Sphere(const Sphere &sphere) : _points(sphere.get_points()), _faces(sphere.get_faces()), _center(sphere.get_center()),
        _radius(sphere.get_radius()), _mass(sphere.get_mass()) {}
    Sphere(const Sphere &&sphere) : _points(sphere.get_points()), _faces(sphere.get_faces()), _center(sphere.get_center()),
        _radius(sphere.get_radius()), _mass(sphere.get_mass()) {}

    ~Sphere() = default;

    const std::vector<Point> get_points() const
    {
        return _points;
    }
    const std::vector<std::vector<int>> get_faces() const
    {
        return _faces;
    }
    const double get_radius() const
    {
        return _radius;
    }
    const double get_mass() const
    {
        return _mass;
    }
    const Point get_center() const
    {
        return _center;
    }
    void set_center(const Point &point)
    {
        _center = point;
    }
    void set_radius(const double &radius)
    {
        _radius = radius;
    }
    void set_mass(const double &mass)
    {
        _mass = mass;
    }
    void add_point(Point &point)
    {
        _points.push_back(point);
    }
    void add_face(std::vector<int> face)
    {
        _faces.push_back(face);
    }
    void move(const double &dx, const double &dy, const double &dz)
    {
        for (auto &p : _points)
            p.move(dx, dy, dz);
        _center.move(dx, dy, dz);
    }
    void rotate(const double &ang_x, const double &ang_y, const double &ang_z)
    {
        for (auto &p : _points)
            p.rotate(_center, ang_x, ang_y, ang_z);
    }
    void scale(const double &kx, const double &ky, const double &kz, const Point &center)
    {
        for (auto &p : _points)
            p.scale(_center, kx, ky, kz);
    }
    bool check_intersection(const Sphere &sphere)
    {
        double sum_r = sphere.get_radius() + get_radius();
        Point c = sphere.get_center();
        double dist = pow((pow((c.get_x() - _center.get_x()), 2) + pow((c.get_y() - _center.get_y()), 2) + pow((c.get_z() - _center.get_z()), 2)), 0.5);
        if (dist > sum_r)
            return false;
        return true;
    }

private:
    std::vector<Point> _points;
    std::vector<std::vector<int>> _faces;
    Point _center;
    double _radius;
    double _mass;
};

#endif // MESH_H
