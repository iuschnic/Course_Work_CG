#ifndef QTFACTORY_H
#define QTFACTORY_H

#include "AbstractFactory.h"
#include "QTDrawer.h"

class QtFactory: public AbstractFactory
{
public:
    explicit QtFactory(std::shared_ptr<QGraphicsScene> &scene): _scene(scene) {}
    std::unique_ptr<BaseDrawer> create_drawer() override
    {
        return std::unique_ptr<BaseDrawer>(new QtDrawer(_scene));
    }
private:
    std::shared_ptr<QGraphicsScene> _scene;
};

#endif // QTFACTORY_H
