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
    void set_shade_buf(std::vector<std::vector<double>> &buf)
    {
        _shade_buf = buf;
    }
    std::shared_ptr<BaseCamera> get_camera() {return _camera;}
    std::shared_ptr<BaseDrawer> get_drawer() {return _drawer;}
    void request()
    {
        QColor color;
        if (_adaptee && _camera && _drawer)
        {
            auto spheres = _adaptee->get_spheres();
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
                std::vector<Point> points = sphere->get_points();
                std::vector<std::vector<int>> faces = sphere->get_faces();
                for (const auto &face : faces)
                {
                    /*Point s_center = sphere->get_center();
                    Point c_vec = _camera->get_direction();
                    Point p = points[face[0]];
                    Point n_vec = p - s_center;
                    double scalar = c_vec.get_x() * n_vec.get_x() + c_vec.get_y() * n_vec.get_y() + c_vec.get_z() * n_vec.get_z();*/
                    Point s_center = sphere->get_center();
                    Point c_cen = _camera->get_center();
                    Point p = points[face[0]];
                    Point n_vec = p - s_center;
                    Point c_vec = p - c_cen;
                    double scalar = c_vec.get_x() * n_vec.get_x() + c_vec.get_y() * n_vec.get_y() + c_vec.get_z() * n_vec.get_z();
                    if (scalar < 0)
                        continue;
                    if (face.size() == 3)
                    {
                        /*Point t0 = get_projection(points[face[0]]);
                        Point t1 = get_projection(points[face[1]]);
                        Point t2 = get_projection(points[face[2]]);
                        triangle(t0, t1, t2, color, intensities[face[0]], intensities[face[1]], intensities[face[2]]);*/
                        triangle(points[face[0]], points[face[1]], points[face[2]], color, intensities[face[0]], intensities[face[1]], intensities[face[2]]);
                    }
                    else if (face.size() == 4)
                    {
                        /*Point t0 = get_projection(points[face[1]]);
                        Point t1 = get_projection(points[face[2]]);
                        Point t2 = get_projection(points[face[3]]);
                        triangle(t0, t1, t2, color, intensities[face[1]], intensities[face[2]], intensities[face[3]]);*/
                        triangle(points[face[1]], points[face[2]], points[face[3]], color, intensities[face[1]], intensities[face[2]], intensities[face[3]]);
                        /*t0 = get_projection(points[face[0]]);
                        t1 = get_projection(points[face[1]]);
                        t2 = get_projection(points[face[3]]);
                        triangle(t0, t1, t2, color, intensities[face[0]], intensities[face[1]], intensities[face[3]]);*/
                        triangle(points[face[0]], points[face[1]], points[face[3]], color, intensities[face[0]], intensities[face[1]], intensities[face[3]]);
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
    void set_to_light(Matrix m)
    {
        _to_light = m;
    }
private:
    std::shared_ptr<BaseCamera> _camera;
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Object> _adaptee;
    std::vector<std::vector<double>> _z_buf;
    std::vector<std::vector<double>> _shade_buf;
    std::vector<std::vector<double>> _intensities;
    Matrix _to_light;
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

    void triangle(Point &pp0, Point &pp1, Point &pp2, QColor &color, double &i0, double &i1, double &i2) {
        //std::cout << "size1 " << _shade_buf.size() << " " << _shade_buf[0].size() << std::endl;
        Point p0 = get_projection(pp0);
        Point p1 = get_projection(pp1);
        Point p2 = get_projection(pp2);
        Pixel t0 = Pixel(p0);
        Pixel t1 = Pixel(p1);
        Pixel t2 = Pixel(p2);
        int height = _drawer->get_height() / 2;
        int width = _drawer->get_width() / 2;
        //std::cout << "WH " << width << " " << height << std::endl;
        t0.set_x(t0.get_x() + width);
        t0.set_y(t0.get_y() + height);
        t1.set_x(t1.get_x() + width);
        t1.set_y(t1.get_y() + height);
        t2.set_x(t2.get_x() + width);
        t2.set_y(t2.get_y() + height);
        t0.set_intensity(i0);
        t1.set_intensity(i1);
        t2.set_intensity(i2);
        if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y())
        {
            if (t0.get_x() > t1.get_x())
            {
                std::swap(t0, t1);
                std::swap(p0, p1);
            }
            if (t0.get_x() > t2.get_x())
            {
                std::swap(t0, t2);
                std::swap(p0, p2);
            }
            if (t1.get_x() > t2.get_x())
            {
                std::swap(t1, t2);
                std::swap(p1, p2);
            }
            double orig_dz1 = (p1.get_z() - p0.get_z()) / (t1.get_x() - t0.get_x() + 1);
            double orig_dz2 = (p2.get_z() - p1.get_z()) / (t2.get_x() - t1.get_x() + 1);
            double orig_z = p0.get_z();
            double orig_dx1 = (p1.get_x() - p0.get_x()) / (t1.get_x() - t0.get_x() + 1);
            double orig_dx2 = (p2.get_x() - p1.get_x()) / (t2.get_x() - t1.get_x() + 1);
            double orig_x = p0.get_x();
            double orig_dy1 = (p1.get_y() - p0.get_y()) / (t1.get_x() - t0.get_x() + 1);
            double orig_dy2 = (p2.get_y() - p1.get_y()) / (t2.get_x() - t1.get_x() + 1);
            double orig_y = p0.get_y();
            Point cur(orig_x, orig_y, orig_z);
            Point dcur(orig_dx1, orig_dy1, orig_dz1);
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
                    auto m_point = Matrix(cur);
                    Matrix to_light_coords = _to_light * m_point;
                    //int sx = to_light_coords[0][0] + width;
                    //int sy = to_light_coords[1][0] + height;
                    int sx = to_light_coords[0][0];
                    int sy = to_light_coords[1][0];
                    double sz = to_light_coords[2][0];
                    if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size())
                        std::cout << sx << " " << sy << " " << sz << std::endl;
                    if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size() || _shade_buf[sy][sx] < sz)
                    {
                        auto pixel = Pixel(j, t0.get_y(), z, 0.1);
                        _drawer->add_point(pixel, color);
                    }
                    else
                    {
                        auto pixel = Pixel(j, t0.get_y(), z, i);
                        _drawer->add_point(pixel, color);
                    }
                    //auto pixel = Pixel(j, t0.get_y(), z, i);
                    //_drawer->add_point(pixel, color);
                }
                z += dz1;
                i += di1;
                cur += dcur;
            }
            dcur = Point(orig_dx2, orig_dy2, orig_dz2);
            for (int j = t1.get_x(); j <= t2.get_x(); j++) {
                if (t0.get_y() >= _z_buf.size() || t0.get_y() < 0 || j < 0 || j >= _z_buf[0].size())
                    continue;
                if (_z_buf[t0.get_y()][j] > z) {
                    _z_buf[t0.get_y()][j] = z;
                    auto m_point = Matrix(cur);
                    Matrix to_light_coords = _to_light * m_point;
                    //int sx = to_light_coords[0][0] + width;
                    //int sy = to_light_coords[1][0] + height;
                    int sx = to_light_coords[0][0];
                    int sy = to_light_coords[1][0];
                    double sz = to_light_coords[2][0];
                    if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size())
                        std::cout << sx << " " << sy << " " << sz << std::endl;
                    if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size() || _shade_buf[sy][sx] < sz)
                    {
                        auto pixel = Pixel(j, t0.get_y(), z, 0.1);
                        _drawer->add_point(pixel, color);
                    }
                    else
                    {
                        auto pixel = Pixel(j, t0.get_y(), z, i);
                        _drawer->add_point(pixel, color);
                    }
                    //auto pixel = Pixel(j, t0.get_y(), z, i);
                    //_drawer->add_point(pixel, color);
                }
                z += dz2;
                i += di2;
                cur += dcur;
            }
            return;
        }
        if (t0.get_y() > t1.get_y())
        {
            std::swap(t0, t1);
            std::swap(p0, p1);
        }
        if (t0.get_y() > t2.get_y())
        {
            std::swap(t0, t2);
            std::swap(p0, p2);
        }
        if (t1.get_y() > t2.get_y())
        {
            std::swap(t1, t2);
            std::swap(p1, p2);
        }

        //Расчет инкрементов глубины и интенсивности при движении по оси y, двигаемся снизу вверх, от t0 к t1
        double dz_y_0_1 = (t1.get_z() - t0.get_z()) / (t1.get_y() - t0.get_y() + 1); //Между t0 и t1
        double dz_y_0_2 = (t2.get_z() - t0.get_z()) / (t2.get_y() - t0.get_y() + 1); //Между t0 и t2
        //double orig_dz_y_0_1 = (p1.get_z() - p0.get_z()) / (t1.get_y() - t0.get_y() + 1); //Между p0 и p1
        //double prig_dz_y_0_2 = (p2.get_z() - p0.get_z()) / (t2.get_y() - t0.get_y() + 1); //Между t0 и t2

        double di_y_0_1 = (t1.get_intensity() - t0.get_intensity()) / (t1.get_y() - t0.get_y() + 1);
        double di_y_0_2 = (t2.get_intensity() - t0.get_intensity()) / (t2.get_y() - t0.get_y() + 1);
        double z0 = t0.get_z(); //начальное значение z
        double in = t0.get_intensity();
        double z_0_1 = z0; //значение z на отрезке 0-1
        double z_0_2 = z0; //значение z на отрезке 0-2
        //double orig_z_0_1 = orig_z0;
        //double orig_z_0_2 = orig_z0;
        double i_0_1 = in;
        double i_0_2 = in;
        int total_height = t2.get_y() - t0.get_y();
        int segment_height = t1.get_y() - t0.get_y() + 1;
        for (int y = t0.get_y(); y <= t1.get_y(); y++) {
            double alpha = (double)(y - t0.get_y()) / total_height;
            double beta  = (double)(y - t0.get_y()) / segment_height; // be careful with divisions by zero
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 0-2
            Pixel p_0_1 = t0 + (t1 - t0) * beta; //Точка на отрезке 0-1
            Point orig_p_0_2 = p0 + (p2 - p0) * alpha;
            Point orig_p_0_1 = p0 + (p1 - p0) * beta;
            //double dz_0_2 = dz_y_0_2;
            //double dz_0_1 = dz_y_0_1;
            if (p_0_2.get_x() > p_0_1.get_x())
            {
                double z_cur = z_0_1;
                double i_cur = i_0_1;
                double dz_x = (z_0_2 - z_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                double di_x = (i_0_2 - i_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                Point orig_dp_x = (orig_p_0_2 - orig_p_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                for (int j = p_0_1.get_x(); j <= p_0_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] > z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto m_point = Matrix(orig_p_0_1);
                        Matrix to_light_coords = _to_light * m_point;
                        //int sx = to_light_coords[0][0] + width;
                        //int sy = to_light_coords[1][0] + height;
                        int sx = to_light_coords[0][0];
                        int sy = to_light_coords[1][0];
                        double sz = to_light_coords[2][0];
                        if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size())
                            std::cout << sx << " " << sy << " " << sz << std::endl;
                        if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size() || _shade_buf[sy][sx] < sz)
                        {
                            auto pixel = Pixel(j, y, 0, 0.1);
                            _drawer->add_point(pixel, color);
                        }
                        else
                        {
                            auto pixel = Pixel(j, y, 0, i_cur);
                            _drawer->add_point(pixel, color);
                        }
                        //auto pixel = Pixel(j, y, 0, i_cur);
                        //_drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                    orig_p_0_1 += orig_dp_x;
                }
            }
            else
            {
                double z_cur = z_0_2;
                double i_cur = i_0_2;
                double dz_x = (z_0_1 - z_0_2) / (p_0_1.get_x() - p_0_2.get_x() + 1);
                Point orig_dp_x = (orig_p_0_1 - orig_p_0_2) / (p_0_1.get_x() - p_0_2.get_x() + 1);
                double di_x = (i_0_1 - i_0_2) / (p_0_1.get_x() - p_0_2.get_x() + 1);
                for (int j = p_0_2.get_x(); j <= p_0_1.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] > z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto m_point = Matrix(orig_p_0_2);
                        Matrix to_light_coords = _to_light * m_point;
                        //int sx = to_light_coords[0][0] + width;
                        //int sy = to_light_coords[1][0] + height;
                        int sx = to_light_coords[0][0];
                        int sy = to_light_coords[1][0];
                        double sz = to_light_coords[2][0];
                        if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size())
                            std::cout << sx << " " << sy << " " << sz << std::endl;
                        if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size() || _shade_buf[sy][sx] < sz)
                        {
                            auto pixel = Pixel(j, y, 0, 0.1);
                            _drawer->add_point(pixel, color);
                        }
                        else
                        {
                            auto pixel = Pixel(j, y, 0, i_cur);
                            _drawer->add_point(pixel, color);
                        }
                        //auto pixel = Pixel(j, y, 0, i_cur);
                        //_drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                    orig_p_0_2 += orig_dp_x;
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
            Point orig_p_0_2 = p0 + (p2 - p0) * alpha;
            Point orig_p_1_2 = p1 + (p2 - p1) * beta;
            //double dz_0_2 = dz_y_0_2;
            //double dz_0_1 = dz_y_0_1;
            if (p_0_2.get_x() > p_1_2.get_x())
            {
                double z_cur = z_1_2;
                double i_cur = i_1_2;
                double dz_x = (z_0_2 - z_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                double di_x = (i_0_2 - i_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                Point orig_dp_x = (orig_p_0_2 - orig_p_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                for (int j = p_1_2.get_x(); j <= p_0_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] >= z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto m_point = Matrix(orig_p_1_2);
                        Matrix to_light_coords = _to_light * m_point;
                        //int sx = to_light_coords[0][0] + width;
                        //int sy = to_light_coords[1][0] + height;
                        int sx = to_light_coords[0][0];
                        int sy = to_light_coords[1][0];
                        double sz = to_light_coords[2][0];
                        if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size())
                            std::cout << sx << " " << sy << " " << sz << std::endl;
                        if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size() || _shade_buf[sy][sx] < sz)
                        {
                            auto pixel = Pixel(j, y, 0, 0.1);
                            _drawer->add_point(pixel, color);
                        }
                        else
                        {
                            auto pixel = Pixel(j, y, 0, i_cur);
                            _drawer->add_point(pixel, color);
                        }
                        //auto pixel = Pixel(j, y, 0, i_cur);
                        //_drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                    orig_p_1_2 += orig_dp_x;
                }
            }
            else
            {
                double z_cur = z_0_2;
                double i_cur = i_0_2;
                double dz_x = (z_1_2 - z_0_2) / (p_1_2.get_x() - p_0_2.get_x() + 1);
                double di_x = (i_1_2 - i_0_2) / (p_1_2.get_x() - p_0_2.get_x() + 1);
                Point orig_dp_x = (orig_p_1_2 - orig_p_0_2) / (p_1_2.get_x() - p_0_2.get_x() + 1);
                for (int j = p_0_2.get_x(); j <= p_1_2.get_x(); j++) {
                    if (y >= _z_buf.size() || y < 0 || j < 0 || j >= _z_buf[0].size())
                        continue;
                    if (_z_buf[y][j] >= z_cur) {
                        _z_buf[y][j] = z_cur;
                        auto m_point = Matrix(orig_p_0_2);
                        Matrix to_light_coords = _to_light * m_point;
                        //int sx = to_light_coords[0][0] + width;
                        //int sy = to_light_coords[1][0] + height;
                        int sx = to_light_coords[0][0];
                        int sy = to_light_coords[1][0];
                        double sz = to_light_coords[2][0];
                        if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size())
                            std::cout << sx << " " << sy << " " << sz << std::endl;
                        if (sy >= _shade_buf.size() || sy < 0 || sx < 0 || sx >= _shade_buf[0].size() || _shade_buf[sy][sx] < sz)
                        {
                            auto pixel = Pixel(j, y, 0, 0.1);
                            _drawer->add_point(pixel, color);
                        }
                        else
                        {
                            auto pixel = Pixel(j, y, 0, i_cur);
                            _drawer->add_point(pixel, color);
                        }
                        //auto pixel = Pixel(j, y, 0, i_cur);
                        //_drawer->add_point(pixel, color);
                    }
                    z_cur += dz_x;
                    i_cur += di_x;
                    orig_p_0_2 += orig_dp_x;
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

