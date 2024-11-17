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

#define G 1
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
    std::size_t add_camera(const Point &location)
    {
        auto cam = std::make_shared<PProjCamera>(PProjCamera(location));
        _invisible_objects.insert({cam->get_id(), cam});
        return cam->get_id();
    }
    std::size_t add_light(const Point &location)
    {
        auto light = std::make_shared<PointLight>(PointLight(location));
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
            Point accel(0, 0, 0);
            double mass_fir = obj_fir->get_mass();
            Point mass_center_fir = obj_fir->get_mass_center();
            for (auto &[id_sec, obj_sec]: _visible_objects)
            {
                if (id_fir != id_sec)
                {
                    double mass_sec = obj_sec->get_mass();
                    Point mass_center_sec = obj_sec->get_mass_center();
                    Point vec = mass_center_sec;
                    vec -= mass_center_fir;
                    double r = (vec.get_x() ** 2 + vec.get_y() ** 2 + vec.get_z() ** 2) ** 0.5;
                    accel += (G * mass_sec / (r ** 3)) * vec;
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

    //Метод для обработки столкновений объектов в сцене
    void process_collisions()
    {
        std::map<std::size_t, std::vector<size_t>> collisions;
        std::vector<size_t> obj_id_vec = get_visible_index();
        for (size_t i = 0; i < obj_id_vec.size(); i++)
        {
            auto fir = _visible_objects[obj_id_vec[i]];
            for (size_t j = i; j < obj_id_vec.size(); j++)
            {
                //Для каждого i-го объекта в его массив заносятся id объектов, пересекающихся с ним
                auto sec = _visible_objects[obj_id_vec[j]];
                if (fir->check_intersection(sec))
                {
                    collisions[obj_id_vec[i]] = {};
                    collisions[obj_id_vec[i]].push_back(obj_id_vec[j]);
                }
            }
        }
        if (collisions.empty())
            return;
        while (true)
        {
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
            int flag = 0;
            for (auto &[id, data]: collisions)
            {
                flag = 0;
                for (auto &intersected: data)
                {
                    if (collisions.count(intersected) > 0)
                    {
                        for (size_t x: collisions[intersected])
                        {
                            //Чтобы не было повторов
                            if (!data.contains(x))
                                data.push_back(x);
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
        for (auto &[id, data]: collisions)
        {
            std::shared_ptr<VisibleObject> obj = get_object(id);
            for (size_t id2: data)
            {
                std::shared_ptr<VisibleObject> obj2 = get_object(id2);
                obj->add_object(obj2);
                _visible_objects.erase(id2);
            }
        }
    }
    void sim_iteration()
    {
        process_collisions();
        std::map<std::size_t, Point> accel = calc_accelerations();
        for (auto &[id, obj]: _visible_objects)
        {
            Point speed(accel[id]);
            speed *= TIME;
            obj->add_speed(speed);
            speed *= TIME;
            obj->move(speed.get_x(), speed.get_y(), speed.get_z());
        }
    }

private:
    std::map<std::size_t, std::shared_ptr<VisibleObject>> _visible_objects;
    std::map<std::size_t, std::shared_ptr<InvisibleObject>> _invisible_objects;
    std::size_t _index;
};

#endif // SCENE_H

