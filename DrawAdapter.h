#ifndef DRAW_ADAPTER_H
#define DRAW_ADAPTER_H

#include <iostream>
#include <limits>
#include "QTDrawer.h"
#include "PerspectiveProjCamera.h"
#include "PointLight.h"
#include "Object.h"
#define Ia 0.1
#define ka 1

class DrawAdapter
{
public:
    DrawAdapter() = default;
    ~DrawAdapter() = default;
    void set_adaptee(std::shared_ptr<Object> adaptee) {_adaptee = adaptee;}
    void set_camera(std::shared_ptr<BaseCamera> camera) {_camera = camera;}
    void set_light(std::shared_ptr<PointLight> light) {_light = light;}
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
    std::shared_ptr<PointLight> get_light() {return _light;}
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
                Point cam_sphere = sphere->get_center() - _camera->get_center();
                Point dir = _camera->get_direction();
                double scalar = dir.get_x() * cam_sphere.get_x() + dir.get_y() * cam_sphere.get_y() + dir.get_z() * cam_sphere.get_z();
                if (scalar < 0)
                    continue;
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
                    Point s_center = sphere->get_center();
                    Point c_cen = _camera->get_center();
                    Point p0 = points[face[0]];
                    Point p1 = points[face[1]];
                    Point p2 = points[face[2]];
                    Point n_vec = (p0 - s_center) + (p1 - s_center) + (p2 - s_center);
                    Point c_vec = p0 - c_cen;
                    double scalar = c_vec.get_x() * n_vec.get_x() + c_vec.get_y() * n_vec.get_y() + c_vec.get_z() * n_vec.get_z();
                    if (scalar > 0)
                        continue;
                    if (face.size() == 3)
                    {
                        triangle(points[face[0]], points[face[1]], points[face[2]], color, intensities[face[0]], intensities[face[1]], intensities[face[2]]);
                    }
                    else if (face.size() == 4)
                    {
                        triangle(points[face[1]], points[face[2]], points[face[3]], color, intensities[face[1]], intensities[face[2]], intensities[face[3]]);
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
    void set_spheres(std::vector<std::pair<Point, double>> &spheres)
    {
        _spheres = spheres;
    }
private:
    std::shared_ptr<BaseCamera> _camera;
    std::shared_ptr<PointLight> _light;
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Object> _adaptee;
    std::vector<std::vector<double>> _z_buf;
    std::vector<std::vector<double>> _intensities;
    std::vector<std::pair<Point, double>> _spheres;
    Pixel get_projection(const Point &point)
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

    bool is_shaded(const Point &pnt)
    {
        Point l_center = _light->get_center();
        //вектор из точки к источнику света
        Point pnt_l = l_center - pnt;
        for (auto &pair: _spheres)
        {
            //Если источник находится внутри любой из сфер, все точки затенены
            Point l_s = l_center - pair.first;
            double l_s_mod = pow(pow(l_s.get_x(), 2) + pow(l_s.get_y(), 2) + pow(l_s.get_z(), 2), 0.5);
            if (l_s_mod < pair.second)
                return true;
            //вектор из точки в центр сферы, радиус сферы
            Point pnt_s = pair.first - pnt;
            double r_s = pair.second;
            //если эти вектора противонаправлены, то сфера находится за точкой и тени точно на нее не отбрасывает
            double scalar = pnt_s.get_x() * pnt_l.get_x() + pnt_s.get_y() * pnt_l.get_y() + pnt_s.get_z() * pnt_l.get_z();
            if (scalar < 0)
                continue;
            //ищем расстояние от центра сферы до прямой точка-свет, если оно меньше радиуса, то сфера отбрасывает на точку тень
            //модуль направляющего вектора прямой точка-свет
            //векторное произведение направляющего вектора и вектора центр_сферы-точка
            Point vec_prod = pnt_s * pnt_l * -1;
            double vec_prod_mod = pow(pow(vec_prod.get_x(), 2) + pow(vec_prod.get_y(), 2) + pow(vec_prod.get_z(), 2), 0.5);
            double pnt_l_mod = pow(pow(pnt_l.get_x(), 2) + pow(pnt_l.get_y(), 2) + pow(pnt_l.get_z(), 2), 0.5);
            double d = vec_prod_mod / pnt_l_mod;
            if (d < r_s)
                return true;
        }
        return false;
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

    void triangle(Point p0, Point p1, Point p2, QColor &color, double &i0, double &i1, double &i2) {
        int size_y = (int) _z_buf.size();
        int size_x = (int) (_z_buf[0]).size();
        Pixel t0 = get_projection(p0);
        Pixel t1 = get_projection(p1);
        Pixel t2 = get_projection(p2);
        int height = (int) _drawer->get_height() / 2;
        int width = (int) _drawer->get_width() / 2;
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
                if ((t0.get_y() > 0) && (j > 0) && (t0.get_y() < size_y) && (j < size_x))
                    if (_z_buf[t0.get_y()][j] > z) {
                        _z_buf[t0.get_y()][j] = z;
                        if (is_shaded(cur))
                        {
                            auto pixel = Pixel(j, t0.get_y(), z, Ia * ka);
                            _drawer->add_point(pixel, color);
                        }
                        else
                        {
                            auto pixel = Pixel(j, t0.get_y(), z, i);
                            _drawer->add_point(pixel, color);
                        }
                    }
                z += dz1;
                i += di1;
                cur += dcur;
            }
            dcur = Point(orig_dx2, orig_dy2, orig_dz2);
            for (int j = t1.get_x(); j <= t2.get_x(); j++) {
                if ((t0.get_y() > 0) && (j > 0) && (t0.get_y() < size_y) && (j < size_x))
                    if (_z_buf[t0.get_y()][j] > z) {
                        _z_buf[t0.get_y()][j] = z;
                        if (is_shaded(cur))
                        {
                            auto pixel = Pixel(j, t0.get_y(), z, Ia * ka);
                            _drawer->add_point(pixel, color);
                        }
                        else
                        {
                            auto pixel = Pixel(j, t0.get_y(), z, i);
                            _drawer->add_point(pixel, color);
                        }
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

        double di_y_0_1 = (t1.get_intensity() - t0.get_intensity()) / (t1.get_y() - t0.get_y() + 1);
        double di_y_0_2 = (t2.get_intensity() - t0.get_intensity()) / (t2.get_y() - t0.get_y() + 1);
        double z0 = t0.get_z(); //начальное значение z
        double in = t0.get_intensity();
        double z_0_1 = z0; //значение z на отрезке 0-1
        double z_0_2 = z0; //значение z на отрезке 0-2
        double i_0_1 = in;
        double i_0_2 = in;
        int total_height = t2.get_y() - t0.get_y() + 1;
        int segment_height = t1.get_y() - t0.get_y() + 1;
        for (int y = t0.get_y(); y <= t1.get_y(); y++) {
            double alpha = (double)(y - t0.get_y()) / total_height;
            double beta  = (double)(y - t0.get_y()) / segment_height; // be careful with divisions by zero
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 0-2
            Pixel p_0_1 = t0 + (t1 - t0) * beta; //Точка на отрезке 0-1
            Point orig_p_0_2 = p0 + (p2 - p0) * alpha;
            Point orig_p_0_1 = p0 + (p1 - p0) * beta;
            if (p_0_2.get_x() > p_0_1.get_x())
            {
                double z_cur = z_0_1;
                double i_cur = i_0_1;
                double dz_x = (z_0_2 - z_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                double di_x = (i_0_2 - i_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                Point orig_dp_x = (orig_p_0_2 - orig_p_0_1) / (p_0_2.get_x() - p_0_1.get_x() + 1);
                for (int j = p_0_1.get_x(); j <= p_0_2.get_x(); j++) {
                    if ((y > 0) && (j > 0) && (y < size_y) && (j < size_x))
                        if (_z_buf[y][j] > z_cur) {
                            _z_buf[y][j] = z_cur;
                            if (is_shaded(orig_p_0_1))
                            {
                                auto pixel = Pixel(j, y, 0, Ia * ka);
                                _drawer->add_point(pixel, color);
                            }
                            else
                            {
                                auto pixel = Pixel(j, y, 0, i_cur);
                                _drawer->add_point(pixel, color);
                            }
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
                    if ((y > 0) && (j > 0) && (y < size_y) && (j < size_x))
                        if (_z_buf[y][j] > z_cur) {
                            _z_buf[y][j] = z_cur;
                            if (is_shaded(orig_p_0_2))
                            {
                                auto pixel = Pixel(j, y, 0, Ia * ka);
                                _drawer->add_point(pixel, color);
                            }
                            else
                            {
                                auto pixel = Pixel(j, y, 0, i_cur);
                                _drawer->add_point(pixel, color);
                            }
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
            double beta  = (double)(y - t1.get_y()) / segment_height;
            Pixel p_0_2 = t0 + (t2 - t0) * alpha; //Точка на отрезке 2-0
            Pixel p_1_2 = t1 + (t2 - t1) * beta; //Точка на отрезке 2-1
            Point orig_p_0_2 = p0 + (p2 - p0) * alpha;
            Point orig_p_1_2 = p1 + (p2 - p1) * beta;
            if (p_0_2.get_x() > p_1_2.get_x())
            {
                double z_cur = z_1_2;
                double i_cur = i_1_2;
                double dz_x = (z_0_2 - z_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                double di_x = (i_0_2 - i_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                Point orig_dp_x = (orig_p_0_2 - orig_p_1_2) / (p_0_2.get_x() - p_1_2.get_x() + 1);
                for (int j = p_1_2.get_x(); j <= p_0_2.get_x(); j++) {
                    if ((y > 0) && (j > 0) && (y < size_y) && (j < size_x))
                        if (_z_buf[y][j] >= z_cur) {
                            _z_buf[y][j] = z_cur;
                            if (is_shaded(orig_p_1_2))
                            {
                                auto pixel = Pixel(j, y, 0, Ia * ka);
                                _drawer->add_point(pixel, color);
                            }
                            else
                            {
                                auto pixel = Pixel(j, y, 0, i_cur);
                                _drawer->add_point(pixel, color);
                            }
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
                    if ((y > 0) && (j > 0) && (y < size_y) && (j < size_x))
                        if (_z_buf[y][j] >= z_cur) {
                            _z_buf[y][j] = z_cur;
                            if (is_shaded(orig_p_0_2))
                            {
                                auto pixel = Pixel(j, y, 0, Ia * ka);
                                _drawer->add_point(pixel, color);
                            }
                            else
                            {
                                auto pixel = Pixel(j, y, 0, i_cur);
                                _drawer->add_point(pixel, color);
                            }
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

