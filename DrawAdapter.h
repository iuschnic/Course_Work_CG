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
    void set_adaptee(std::shared_ptr<Object> &adaptee) {_adaptee = adaptee;}
    void set_camera(std::shared_ptr<BaseCamera> &camera) {_camera = camera;}
    void set_drawer(std::shared_ptr<BaseDrawer> &drawer) {_drawer = drawer;}
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
            for (const auto &sphere : _adaptee->get_spheres())
            {
                std::vector<Point> points = sphere->get_points();
                std::vector<std::vector<int>> faces = sphere->get_faces();
                for (const auto &face : faces)
                {
                    //std::cout << "link\n";
                    for (int i = 0; i < face.size() - 1; i++)
                    {
                        _drawer->draw_line(get_projection(points[i]),
                                           get_projection(points[i + 1]));
                    }
                    _drawer->draw_line(get_projection(points[0]),
                                       get_projection(points[face.size() - 1]));
                }
            }
        }
    }
private:
    std::shared_ptr<BaseCamera> _camera;
    std::shared_ptr<BaseDrawer> _drawer;
    std::shared_ptr<Object> _adaptee;
    Point get_projection(const Point &point)
    {
        return _camera->get_projection(point);
    }
};

#endif // DRAW_ADAPTER_H

