#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include <memory>
#include "BaseDrawer.h"

class AbstractFactory
{
public:
    virtual std::unique_ptr<BaseDrawer> create_drawer() = 0;
};

#endif // ABSTRACTFACTORY_H
