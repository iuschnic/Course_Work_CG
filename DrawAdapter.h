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
        size_t height = drawer->get_height();
        size_t width = drawer->get_width();
        for (size_t i = 0; i < height; i++)
        {
            _z_buf.push_back({});
            for (size_t j = 0; j < width; j++)
            {
                _z_buf[i].push_back(std::numeric_limits<double>::max());
            }
        }
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
            for (const auto &sphere : _adaptee->get_spheres())
            {
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
                        /*std::cout << "FACE\n";
                        std::cout << face[0] << " " << face[1] << " " << face[2] << std::endl;
                        std::cout << t0.get_x() << " " << t0.get_y() << " " << t0.get_z() << std::endl;
                        std::cout << t1.get_x() << " " << t1.get_y() << " " << t1.get_z() << std::endl;
                        std::cout << t2.get_x() << " " << t2.get_y() << " " << t2.get_z() << std::endl;*/
                        Pixel p0 = Pixel(t0);
                        Pixel p1 = Pixel(t1);
                        Pixel p2 = Pixel(t2);
                        triangle(p0, p1, p2, color);
                    }
                    else if(face.size() == 4)
                    {
                        Point t0 = get_projection(points[face[1]]);
                        Point t1 = get_projection(points[face[2]]);
                        Point t2 = get_projection(points[face[3]]);
                        Pixel p0 = Pixel(t0);
                        Pixel p1 = Pixel(t1);
                        Pixel p2 = Pixel(t2);
                        /*std::cout << "FACE\n";
                        std::cout << face[1] << " " << face[2] << " " << face[3] << std::endl;
                        std::cout << t0.get_x() << " " << t0.get_y() << " " << t0.get_z() << std::endl;
                        std::cout << t1.get_x() << " " << t1.get_y() << " " << t1.get_z() << std::endl;
                        std::cout << t2.get_x() << " " << t2.get_y() << " " << t2.get_z() << std::endl;*/
                        triangle(p0, p1, p2, color);
                        t0 = get_projection(points[face[0]]);
                        t1 = get_projection(points[face[1]]);
                        t2 = get_projection(points[face[3]]);
                        p0 = Pixel(t0);
                        p1 = Pixel(t1);
                        p2 = Pixel(t2);
                        /*std::cout << "FACE\n";
                        std::cout << face[0] << " " << face[1] << " " << face[3] << std::endl;
                        std::cout << t0.get_x() << " " << t0.get_y() << " " << t0.get_z() << std::endl;
                        std::cout << t1.get_x() << " " << t1.get_y() << " " << t1.get_z() << std::endl;
                        std::cout << t2.get_x() << " " << t2.get_y() << " " << t2.get_z() << std::endl;*/
                        triangle(p0, p1, p2, color);
                    }

                }
            }
        }
    }
    void draw()
    {
        _drawer->draw();
    }
private:
    std::shared_ptr<BaseCamera> _camera;
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Object> _adaptee;
    std::vector<std::vector<double>> _z_buf;
    Point get_projection(const Point &point)
    {
        return _camera->get_projection(point);
    }
    void triangle(Pixel &t0, Pixel &t1, Pixel &t2, QColor &color) {
        if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y())
        {
            if (t0.get_x() > t1.get_x()) std::swap(t0, t1);
            if (t0.get_x() > t2.get_x()) std::swap(t0, t2);
            if (t1.get_x() > t2.get_x()) std::swap(t1, t2);
            double dz = (t2.get_z() - t0.get_z()) / (t2.get_x() - t0.get_x() + 1);
            double z = t0.get_z();
            for (int j = t0.get_x(); j <= t2.get_x(); j++) {
                if (_z_buf[t0.get_y()][j] >= z) {
                    _z_buf[t0.get_y()][j] = z;
                    auto pixel = Pixel(j, t0.get_y(), z);
                    _drawer->add_point(pixel, color);
                }
                z += dz;
            }
            return; // i dont care about degenerate triangles
        }
        /*// sort the vertices, t0, t1, t2 lower-to-upper (bubblesort yay!)
        if (t0.get_y() > t1.get_y()) std::swap(t0, t1);
        if (t0.get_y() > t2.get_y()) std::swap(t0, t2);
        if (t1.get_y() > t2.get_y()) std::swap(t1, t2);
        int total_height = t2.get_y() - t0.get_y();
        for (int i = 0; i < total_height; i++) {
            bool second_half = i > t1.get_y() - t0.get_y() || t1.get_y() == t0.get_y();
            int segment_height = second_half ? t2.get_y() - t1.get_y() : t1.get_y() - t0.get_y();
            float alpha = (float) i / total_height;
            float beta  = (float) (i - (second_half ? t1.get_y() - t0.get_y() : 0)) / segment_height; // be careful: with above conditions no division by zero here
            Pixel A = t0 + (t2 - t0) * alpha;
            Pixel B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
            if (A.get_x() > B.get_x()) std::swap(A, B);
            for (int j = A.get_x(); j <= B.get_x(); j++) {
                //image.set(j, t0.get_y()+i, color); // attention, due to int casts t0.get_y()+i != A.get_y()
                auto pixel = Pixel(j, t0.get_y() + i, 0);
                _drawer->addPoint(pixel, color);
            }
        }*/




        /*// sort the vertices, t0, t1, t2 lower-to-upper (bubblesort yay!)
        if (t0.get_y() > t1.get_y()) std::swap(t0, t1);
        if (t0.get_y() > t2.get_y()) std::swap(t0, t2);
        if (t1.get_y() > t2.get_y()) std::swap(t1, t2);

        int total_height = t2.get_y() - t0.get_y();
        for (int y = t0.get_y(); y <= t1.get_y(); y++) {
            int segment_height = t1.get_y() - t0.get_y() + 1;
            float alpha = (float)(y - t0.get_y()) / total_height;
            float beta  = (float)(y - t0.get_y()) / segment_height; // be careful with divisions by zero
            Pixel A = t0 + (t2 - t0) * alpha;
            Pixel B = t0 + (t1 - t0) * beta;
            if (A.get_x() > B.get_x()) std::swap(A, B);
            for (int j = A.get_x(); j <= B.get_x(); j++) {
                //image.set(j, y, color); // attention, due to int casts t0.get_y()+i != A.get_y()
                auto pixel = Pixel(j, y, 0);
                _drawer->add_point(pixel, color);
            }
        }
        for (int y = t1.get_y(); y <= t2.get_y(); y++) {
            int segment_height = t2.get_y() - t1.get_y() + 1;
            float alpha = (float)(y - t0.get_y()) / total_height;
            float beta  = (float)(y - t1.get_y()) / segment_height; // be careful with divisions by zero
            Pixel A = t0 + (t2 - t0) * alpha;
            Pixel B = t1 + (t2 - t1) * beta;
            if (A.get_x() > B.get_x()) std::swap(A, B);
            for (int j = A.get_x(); j <= B.get_x(); j++) {
                //image.set(j, y, color); // attention, due to int casts t0.get_y()+i != A.get_y()
                auto pixel = Pixel(j, y, 0);
                _drawer->add_point(pixel, color);
            }
        }*/
        if (t0.get_y() > t1.get_y()) std::swap(t0, t1);
        if (t0.get_y() > t2.get_y()) std::swap(t0, t2);
        if (t1.get_y() > t2.get_y()) std::swap(t1, t2);
        int total_height = t2.get_y() - t0.get_y();
        for (int i = 0; i < total_height; i++) {
            bool second_half = i > t1.get_y() - t0.get_y() || t1.get_y() == t0.get_y();
            int segment_height = second_half ? t2.get_y() - t1.get_y() : t1.get_y() - t0.get_y();
            double alpha = (double) i / total_height;
            double beta  = (double) (i - (second_half ? t1.get_y() - t0.get_y() : 0)) / segment_height; // be careful: with above conditions no division by zero here
            //Pixel A = t0 + (t2 - t0) * alpha;
            Pixel A = t0 + Point(t2.get_x() - t0.get_x(), t2.get_y() - t0.get_y(), t2.get_z() - t0.get_z()) * alpha;
            //Pixel B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
            Pixel B = second_half ? t1 + Point(t2.get_x() - t1.get_x(), t2.get_y() - t1.get_y(), t2.get_z() - t1.get_z())
                * beta : t0 + Point(t1.get_x() - t0.get_x(), t1.get_y() - t0.get_y(), t1.get_z() - t0.get_z()) * beta;
            if (A.get_x() > B.get_x()) std::swap(A, B);
            for (int j = A.get_x(); j <= B.get_x(); j++) {
                double phi = B.get_x() == A.get_x() ? 1. : (double)(j - A.get_x()) / (double)(B.get_x() - A.get_x());
                //Pixel P = A + (B - A) * phi;
                Pixel P = Point(A.get_x(), A.get_y(), A.get_z()) + Point(B.get_x() - A.get_x(), B.get_y() - A.get_y(), B.get_z() - A.get_z()) * phi;
                //int idx = P.get_x() + P.get_y() * width;
                if (_z_buf[P.get_y()][P.get_x()] >= P.get_z()) {
                    _z_buf[P.get_y()][P.get_x()] = P.get_z();
                    //image.set(P.get_x(), P.get_y(), color);
                    _drawer->add_point(P, color);
                }
            }
        }
    }
};

#endif // DRAW_ADAPTER_H

