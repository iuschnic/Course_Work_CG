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

    std::vector<std::shared_ptr<VisibleObject>> load(const int &n_objs, const double &min_r, const double &max_r,
                                                     const double &min_s, const double &max_s)
    {
        return _solution->load(n_objs, min_r, max_r, min_s, max_s);
    }

private:
    std::shared_ptr<LoadSolution> _solution;
};

#endif // FILELOADMANAGER_H
