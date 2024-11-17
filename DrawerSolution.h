#ifndef DRAWERSOLUTION_H
#define DRAWERSOLUTION_H

#include "AbstractFactory.h"
#include "QtFactory.h"

template <typename Tfactory, typename Tscene>
class DrawerSolution
{
public:
    std::unique_ptr<BaseDrawer> create_drawer(Tscene *scene)
    {
        std::shared_ptr<AbstractFactory> factory(new Tfactory(scene));
        return factory->create_drawer();
    }
};

#endif // DRAWERSOLUTION_H
