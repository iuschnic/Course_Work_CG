#ifndef FILELOADMANAGER_H
#define FILELOADMANAGER_H

#include "Object.h"
#include "LoadSolution.h"
#include <string>

class FileLoadManager
{
public:
    FileLoadManager()
    {
        _solution = std::make_shared<LoadSolution>(LoadSolution());
    }
    ~FileLoadManager() = default;

    std::vector<std::shared_ptr<VisibleObject>> load(std::string &filename)
    {
        return _solution->load(filename);
    }

private:
    std::shared_ptr<LoadSolution> _solution;
};

#endif // FILELOADMANAGER_H
