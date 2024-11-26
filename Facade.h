#ifndef FACADE_H
#define FACADE_H

#include "BaseCommand.h"

class Facade
{
public:
    Facade()
    {
        _scene_manager = std::make_shared<SceneManager>(SceneManager());
        _load_manager = std::make_shared<FileLoadManager>(FileLoadManager());
    }
    ~Facade() = default;

    void execute(BaseCommand &command)
    {
        command.set_managers(_scene_manager, _load_manager);
        //auto cam = _scene_manager->get_main_camera();
        //_scene_manager->set_main_camera(cam);
        command.execute();
    }

private:
    std::shared_ptr<FileLoadManager> _load_manager;
    std::shared_ptr<SceneManager> _scene_manager;
};

#endif // FACADE_H

