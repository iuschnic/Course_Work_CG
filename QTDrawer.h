#ifndef QT_DRAWER_H
#define QT_DRAWER_H

#include <QGraphicsScene>
#include "BaseDrawer.h"

class QtDrawer: public BaseDrawer
{
public:
    QtDrawer() = delete;
    explicit QtDrawer(QGraphicsScene *&scene): _scene(scene) {}
    QtDrawer(const QtDrawer &drawer): _scene(drawer._scene) {}

    // Класс точки использован для удобства, сама проекция точки на камеру будет искаться до момента отрисовки
    void draw_line(const Point &p1, const Point &p2) override
    {
        _scene->addLine(p1.get_x(), p1.get_y(), p2.get_x(), p2.get_y());
    }

    void draw_pixel(const Point &p1, QColor &color)
    {
        _scene->addEllipse(p1.get_x(), p1.get_y(), 1, 0, QPen(color));
    }

    void clear_scene() override
    {
        _scene->clear();
    }

private:
    QGraphicsScene *_scene;
};

#endif // QT_DRAWER_H
