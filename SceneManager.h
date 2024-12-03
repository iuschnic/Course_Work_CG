#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "DrawAdapter.h"
#include <vector>
#include <iostream>

#define Ia 0.15
#define ka 1
#define Il 1
#define kd 0.4
#define ks 0.4
#define ndeg 5
#define K 1

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
        std::map<std::size_t, std::vector<std::vector<double>>> intensities = calc_intensities(_light, _camera);
        draw(intensities);
        //draw();
    }

    void draw_scene(std::shared_ptr<BaseDrawer> drawer)
    {
        //std::cout << "in draw_scene method\n";
        std::map<std::size_t, std::vector<std::vector<double>>> intensities = calc_intensities(_light, _camera);
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

    std::map<std::size_t, std::vector<std::vector<double>>> calc_intensities(const std::shared_ptr<PointLight> &light, const std::shared_ptr<PProjCamera> &camera)
    {
        std::map<std::size_t, std::vector<std::vector<double>>> intensities;
        auto l_center = light->get_center();
        auto c_center = camera->get_center();
        for (const auto &[id, obj]: _scene->_visible_objects)
        {
            intensities[id] = {};
            auto object = std::static_pointer_cast<Object>(obj);
            auto spheres = object->get_spheres();
            for (int i = 0; i < spheres.size(); i++)
            {
                auto s = spheres[i];
                auto s_center = s->get_center();
                auto points = s->get_points();
                std::vector<double> sphere_intensities;
                for (int i = 0; i < points.size(); i++)
                {
                    auto p = points[i];
                    //вектор нормали к сфере в данной точке
                    auto n_vec = p - s_center;
                    double n_mod = pow(pow(n_vec.get_x(), 2) + pow(n_vec.get_y(), 2) + pow(n_vec.get_z(), 2), 0.5);
                    //вектор от точки до источника света
                    auto l_vec = l_center - p;
                    double l_mod = pow(pow(l_vec.get_x(), 2) + pow(l_vec.get_y(), 2) + pow(l_vec.get_z(), 2), 0.5);
                    //вектор от точки до камеры
                    auto c_vec = c_center - p;
                    double c_mod = pow(pow(c_vec.get_x(), 2) + pow(c_vec.get_y(), 2) + pow(c_vec.get_z(), 2), 0.5);
                    //нормализованный вектор нормали
                    auto n_norm = n_vec;
                    n_norm.normalize();
                    //вектор падающего луча
                    auto f_vec = l_vec * (-1);
                    //вектор отражения
                    double scalar_f_n = f_vec.get_x() * n_norm.get_x() + f_vec.get_y() * n_norm.get_y() + f_vec.get_z() * n_norm.get_z();
                    auto r_vec = f_vec - n_norm * 2 * scalar_f_n;
                    double r_mod = pow(pow(r_vec.get_x(), 2) + pow(r_vec.get_y(), 2) + pow(r_vec.get_z(), 2), 0.5);
                    //double dist = pow(pow(p.get_x() - l_center.get_x(), 2) + pow(p.get_y() - l_center.get_y(), 2) + pow(p.get_z() - l_center.get_z(), 2), 0.5);
                    double cos_teta = (n_vec.get_x() * l_vec.get_x() + n_vec.get_y() * l_vec.get_y() + n_vec.get_z() * l_vec.get_z()) / (n_mod * l_mod);
                    double cos_alpha = (r_vec.get_x() * c_vec.get_x() + r_vec.get_y() * c_vec.get_y() + r_vec.get_z() * c_vec.get_z()) / (r_mod * c_mod);
                    if (cos_alpha < 0)
                        cos_alpha = 0;
                    if (cos_teta < 0)
                        cos_teta = 0;
                    double intensity = Ia * ka + Il * (kd * cos_teta + ks * pow(cos_alpha, ndeg));
                    sphere_intensities.push_back(intensity);
                    //std::cout << "I " << intensity << std::endl;
                }
                intensities[id].push_back(sphere_intensities);
            }
        }
        return intensities;
    }
};

#endif // SCENEMANAGER_H
