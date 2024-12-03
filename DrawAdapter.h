#ifndef DRAW_ADAPTER_H
#define DRAW_ADAPTER_H

#include <iostream>
#include <limits>
#include "QTDrawer.h"
#include "PerspectiveProjCamera.h"
#include "Object.h"

class DrawAdapter
{
public:
    DrawAdapter() = default;
    ~DrawAdapter() = default;
    void set_adaptee(std::shared_ptr<Object> adaptee) {_adaptee = adaptee;}
    void set_camera(std::shared_ptr<BaseCamera> camera) {_camera = camera;}
    void set_drawer(std::shared_ptr<BaseDrawer> drawer)
    {
        _z_buf.clear();
        _drawer = drawer;
        size_t height = _drawer->get_height();
        size_t width = _drawer->get_width();
        init_z_buf(height, width);
    }
    void set_adaptee_intensities(std::vector<std::vector<double>> &intensities)
    {
        _intensities = intensities;
    }
    std::shared_ptr<BaseCamera> get_camera() {return _camera;}
    std::shared_ptr<BaseDrawer> get_drawer() {return _drawer;}
    void request()
    {
        /*if (_adaptee)
            std::cout << "adaptee\n";
        if (_camera)
            std::cout << "camera\n";
        if (_drawer)
            std::cout << "drawer\n";*/
        QColor color;
        if (_adaptee && _camera && _drawer)
        {
            //std::cout << "OK\n";
            auto spheres = _adaptee->get_spheres();
            //for (const auto &sphere : _adaptee->get_spheres())
            for (int i = 0; i < spheres.size(); i++)
            {
                auto sphere = spheres[i];
                auto intensities = _intensities[i];
                if (_adaptee->get_mass() < 0)
                {
                    color.setRed(0);
                    color.setGreen(0);
                    color.setBlue(255);
                }
                else
                {
                    color.setRed(255);
                    color.setGreen(0);
                    color.setBlue(0);
                }
                //std::cout << "found sphere\n";
                std::vector<Point> points = sphere->get_points();
                std::vector<std::vector<int>> faces = sphere->get_faces();
                //std::cout << points.size() << std::endl;
                //std::cout << faces.size() << std::endl;
                for (const auto &face : faces)
                {
                    Point s_center = sphere->get_center();
                    Point c_vec = _camera->get_direction();
                    Point p = points[face[0]];
                    Point n_vec = p - s_center;
                    double scalar = c_vec.get_x() * n_vec.get_x() + c_vec.get_y() * n_vec.get_y() + c_vec.get_z() * n_vec.get_z();
                    if (scalar < 0)
                        continue;
                    //std::cout << "FACE\n";
                    /*for (int i = 0; i < face.size() - 1; i++)
                    {
                        //std::cout << "|" << get_projection(points[i]).get_x() << "|" << get_projection(points[i + 1]).get_x() << "|\n";
                        _drawer->addLine(get_projection(points[face[i]]),
                                         get_projection(points[face[i + 1]]), QColor(255, 255, 255));
                    }
                    _drawer->addLine(get_projection(points[face[0]]),
                                       get_projection(points[face[face.size() - 1]]), QColor(255, 255, 255));*/
                    if (face.size() == 3)
                    {
                        Point t0 = get_projection(points[face[0]]);
                        Point t1 = get_projection(points[face[1]]);
                        Point t2 = get_projection(points[face[2]]);
                        /*t0.set_x(t0.get_x() + _drawer->get_width() / 2);
                        t0.set_y(t0.get_y() + _drawer->get_height() / 2);
                        t1.set_x(t1.get_x() + _drawer->get_width() / 2);
                        t1.set_y(t1.get_y() + _drawer->get_height() / 2);
                        t2.set_x(t2.get_x() + _drawer->get_width() / 2);
                        t2.set_y(t2.get_y() + _drawer->get_height() / 2);*/
                        /*Pixel p0 = Pixel(t0);
                        Pixel p1 = Pixel(t1);
                        Pixel p2 = Pixel(t2);
                        p0.set_x(p0.get_x() + _drawer->get_width() / 2);
                        p0.set_y(p0.get_y() + _drawer->get_height() / 2);
                        p1.set_x(p1.get_x() + _drawer->get_width() / 2);
                        p1.set_y(p1.get_y() + _drawer->get_height() / 2);
                        p2.set_x(p2.get_x() + _drawer->get_width() / 2);
                        p2.set_y(p2.get_y() + _drawer->get_height() / 2);
                        p0.set_intensity(intensities[face[0]]);
                        p1.set_intensity(intensities[face[1]]);
                        p2.set_intensity(intensities[face[2]]);*/
                        triangle(t0, t1, t2, color, intensities[face[0]], intensities[face[1]], intensities[face[2]]);
                    }
                    else if (face.size() == 4)
                    {
                        Point t0 = get_projection(points[face[1]]);
                        Point t1 = get_projection(points[face[2]]);
                        Point t2 = get_projection(points[face[3]]);
                        /*t0.set_x(t0.get_x() + _drawer->get_width() / 2);
                        t0.set_y(t0.get_y() + _drawer->get_height() / 2);
                        t1.set_x(t1.get_x() + _drawer->get_width() / 2);
                        t1.set_y(t1.get_y() + _drawer->get_height() / 2);
                        t2.set_x(t2.get_x() + _drawer->get_width() / 2);
                        t2.set_y(t2.get_y() + _drawer->get_height() / 2);*/
                        /*Pixel p0 = Pixel(t0);
                        Pixel p1 = Pixel(t1);
                        Pixel p2 = Pixel(t2);
                        p0.set_x(p0.get_x() + _drawer->get_width() / 2);
                        p0.set_y(p0.get_y() + _drawer->get_height() / 2);
                        p1.set_x(p1.get_x() + _drawer->get_width() / 2);
                        p1.set_y(p1.get_y() + _drawer->get_height() / 2);
                        p2.set_x(p2.get_x() + _drawer->get_width() / 2);
                        p2.set_y(p2.get_y() + _drawer->get_height() / 2);
                        p0.set_intensity(intensities[face[1]]);
                        p1.set_intensity(intensities[face[2]]);
                        p2.set_intensity(intensities[face[3]]);*/
                        /*std::cout << "FACE\n";
                        std::cout << face[1] << " " << face[2] << " " << face[3] << std::endl;
                        std::cout << t0.get_x() << " " << t0.get_y() << " " << t0.get_z() << std::endl;
                        std::cout << t1.get_x() << " " << t1.get_y() << " " << t1.get_z() << std::endl;
                        std::cout << t2.get_x() << " " << t2.get_y() << " " << t2.get_z() << std::endl;*/
                        triangle(t0, t1, t2, color, intensities[face[1]], intensities[face[2]], intensities[face[3]]);
                        t0 = get_projection(points[face[0]]);
                        t1 = get_projection(points[face[1]]);
                        t2 = get_projection(points[face[3]]);
                        /*t0.set_x(t0.get_x() + _drawer->get_width() / 2);
                        t0.set_y(t0.get_y() + _drawer->get_height() / 2);
                        t1.set_x(t1.get_x() + _drawer->get_width() / 2);
                        t1.set_y(t1.get_y() + _drawer->get_height() / 2);
                        t2.set_x(t2.get_x() + _drawer->get_width() / 2);
                        t2.set_y(t2.get_y() + _drawer->get_height() / 2);*/
                        /*p0 = Pixel(t0);
                        p1 = Pixel(t1);
                        p2 = Pixel(t2);
                        p0.set_x(p0.get_x() + _drawer->get_width() / 2);
                        p0.set_y(p0.get_y() + _drawer->get_height() / 2);
                        p1.set_x(p1.get_x() + _drawer->get_width() / 2);
                        p1.set_y(p1.get_y() + _drawer->get_height() / 2);
                        p2.set_x(p2.get_x() + _drawer->get_width() / 2);
                        p2.set_y(p2.get_y() + _drawer->get_height() / 2);
                        p0.set_intensity(intensities[face[0]]);
                        p1.set_intensity(intensities[face[1]]);
                        p2.set_intensity(intensities[face[3]]);*/
                        /*std::cout << "FACE\n";
                        std::cout << face[0] << " " << face[1] << " " << face[3] << std::endl;
                        std::cout << t0.get_x() << " " << t0.get_y() << " " << t0.get_z() << std::endl;
                        std::cout << t1.get_x() << " " << t1.get_y() << " " << t1.get_z() << std::endl;
                        std::cout << t2.get_x() << " " << t2.get_y() << " " << t2.get_z() << std::endl;*/
                        triangle(t0, t1, t2, color, intensities[face[0]], intensities[face[1]], intensities[face[3]]);
                    }

                }
            }
        }
    }
    void draw()
    {
        _drawer->draw();
        clear_z_buf();
    }
    void clear()
    {
        _drawer->clear();
        clear_z_buf();
    }
private:
    std::shared_ptr<BaseCamera> _camera;
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Object> _adaptee;
    std::vector<std::vector<double>> _z_buf;
    std::vector<std::vector<double>> _intensities;
    Point get_projection(const Point &point)
    {
        return _camera->get_projection(point);
    }

    void init_z_buf(size_t &height, size_t &width)
    {
        _z_buf.clear();
        for (size_t i = 0; i < height; i++)
        {
            _z_buf.push_back({});
            for (size_t j = 0; j < width; j++)
            {
                _z_buf[i].push_back(std::numeric_limits<double>::max());
            }
        }
    }

    void clear_z_buf()
    {
        size_t height = _drawer->get_height();
        size_t width = _drawer->get_width();
        for (size_t i = 0; i < height; i++)
        {
            for (size_t j = 0; j < width; j++)
            {
                _z_buf[i][j] = std::numeric_limits<double>::max();
            }
        }
    }

    /*void triangle(Pixel &t0, Pixel &t1, Pixel &t2, QColor &color) {
        if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y())
        {
            if (t0.get_x() > t1.get_x()) std::swap(t0, t1);
            if (t0.get_x() > t2.get_x()) std::swap(t0, t2);
            if (t1.get_x() > t2.get_x()) std::swap(t1, t2);
            double dz1 = (t1.get_z() - t0.get_z()) / (t1.get_x() - t0.get_x() + 1);
            double dz2 = (t2.get_z() - t1.get_z()) / (t2.get_x() - t1.get_x() + 1);
            double z = t0.get_z();
            double di1 = (t1.get_intensity() - t0.get_intensity()) / (t1.get_x() - t0.get_x() + 1);
            double di2 = (t2.get_intensity() - t1.get_intensity()) / (t2.get_x() - t1.get_x() + 1);
            double i = t0.get_intensity();
            for (int j = t0.get_x(); j <= t1.get_x(); j++) {
                if (t0.get_y() >= _z_buf.size() || t0.get_y() < 0 || j < 0 || j >= _z_buf[0].size())
                    continue;
                if (_z_buf[t0.get_y()][j] > z) {
                    _z_buf[t0.get_y()][j] = z;
                    auto pixel = Pixel(j, t0.get_y(), z, i);
                    _drawer->add_point(pixel, color);
                }
                z += dz1;
                i += di1;
            }
            for (int j = t1.get_x(); j <= t2.get_x(); j++) {
                if (t0.get_y() >= _z_buf.size() || t0.get_y() < 0 || j < 0 || j >= _z_buf[0].size())
                    continue;
                if (_z_buf[t0.get_y()][j] > z) {
                    _z_buf[t0.get_y()][j] = z;
                    auto pixel = Pixel(j, t0.get_y(), z, i);
                    _drawer->add_point(pixel, color);
                }
                z += dz2;
                i += di2;
            }
            return;
        }
        if (t0.get_y() > t1.get_y()) std::swap(t0, t1);
        if (t0.get_y() > t2.get_y()) std::swap(t0, t2);
        if (t1.get_y() > t2.get_y()) std::swap(t1, t2);

        //Расчет инкрементов глубины и интенсивности при движении по оси y, двигаемся снизу вверх, от t0 к t1
        double dz_y_0_1 = (t1.get_z() - t0.get_z()) / (t1.get_y() - t0.get_y() + 1); //Между t0 и t1
        double dz_y_0_2 = (t2.get_z() - t0.get_z()) / (t2.get_y() - t0.get_y() + 1); //Между t0 и t2
        double di_y_0_1 = (t1.get_intensity() - t0.get_intensity()) / (t1.get_y() - t0.get_y() + 1);
        double di_y_0_2 = (t2.get_intensity() - t0.get_intensity()) / (t2.get_y() - t0.get_y() + 1);
        double z0 = t0.get_z(); //начальное значение z
        double i0 = t0.get_intensity();
        double z_0_1 = z0; //значение z на отрезке 0-1
        double z_0_2 = z0; //значение z на отрезке 0-2
        double i_0_1 = i0;
        double i_0_2 = i0;
        int total_height = t2.get_y() - t0.get_y();
        int segment_height = t1.get_y() - t0.get_y() + 1;
        for (int y = t0.get_y(); y <= t1.get_y(); y++) {
            double alpha = (double)(y - t0.get_y()) / total_height;
            double beta  = (double)(y - t0.get_y()) / segment_height; // be careful with divisions by zero
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 0-2
            Pixel p_0_1 = t0 + (t1 - t0) * beta; //Точка на отрезке 0-1
            //double dz_0_2 = dz_y_0_2;
            //double dz_0_1 = dz_y_0_1;
            if (p_0_2.get_x() > p_0_1.get_x())
            {
                double z_cur = z_0_1;
                double i_cur = i_0_1;
                double dz_x = (z_0_2 - z_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                double di_x = (i_0_2 - i_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                for (int j = p_0_1.get_x(); j <= p_0_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] > z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto pixel = Pixel(j, y, 0, i_cur);
                        _drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                }
            }
            else
            {
                double z_cur = z_0_2;
                double i_cur = i_0_2;
                double dz_x = (z_0_1 - z_0_2) / (p_0_1.get_x() - p_0_2.get_x() + 1);
                double di_x = (i_0_1 - i_0_2) / (p_0_1.get_x() - p_0_2.get_x() + 1);
                for (int j = p_0_2.get_x(); j <= p_0_1.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] > z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto pixel = Pixel(j, y, 0, i_cur);
                        _drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                }
            }
            z_0_1 += dz_y_0_1;
            z_0_2 += dz_y_0_2;
            i_0_1 += di_y_0_1;
            i_0_2 += di_y_0_2;
        }

        double z_1_2 = t1.get_z(); //начальное значение z на отрезке 1-2
        double i_1_2 = t1.get_intensity();
        double dz_y_1_2 = (t2.get_z() - t1.get_z()) / (t2.get_y() - t1.get_y() + 1); //Между t1 и t1
        double di_y_1_2 = (t2.get_intensity() - t1.get_intensity()) / (t2.get_y() - t1.get_y() + 1);
        //z_0_2 остается тем же
        segment_height = t2.get_y() - t1.get_y() + 1;
        for (int y = t1.get_y(); y <= t2.get_y(); y++) {
            double alpha = (double)(y - t0.get_y()) / total_height;
            double beta  = (double)(y - t1.get_y()) / segment_height; // be careful with divisions by zero
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 2-0
            Pixel p_1_2 = t1 + (t2 - t1) * beta; //Точка на отрезке 2-1
            //double dz_0_2 = dz_y_0_2;
            //double dz_0_1 = dz_y_0_1;
            if (p_0_2.get_x() > p_1_2.get_x())
            {
                double z_cur = z_1_2;
                double i_cur = i_1_2;
                double dz_x = (z_0_2 - z_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                double di_x = (i_0_2 - i_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                for (int j = p_1_2.get_x(); j <= p_0_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] >= z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto pixel = Pixel(j, y, 0, i_cur);
                        _drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                }
            }
            else
            {
                double z_cur = z_0_2;
                double i_cur = i_0_2;
                double dz_x = (z_1_2 - z_0_2) / (p_1_2.get_x() - p_0_2.get_x() + 1);
                double di_x = (i_1_2 - i_0_2) / (p_1_2.get_x() - p_0_2.get_x() + 1);
                for (int j = p_0_2.get_x(); j <= p_1_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] >= z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto pixel = Pixel(j, y, 0, i_cur);
                        _drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                }
            }
            z_1_2 += dz_y_1_2;
            z_0_2 += dz_y_0_2;
            i_1_2 += di_y_1_2;
            i_0_2 += di_y_0_2;
        }
    }*/
    void triangle(Point &p0, Point &p1, Point &p2, QColor &color, double &i0, double &i1, double &i2) {
        Pixel t0 = Pixel(p0);
        Pixel t1 = Pixel(p1);
        Pixel t2 = Pixel(p2);
        t0.set_x(t0.get_x() + _drawer->get_width() / 2);
        t0.set_y(t0.get_y() + _drawer->get_height() / 2);
        t1.set_x(t1.get_x() + _drawer->get_width() / 2);
        t1.set_y(t1.get_y() + _drawer->get_height() / 2);
        t2.set_x(t2.get_x() + _drawer->get_width() / 2);
        t2.set_y(t2.get_y() + _drawer->get_height() / 2);
        t0.set_intensity(i0);
        t1.set_intensity(i1);
        t2.set_intensity(i2);
        if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y())
        {
            if (t0.get_x() > t1.get_x()) std::swap(t0, t1);
            if (t0.get_x() > t2.get_x()) std::swap(t0, t2);
            if (t1.get_x() > t2.get_x()) std::swap(t1, t2);
            double dz1 = (t1.get_z() - t0.get_z()) / (t1.get_x() - t0.get_x() + 1);
            double dz2 = (t2.get_z() - t1.get_z()) / (t2.get_x() - t1.get_x() + 1);
            double z = t0.get_z();
            double di1 = (t1.get_intensity() - t0.get_intensity()) / (t1.get_x() - t0.get_x() + 1);
            double di2 = (t2.get_intensity() - t1.get_intensity()) / (t2.get_x() - t1.get_x() + 1);
            double i = t0.get_intensity();
            for (int j = t0.get_x(); j <= t1.get_x(); j++) {
                if (t0.get_y() >= _z_buf.size() || t0.get_y() < 0 || j < 0 || j >= _z_buf[0].size())
                    continue;
                if (_z_buf[t0.get_y()][j] > z) {
                    _z_buf[t0.get_y()][j] = z;
                    auto pixel = Pixel(j, t0.get_y(), z, i);
                    _drawer->add_point(pixel, color);
                }
                z += dz1;
                i += di1;
            }
            for (int j = t1.get_x(); j <= t2.get_x(); j++) {
                if (t0.get_y() >= _z_buf.size() || t0.get_y() < 0 || j < 0 || j >= _z_buf[0].size())
                    continue;
                if (_z_buf[t0.get_y()][j] > z) {
                    _z_buf[t0.get_y()][j] = z;
                    auto pixel = Pixel(j, t0.get_y(), z, i);
                    _drawer->add_point(pixel, color);
                }
                z += dz2;
                i += di2;
            }
            return;
        }
        if (t0.get_y() > t1.get_y()) std::swap(t0, t1);
        if (t0.get_y() > t2.get_y()) std::swap(t0, t2);
        if (t1.get_y() > t2.get_y()) std::swap(t1, t2);

        //Расчет инкрементов глубины и интенсивности при движении по оси y, двигаемся снизу вверх, от t0 к t1
        double dz_y_0_1 = (t1.get_z() - t0.get_z()) / (t1.get_y() - t0.get_y() + 1); //Между t0 и t1
        double dz_y_0_2 = (t2.get_z() - t0.get_z()) / (t2.get_y() - t0.get_y() + 1); //Между t0 и t2
        double di_y_0_1 = (t1.get_intensity() - t0.get_intensity()) / (t1.get_y() - t0.get_y() + 1);
        double di_y_0_2 = (t2.get_intensity() - t0.get_intensity()) / (t2.get_y() - t0.get_y() + 1);
        double z0 = t0.get_z(); //начальное значение z
        double in = t0.get_intensity();
        double z_0_1 = z0; //значение z на отрезке 0-1
        double z_0_2 = z0; //значение z на отрезке 0-2
        double i_0_1 = in;
        double i_0_2 = in;
        int total_height = t2.get_y() - t0.get_y();
        int segment_height = t1.get_y() - t0.get_y() + 1;
        for (int y = t0.get_y(); y <= t1.get_y(); y++) {
            double alpha = (double)(y - t0.get_y()) / total_height;
            double beta  = (double)(y - t0.get_y()) / segment_height; // be careful with divisions by zero
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 0-2
            Pixel p_0_1 = t0 + (t1 - t0) * beta; //Точка на отрезке 0-1
            //double dz_0_2 = dz_y_0_2;
            //double dz_0_1 = dz_y_0_1;
            if (p_0_2.get_x() > p_0_1.get_x())
            {
                double z_cur = z_0_1;
                double i_cur = i_0_1;
                double dz_x = (z_0_2 - z_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                double di_x = (i_0_2 - i_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                for (int j = p_0_1.get_x(); j <= p_0_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] > z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto pixel = Pixel(j, y, 0, i_cur);
                        _drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                }
            }
            else
            {
                double z_cur = z_0_2;
                double i_cur = i_0_2;
                double dz_x = (z_0_1 - z_0_2) / (p_0_1.get_x() - p_0_2.get_x() + 1);
                double di_x = (i_0_1 - i_0_2) / (p_0_1.get_x() - p_0_2.get_x() + 1);
                for (int j = p_0_2.get_x(); j <= p_0_1.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] > z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto pixel = Pixel(j, y, 0, i_cur);
                        _drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                }
            }
            z_0_1 += dz_y_0_1;
            z_0_2 += dz_y_0_2;
            i_0_1 += di_y_0_1;
            i_0_2 += di_y_0_2;
        }

        double z_1_2 = t1.get_z(); //начальное значение z на отрезке 1-2
        double i_1_2 = t1.get_intensity();
        double dz_y_1_2 = (t2.get_z() - t1.get_z()) / (t2.get_y() - t1.get_y() + 1); //Между t1 и t1
        double di_y_1_2 = (t2.get_intensity() - t1.get_intensity()) / (t2.get_y() - t1.get_y() + 1);
        //z_0_2 остается тем же
        segment_height = t2.get_y() - t1.get_y() + 1;
        for (int y = t1.get_y(); y <= t2.get_y(); y++) {
            double alpha = (double)(y - t0.get_y()) / total_height;
            double beta  = (double)(y - t1.get_y()) / segment_height; // be careful with divisions by zero
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 2-0
            Pixel p_1_2 = t1 + (t2 - t1) * beta; //Точка на отрезке 2-1
            //double dz_0_2 = dz_y_0_2;
            //double dz_0_1 = dz_y_0_1;
            if (p_0_2.get_x() > p_1_2.get_x())
            {
                double z_cur = z_1_2;
                double i_cur = i_1_2;
                double dz_x = (z_0_2 - z_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                double di_x = (i_0_2 - i_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                for (int j = p_1_2.get_x(); j <= p_0_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] >= z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto pixel = Pixel(j, y, 0, i_cur);
                        _drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                }
            }
            else
            {
                double z_cur = z_0_2;
                double i_cur = i_0_2;
                double dz_x = (z_1_2 - z_0_2) / (p_1_2.get_x() - p_0_2.get_x() + 1);
                double di_x = (i_1_2 - i_0_2) / (p_1_2.get_x() - p_0_2.get_x() + 1);
                for (int j = p_0_2.get_x(); j <= p_1_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] >= z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto pixel = Pixel(j, y, 0, i_cur);
                        _drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                }
            }
            z_1_2 += dz_y_1_2;
            z_0_2 += dz_y_0_2;
            i_1_2 += di_y_1_2;
            i_0_2 += di_y_0_2;
        }
    }
};

#endif // DRAW_ADAPTER_H

