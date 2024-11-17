#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "DrawAdapter.h"
#include <vector>
#include <iostream>

class SceneManager
{
public:
    SceneManager()
    {
        _camera = nullptr;
        _scene = std::make_shared<Scene>(Scene());
        _sim_flag = 0;
    }
    ~SceneManager() = default;

    void set_main_camera(const std::size_t cam_id)
    {
        std::shared_ptr<InvisibleObject> cam = _scene->get_camera(cam_id);
        auto cam_pnt = std::dynamic_pointer_cast<PProjCamera>(cam);
        _camera = cam_pnt;
        _scene->set_cam_index(cam_id);
    }
    std::shared_ptr<PProjCamera> get_main_camera() {return _camera;}
    std::size_t add_object(std::shared_ptr<VisibleObject> &object)
    {
        return _scene->add_object(object);
    }
    std::size_t add_camera(const Point &location)
    {
        return _scene->add_camera(location);
    }
    std::size_t add_light(const Point &location)
    {
        return _scene->add_light(location);
    }
    void delete_object(const std::size_t &id_object)
    {
        _scene->delete_object(id_object);
    }
    void delete_camera(const std::size_t &id_camera)
    {
        if (_camera->get_id() == id_camera)
            _camera = nullptr;
        _scene->delete_camera(id_camera);
    }
    void delete_light(const std::size_t &id_light)
    {
        _scene->delete_light(id_light);
    }

    void start_simulation(std::shared_ptr<BaseDrawer> &drawer)
    {
        _adapter->set_drawer(drawer);
        _adapter->set_camera(_camera);
        _sim_flag = 1;
        while (_sim_flag)
        {
            _scene->sim_iteration();
            draw_scene();
        }
    }

    void stop_simulation()
    {
        _sim_flag = 0;
    }

    void draw_scene()
    {
        for (const auto &kv : scene->_visible_objects)
        {
            auto obj_ptr = std::dynamic_pointer_cast<Object>(kv.second);
            _adapter->set_adaptee(obj_ptr);
            _adapter->request();
        }
    }

    void clear_graphics_scene()
    {
        //_scene->_visible_objects.clear();
        auto drawer = _adapter->get_drawer();
        drawer->clear_scene();
    }

    void clear_scene()
    {
        _scene->_visible_objects.clear();
    }


private:
    std::shared_ptr<PProjCamera> _camera;
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<DrawAdapter> _adapter;
    int _sim_flag;
};

#endif // SCENEMANAGER_H
