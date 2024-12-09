#ifndef COMMANDS_H
#define COMMANDS_H

#include "BaseCommand.h"
#include <iostream>

enum direction
{
    m_forward = 0,
    m_backward = 1,
    m_right = 2,
    m_left = 3,
    m_up = 4,
    m_down = 5,
    r_left = 6,
    r_right = 7,
    none = 8
};

//Команды отрисовки и очистки сцены
class DrawSceneCommand: public BaseCommand
{
    using Action = void(SceneManager::*)(std::shared_ptr<BaseDrawer> drawer);

public:
    DrawSceneCommand(std::shared_ptr<BaseDrawer> drawer)
    {
        _drawer = drawer;
        _method = &SceneManager::draw_scene;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)(_drawer);
    }

private:
    Action _method;
    std::shared_ptr<BaseDrawer> _drawer;
};

class ClearGraphicsSceneCommand: public BaseCommand
{
    using Action = void(SceneManager::*)(std::shared_ptr<BaseDrawer> drawer);

public:
    ClearGraphicsSceneCommand(std::shared_ptr<BaseDrawer> drawer)
    {
        _drawer = drawer;
        _method = &SceneManager::clear_graphics_scene;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)(_drawer);
    }

private:
    Action _method;
    std::shared_ptr<BaseDrawer> _drawer;

};

class ClearSceneCommand: public BaseCommand
{
    using Action = void(SceneManager::*)();

public:
    ClearSceneCommand()
    {
        _method = &SceneManager::clear_scene;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)();
    }

private:
    Action _method;
};

//Команды запуска и остановки симуляции

class SimIterationCommand: public BaseCommand
{
    using Action = void(SceneManager::*)(std::shared_ptr<BaseDrawer> drawer);

public:
    SimIterationCommand(std::shared_ptr<BaseDrawer> drawer)
    {
        _method = &SceneManager::sim_iteration;
        _drawer = drawer;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)(_drawer);
    }

private:
    Action _method;
    std::shared_ptr<BaseDrawer> _drawer;
};

//Команды добавления/удаления камер
class AddCameraCommand : public BaseCommand
{
    using Action = std::size_t(SceneManager::*)(const Point &center, const Point &direction, const Point &up);

public:
    AddCameraCommand(const Point &center, const Point &direction, const Point &up, std::shared_ptr<std::size_t> &id)
    {
        _id = id;
        _center = center;
        _direction = direction;
        _up = up;
        _method = &SceneManager::add_camera;
    }
    virtual void execute() override
    {
        (*_id) = ((*_scene_manager).*_method)(_center, _direction, _up);
    }

private:
    Point _center;
    Point _direction;
    Point _up;
    std::shared_ptr<std::size_t> _id;
    Action _method;
};

class DeleteCameraCommand : public BaseCommand
{
    using Action = void(SceneManager::*)(const std::size_t &id_camera);

public:
    DeleteCameraCommand(const std::size_t &id_camera)
    {
        _id_camera = id_camera;
        _method = &SceneManager::delete_camera;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)(_id_camera);
    }

private:
    std::size_t _id_camera;
    Action _method;
};

class MoveRotateCameraCommand : public BaseCommand
{
    using Action = void(SceneManager::*)();

public:
    MoveRotateCameraCommand(const direction dir)
    {
        if (dir == m_forward)
            _method = &SceneManager::move_camera_forward;
        if (dir == m_backward)
            _method = &SceneManager::move_camera_backward;
        if (dir == m_left)
            _method = &SceneManager::move_camera_left;
        if (dir == m_right)
            _method = &SceneManager::move_camera_right;
        if (dir == m_up)
            _method = &SceneManager::move_camera_up;
        if (dir == m_down)
            _method = &SceneManager::move_camera_down;
        if (dir == r_left)
            _method = &SceneManager::rotate_camera_left;
        if (dir == r_right)
            _method = &SceneManager::rotate_camera_right;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)();
    }

private:
    Action _method;
};

//Команды добавления/удаления света
class AddLightCommand : public BaseCommand
{
    using Action = std::size_t(SceneManager::*)(const Point &center, const Point &direction, const Point &up);

public:
    AddLightCommand(const Point &center, const Point &direction, const Point &up, std::shared_ptr<std::size_t> &id)
    {
        _id = id;
        _center = center;
        _direction = direction;
        _up = up;
        _method = &SceneManager::add_light;
    }
    virtual void execute() override
    {
        (*_id) = ((*_scene_manager).*_method)(_center, _direction, _up);
    }

private:
    Point _center;
    Point _direction;
    Point _up;
    std::shared_ptr<std::size_t> _id;
    Action _method;
};

class DeleteLightCommand : public BaseCommand
{
    using Action = void(SceneManager::*)(const std::size_t &id_light);

public:
    DeleteLightCommand(const std::size_t &id_light)
    {
        _id_light = id_light;
        _method = &SceneManager::delete_light;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)(_id_light);
    }

private:
    std::size_t _id_light;
    Action _method;
};

//Команды добавления/удаления тел
class AddObjectCommand : public BaseCommand
{
    using Action = std::size_t(SceneManager::*)(std::shared_ptr<VisibleObject> &object);

public:
    AddObjectCommand(std::shared_ptr<VisibleObject> &object, std::shared_ptr<std::size_t> &id) : _id(id)
    {
        _object = object;
        _method = &SceneManager::add_object;
    }
    virtual void execute() override
    {
        (*_id) = ((*_scene_manager).*_method)(_object);
    }

private:
    std::shared_ptr<VisibleObject> _object;
    std::shared_ptr<std::size_t> &_id;
    Action _method;
};

class DeleteObjectCommand : public BaseCommand
{
    using Action = void(SceneManager::*)(const std::size_t &id_object);

public:
    DeleteObjectCommand(const std::size_t &id_object)
    {
        _id_object = id_object;
        _method = &SceneManager::delete_object;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)(_id_object);
    }

private:
    std::size_t _id_object;
    Action _method;
};

//Команды управления основной камерой сцены
class GetMainCameraCommand : public BaseCommand
{
    using Action = std::shared_ptr<PProjCamera> (SceneManager::*)();

public:
    GetMainCameraCommand(std::shared_ptr<PProjCamera> &camera) : _camera(camera)
    {
        _method = &SceneManager::get_main_camera;
    }
    virtual void execute() override
    {
        _camera = ((*_scene_manager).*_method)();
    }

private:
    std::shared_ptr<PProjCamera> &_camera; // Нужна ли & ?
    Action _method;
};

class SetMainCameraCommand : public BaseCommand
{
    using Action = void(SceneManager::*)(const std::size_t id_camera);

public:
    SetMainCameraCommand(const std::size_t &id_camera)
    {
        _id_camera = id_camera;
        _method = &SceneManager::set_main_camera;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)(_id_camera);
    }

private:
    std::size_t _id_camera;
    Action _method;
};

class GetMainLightCommand : public BaseCommand
{
    using Action = std::shared_ptr<PointLight> (SceneManager::*)();

public:
    GetMainLightCommand(std::shared_ptr<PointLight> &light) : _light(light)
    {
        _method = &SceneManager::get_main_light;
    }
    virtual void execute() override
    {
        _light = ((*_scene_manager).*_method)();
    }

private:
    std::shared_ptr<PointLight> &_light; // Нужна ли & ?
    Action _method;
};

class SetMainLightCommand : public BaseCommand
{
    using Action = void(SceneManager::*)(const std::size_t id_light);

public:
    SetMainLightCommand(const std::size_t &id_light)
    {
        _id_light = id_light;
        _method = &SceneManager::set_main_light;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)(_id_light);
    }

private:
    std::size_t _id_light;
    Action _method;
};

//Команда загрузки сцены из файла
class FileLoadModelCommand : public BaseCommand
{
    using Action = std::vector<std::shared_ptr<VisibleObject>>(FileLoadManager::*)(std::string &filename);

public:
    FileLoadModelCommand(std::vector<std::shared_ptr<VisibleObject>> &objs, std::string &filename) : _objs(objs)
    {
        _filename = filename;
        _method = &FileLoadManager::load;
    }

    virtual void execute() override
    {
        _objs = ((*_load_manager).*_method)(_filename);
    }

private:
    std::string _filename;
    std::vector<std::shared_ptr<VisibleObject>> &_objs; // Нужна ли & ?
    Action _method;
};

#endif // COMMANDS_H
