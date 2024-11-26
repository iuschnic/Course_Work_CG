#ifndef BASECOMMAND_H
#define BASECOMMAND_H

#include "SceneManager.h"
#include "FileLoadManager.h"
#include "BaseDrawer.h"

class BaseCommand
{
public:
    BaseCommand() = default;
    virtual ~BaseCommand() = default;

    //Проверить, нужны ли &
    virtual void set_managers(std::shared_ptr<SceneManager> &scene_manager,
                              std::shared_ptr<FileLoadManager> &load_manager)
    {
        _scene_manager = scene_manager;
        _load_manager = load_manager;
    }

    virtual void execute() = 0;

protected:
    std::shared_ptr<SceneManager> _scene_manager;
    std::shared_ptr<FileLoadManager> _load_manager;
};

#endif // BASECOMMAND_H
