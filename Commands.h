#ifndef COMMANDS_H
#define COMMANDS_H

#include "BaseCommand.h"
#include <iostream>

//Команды отрисовки и очистки сцены
class DrawSceneCommand: public BaseCommand
{
    using Action = void(SceneManager::*)();

public:
    DrawSceneCommand()
    {
        _method = &SceneManager::draw_scene;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)();
    }

private:
    Action _method;
};

class ClearGraphicsSceneCommand: public BaseCommand
{
    using Action = void(SceneManager::*)();

public:
    ClearGraphicsSceneCommand()
    {
        _method = &SceneManager::clear_graphics_scene;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)();
    }

private:
    Action _method;
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
class StartSimulationCommand: public BaseCommand
{
    using Action = void(SceneManager::*)();

public:
    StartSimulationCommand()
    {
        _method = &SceneManager::start_simulation;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)();
    }

private:
    Action _method;
};

class StopSimulationCommand: public BaseCommand
{
    using Action = void(SceneManager::*)();

public:
    StopSimulationCommand()
    {
        _method = &SceneManager::stop_simulation;
    }
    virtual void execute() override
    {
        ((*_scene_manager).*_method)();
    }

private:
    Action _method;
};

//Команды добавления/удаления камер
class AddCameraCommand : public BaseCommand
{
    using Action = std::size_t(SceneManager::*)(const Point &location);

public:
    AddCameraCommand(const Point &location, std::shared_ptr<std::size_t> &id)
    {
        _id = id;
        _location = location;
        _method = &SceneManager::add_camera;
    }
    virtual void execute() override
    {
        (*_id) = ((*_scene_manager).*_method)(_location);
    }

private:
    Point _location;
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

//Команды добавления/удаления света
class AddLightCommand : public BaseCommand
{
    using Action = std::size_t(SceneManager::*)(const Point &location);

public:
    AddLightCommand(const Point &location, std::shared_ptr<std::size_t> &id)
    {
        _id = id;
        _location = location;
        _method = &SceneManager::add_light;
    }
    virtual void execute() override
    {
        (*_id) = ((*_scene_manager).*_method)(_location);
    }

private:
    Point _location;
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
class GetMainCameraCommand : public BaseSceneCommand
{
    using Action = std::shared_ptr<Camera> (SceneManager::*)();

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

class SetMainCameraCommand : public BaseSceneCommand
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

#endif // COMMANDS_H
