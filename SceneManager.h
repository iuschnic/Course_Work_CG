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
        _light = nullptr;
        _scene = std::make_shared<Scene>(Scene());
        _adapter = std::make_shared<DrawAdapter>(DrawAdapter());
        _sim_flag = 0;
    }
    ~SceneManager() = default;

    void set_main_camera(const std::size_t cam_id)
    {
        std::shared_ptr<InvisibleObject> cam = _scene->get_camera(cam_id);
        auto cam_pnt = std::dynamic_pointer_cast<PProjCamera>(cam);
        _camera = cam_pnt;
        //_scene->set_cam_index(cam_id);
    }
    std::shared_ptr<PProjCamera> get_main_camera() {return _camera;}
    void set_main_light(const std::size_t light_id)
    {
        std::shared_ptr<InvisibleObject> light = _scene->get_light(light_id);
        auto light_pnt = std::dynamic_pointer_cast<PointLight>(light);
        _light = light_pnt;
    }
    std::shared_ptr<PointLight> get_main_light() {return _light;}
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

    void sim_iteration(std::shared_ptr<BaseDrawer> drawer)
    {
        //std::map<std::size_t, std::vector<std::vector<double>>> intensities = _scene->calc_intensities(_light);
        _adapter->set_drawer(drawer);
        _adapter->set_camera(_camera);
        _adapter->clear();
        _scene->sim_iteration();
        std::map<std::size_t, std::vector<std::vector<double>>> intensities = _scene->calc_intensities(_light, _camera);
        draw(intensities);
        //draw();
    }

    void draw_scene(std::shared_ptr<BaseDrawer> drawer)
    {
        //std::cout << "in draw_scene method\n";
        std::map<std::size_t, std::vector<std::vector<double>>> intensities = _scene->calc_intensities(_light, _camera);
        _adapter->set_drawer(drawer);
        _adapter->set_camera(_camera);
        draw(intensities);
        //draw();
    }

    void clear_graphics_scene(std::shared_ptr<BaseDrawer> drawer)
    {
        //_scene->_visible_objects.clear();
        //auto drawer = _adapter->get_drawer();
        //std::cout << "clearing graphics scene\n";
        drawer->clear();
    }

    void clear_scene()
    {
        _scene->_visible_objects.clear();
    }


private:
    std::shared_ptr<PProjCamera> _camera;
    std::shared_ptr<PointLight> _light;
    std::shared_ptr<Scene> _scene;
    std::shared_ptr<DrawAdapter> _adapter;
    int _sim_flag;

    void draw(std::map<std::size_t, std::vector<std::vector<double>>> &intensities)
    {
        //std::cout << "in draw method\n";
        for (const auto &kv : _scene->_visible_objects)
        {
            auto i = intensities[kv.first];
            auto obj_ptr = std::dynamic_pointer_cast<Object>(kv.second);
            //std::cout << obj_ptr->get_mass() << std::endl;
            _adapter->set_adaptee(obj_ptr);
            _adapter->set_adaptee_intensities(i);
            //std::cout << i << std::endl;
            _adapter->request();
        }
        _adapter->draw();
    }
};

#endif // SCENEMANAGER_H
