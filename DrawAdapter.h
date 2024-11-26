#ifndef DRAW_ADAPTER_H
#define DRAW_ADAPTER_H

#include "QTDrawer.h"
#include "PerspectiveProjCamera.h"
#include "Object.h"
#include <iostream>

class DrawAdapter
{
public:
    DrawAdapter() = default;
    ~DrawAdapter() = default;
    void set_adaptee(std::shared_ptr<Object> adaptee) {_adaptee = adaptee;}
    void set_camera(std::shared_ptr<BaseCamera> camera) {_camera = camera;}
    void set_drawer(std::shared_ptr<BaseDrawer> drawer) {_drawer = drawer;}
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

        if (_adaptee && _camera && _drawer)
        {
            //std::cout << "OK\n";
            for (const auto &sphere : _adaptee->get_spheres())
            {
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

                        triangle(t0, t1, t2);
                    }
                    else if(face.size() == 4)
                    {
                        Point t0 = get_projection(points[face[1]]);
                        Point t1 = get_projection(points[face[2]]);
                        Point t2 = get_projection(points[face[3]]);
                        /*std::cout << "FACE\n";
                        std::cout << face[1] << " " << face[2] << " " << face[3] << std::endl;
                        std::cout << t0.get_x() << " " << t0.get_y() << " " << t0.get_z() << std::endl;
                        std::cout << t1.get_x() << " " << t1.get_y() << " " << t1.get_z() << std::endl;
                        std::cout << t2.get_x() << " " << t2.get_y() << " " << t2.get_z() << std::endl;*/
                        triangle(t0, t1, t2);
                        t0 = get_projection(points[face[0]]);
                        t1 = get_projection(points[face[1]]);
                        t2 = get_projection(points[face[3]]);
                        /*std::cout << "FACE\n";
                        std::cout << face[0] << " " << face[1] << " " << face[3] << std::endl;
                        std::cout << t0.get_x() << " " << t0.get_y() << " " << t0.get_z() << std::endl;
                        std::cout << t1.get_x() << " " << t1.get_y() << " " << t1.get_z() << std::endl;
                        std::cout << t2.get_x() << " " << t2.get_y() << " " << t2.get_z() << std::endl;*/
                        triangle(t0, t1, t2);
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
    Point get_projection(const Point &point)
    {
        return _camera->get_projection(point);
    }
    void triangle(Point t0, Point t1, Point t2) {
        if (t0.get_y() == t1.get_y() && t0.get_y() == t2.get_y())
        {
            for (int j = std::min(t0.get_x(), std::min(t1.get_x(), t2.get_x())); j <= std::max(t0.get_x(), std::max(t1.get_x(), t2.get_x())); j++) {
                //image.set(j, t0.get_y()+i, color); // attention, due to int casts t0.get_y()+i != A.get_y()
                auto pixel = Point(j, t0.get_y(), 0);
                _drawer->addPoint(pixel, QColor(255, 255, 255));
            }
            //_drawer->addPoint(t0, QColor(255, 255, 255));
            return; // i dont care about degenerate triangles
        }
        // sort the vertices, t0, t1, t2 lower-to-upper (bubblesort yay!)
        if (t0.get_y() > t1.get_y()) std::swap(t0, t1);
        if (t0.get_y() > t2.get_y()) std::swap(t0, t2);
        if (t1.get_y() > t2.get_y()) std::swap(t1, t2);
        int total_height = t2.get_y() - t0.get_y();
        for (int i = 0; i < total_height; i++) {
            bool second_half = i > t1.get_y() - t0.get_y() || t1.get_y() == t0.get_y();
            int segment_height = second_half ? t2.get_y() - t1.get_y() : t1.get_y() - t0.get_y();
            float alpha = (float) i / total_height;
            float beta  = (float) (i - (second_half ? t1.get_y() - t0.get_y() : 0)) / segment_height; // be careful: with above conditions no division by zero here
            Point A = t0 + (t2 - t0) * alpha;
            Point B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
            if (A.get_x() > B.get_x()) std::swap(A, B);
            for (int j = A.get_x(); j <= B.get_x(); j++) {
                //image.set(j, t0.get_y()+i, color); // attention, due to int casts t0.get_y()+i != A.get_y()
                auto pixel = Point(j, t0.get_y() + i, 0);
                _drawer->addPoint(pixel, QColor(255, 255, 255));
            }
        }
    }
};

#endif // DRAW_ADAPTER_H

