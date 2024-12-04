#ifndef SCENE_H
#define SCENE_H

#include "PerspectiveProjCamera.h"
#include "PointLight.h"
#include "Point.h"
#include "VisibleObject.h"
#include "InvisibleObject.h"
#include "Object.h"
#include <map>
#include <memory>
#include <iostream>
#include <stdio.h>
#include <vector>

#define G 10
#define TIME 1

class Scene
{
    friend class SceneManager;
public:
    Scene() = default;
    ~Scene() = default;
    std::size_t add_object(const std::shared_ptr<VisibleObject> &object)
    {
        _visible_objects[object->get_id()] = object;
        return object->get_id();
    }
    std::size_t add_camera(const Point &center, const Point &direction, const Point &up)
    {
        auto cam = std::make_shared<PProjCamera>(PProjCamera(center, direction, up));
        _invisible_objects.insert({cam->get_id(), cam});
        return cam->get_id();
    }
    std::size_t add_light(const Point &center, const Point &direction, const Point &up)
    {
        auto light = std::make_shared<PointLight>(PointLight(center, direction, up));
        _invisible_objects.insert({light->get_id(), light});
        return light->get_id();
    }
    void delete_object(const std::size_t &id_object)
    {
        _visible_objects.erase(id_object);
    }
    void delete_camera(const std::size_t &id_camera)
    {
        _invisible_objects.erase(id_camera);
    }
    void delete_light(const std::size_t &id_light)
    {
        _invisible_objects.erase(id_light);
    }

    std::shared_ptr<VisibleObject> get_object(const std::size_t &id_object)
    {
        return _visible_objects[id_object];
    }
    std::shared_ptr<InvisibleObject> get_camera(const std::size_t &id_camera)
    {
        return _invisible_objects[id_camera];
    }
    std::shared_ptr<InvisibleObject> get_light(const std::size_t &id_light)
    {
        return _invisible_objects[id_light];
    }

    void set_cam_index(std::size_t index)
    {
        _index = index;
    }

    std::size_t get_cam_index() const
    {
        return _index;
    }

    std::vector<std::size_t> get_visible_index()
    {
        std::vector<std::size_t> visible;
        for (auto &object: _visible_objects)
            visible.push_back(object.first);
        return visible;
    }

    std::vector<std::size_t> get_invisible_index()
    {
        std::vector<std::size_t> invisible;
        for (auto &object: _invisible_objects)
            invisible.push_back(object.first);
        return invisible;
    }

    std::map<std::size_t, Point> calc_accelerations() const
    {
        std::map<std::size_t, Point> accelerations;
        for (const auto &[id_fir, obj_fir]: _visible_objects)
        {
            auto fir = std::static_pointer_cast<Object>(obj_fir);
            Point accel(0, 0, 0);
            double mass_fir = fir->get_mass();
            Point mass_center_fir = fir->get_mass_center();
            for (auto &[id_sec, obj_sec]: _visible_objects)
            {
                auto sec = std::static_pointer_cast<Object>(obj_sec);
                if (id_fir != id_sec)
                {
                    double mass_sec = sec->get_mass();
                    Point mass_center_sec = sec->get_mass_center();
                    Point vec = mass_center_sec;
                    vec -= mass_center_fir;
                    double r = pow((pow(vec.get_x(), 2) + pow(vec.get_y(), 2) + pow(vec.get_z(), 2)), 0.5);
                    accel += vec * (G * (mass_sec / pow(r, 3))) * (mass_fir < 0 ? -1 : 1);
                }
            }
            accelerations[id_fir] = accel;
        }
        return accelerations;
    }

    //Функция подсчета скорости, полученной засчет ускорения за малое время time
    /*std::map<std::size_t, Point> calc_speeds(const double time, const std::map<std::size_t, Point> &accelerations) const
    {
        std::map<std::size_t, Point> speeds;
        for (const auto &[id_fir, pnt_fir]: accelerations)
        {
            Point speed(pnt_fir);
            speed *= time;
            speeds[id_fir] = speed;
        }
        return speeds;
    }*/

    /*std::map<std::size_t, std::vector<std::vector<double>>> calc_intensities(const std::shared_ptr<PointLight> &light, const std::shared_ptr<PProjCamera> &camera)
    {
        std::map<std::size_t, std::vector<std::vector<double>>> intensities;
        auto l_center = light->get_center();
        auto c_center = camera->get_center();
        for (const auto &[id, obj]: _visible_objects)
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
                    //auto d_vec = l_vec * (-1);
                    auto r_vec = l_vec - (l_vec * n_norm) * n_norm * 2;
                    double r_mod = pow(pow(r_vec.get_x(), 2) + pow(r_vec.get_y(), 2) + pow(r_vec.get_z(), 2), 0.5);
                    double dist = pow(pow(p.get_x() - l_center.get_x(), 2) + pow(p.get_y() - l_center.get_y(), 2) + pow(p.get_z() - l_center.get_z(), 2), 0.5);
                    double cos_teta = (n_vec.get_x() * l_vec.get_x() + n_vec.get_y() * l_vec.get_y() + n_vec.get_z() * l_vec.get_z()) / (n_mod * l_mod);
                    //std::cout << "cos teta " << cos_teta << std::endl;
                    double cos_alpha = (r_vec.get_x() * n_vec.get_x() + r_vec.get_y() * n_vec.get_y() + r_vec.get_z() * n_vec.get_z()) / (r_mod * n_mod);
                    //std::cout << "cos alpha " << cos_alpha << std::endl;
                    //double intensity = Ia * ka + (Il * kd * cos_teta) / (K + dist / 100);
                    //double intensity = Ia * ka + (Il * kd * cos_teta);
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
    }*/

    //Метод для обработки столкновений объектов в сцене
    void process_collisions()
    {

        while (true)
        {
            int flag = 0;
            std::vector<size_t> obj_id_vec = get_visible_index();
            for (size_t i = 0; i < obj_id_vec.size(); i++)
            {
                auto fir = std::static_pointer_cast<Object>(_visible_objects[obj_id_vec[i]]);
                for (size_t j = i + 1; j < obj_id_vec.size(); j++)
                {
                    //Для каждого i-го объекта в его массив заносятся id объектов, пересекающихся с ним
                    auto sec = std::static_pointer_cast<Object>(_visible_objects[obj_id_vec[j]]);
                    if (fir->check_intersection(sec) == true)
                    {
                        std::cout << "FOUND\n";
                        flag = 1;
                        fir->add_object(sec);
                        _visible_objects.erase(obj_id_vec[j]);
                        break;
                    }
                }
                if (flag)
                    break;
            }
            if (!flag)
                break;
        }
        /*std::vector<std::vector<size_t>> collisions;
        std::vector<size_t> obj_id_vec = get_visible_index();
        int flag = 0;
        for (size_t i = 0; i < obj_id_vec.size(); i++)
        {
            auto fir = std::static_pointer_cast<Object>(_visible_objects[obj_id_vec[i]]);
            collisions.push_back({});
            for (size_t j = i + 1; j < obj_id_vec.size(); j++)
            {
                //Для каждого i-го объекта в его массив заносятся id объектов, пересекающихся с ним
                auto sec = std::static_pointer_cast<Object>(_visible_objects[obj_id_vec[j]]);
                if (fir->check_intersection(sec) == true)
                {
                    std::cout << "FOUND\n";
                    flag = 1;
                    collisions[i].push_back(obj_id_vec[j]);
                }
            }
        }
        if (flag)
        {
            for (auto &el: obj_id_vec)
                std::cout << el << std::endl;
            std::cout << "collisions\n";
            for (auto &el: collisions)
            {
                for (auto e: el)
                {
                    std::cout << e << " ";
                }
                std::cout << std::endl;
            }
        }
        if (collisions.empty())
            return;*/




        //std::cout << "collisions\n";
        /* Допустим получили такой map:
             * 1 -> 2, 3
             * 2 -> 3, 4
             * 4 -> 5
             * 6 -> 7
             * Первой итерацией цикла к 1 добавялся 4 и 5, элементы map с ключами 2, 4 удалятся
             * 1 -> 2, 3, 4, 5
             * 6 -> 7
             * На второй итерации цикл завершится по флагу, в итоге получим валидные списки тел, которые нужно объединять
             * */
        /*while (true)
        {
            int flag = 0;
            for (int i = 0; i < obj_id_vec.size(); i++)
            {
                //auto data = collisions[i];
                flag = 0;
                for (auto &intersected: collisions[i])
                {
                    //Если у тела, пересеченного i-м есть еще пересеченные тела, добавляем к i-му
                    if (!collisions[intersected].empty())
                    {
                        for (size_t x: collisions[intersected])
                        {
                            //Чтобы не было повторов
                            if (std::count(collisions[i].begin(), collisions[i].end(), x) == 0)
                                collisions[i].push_back(x);
                        }
                        collisions[intersected].clear();
                        flag = 1;
                    }
                }
                if (flag == 1)
                    break;
            }
            if (flag == 0)
                break;
        }
        if (collisions.empty())
            return;
        //Соединяем все получившиеся объекты
        for (int i = 0; i < obj_id_vec.size(); i++)
        {
            if (collisions[i].empty())
                continue;
            auto obj = std::static_pointer_cast<Object>(get_object(obj_id_vec[i]));
            for (size_t id2: collisions[i])
            {
                auto obj2 = std::static_pointer_cast<Object>(get_object(id2));
                obj->add_object(obj2);
                _visible_objects.erase(id2);
            }
        }*/
    }
    void sim_iteration()
    {
        process_collisions();
        std::map<std::size_t, Point> accel = calc_accelerations();
        for (auto &[id, obj]: _visible_objects)
        {
            auto object = std::static_pointer_cast<Object>(obj);
            Point speed(accel[id]);
            speed *= TIME;
            object->add_speed(speed);
            speed = object->get_speed();
            speed *= TIME;
            //std::cout << "moving " << speed.get_x() << " " << speed.get_y() << " " << speed.get_z();
            obj->move(speed.get_x(), speed.get_y(), speed.get_z());
        }
    }

private:
    std::map<std::size_t, std::shared_ptr<VisibleObject>> _visible_objects;
    std::map<std::size_t, std::shared_ptr<InvisibleObject>> _invisible_objects;
    std::size_t _index;
};

#endif // SCENE_H

