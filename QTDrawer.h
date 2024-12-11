#ifndef QT_DRAWER_H
#define QT_DRAWER_H

#include <QGraphicsScene>
#include "BaseDrawer.h"

class QtDrawer : public BaseDrawer
{
public:
    QtDrawer() = delete;
    explicit QtDrawer(const std::shared_ptr<QGraphicsScene> &scene): _scene(scene)
    {
        _image = std::shared_ptr<QImage>(new QImage(_scene->width(), _scene->height(), QImage::Format_RGB32));
        _scene->setBackgroundBrush(QBrush(QColor(175, 218, 252)));
        _width = _scene->width();
        _height = _scene->height();
    }

    virtual void clear() override
    {
        _lines.clear();
        _scene->clear();
        _image->fill(QColor(175, 218, 252));
    }
    virtual void draw() override
    {
        QPixmap item(QPixmap::fromImage(*_image));
        _scene->addPixmap(item);
    }
    virtual void add_point(const Pixel &point, QColor &color) override
    {
        if (point.get_x() < 0 || point.get_x() >= _width)
            return;
        if (point.get_y() < 0 || point.get_y() >= _height)
            return;
        float h, s, l, a;
        color.getHslF(&h, &s, &l, &a);
        color.setHslF(h, s, point.get_intensity(), a);
        _image->setPixelColor(QPoint(point.get_x(), point.get_y()), color);
    }
    virtual int get_height() const override
    {
        return _scene->height();
    }
    virtual int get_width() const override
    {
        return _scene->width();
    }

private:
    //int _width;
    //int _height;
    std::shared_ptr<QGraphicsScene> _scene;
    std::shared_ptr<QImage> _image;
    std::vector<std::pair<QLineF, QColor>> _lines;
};

#endif // QT_DRAWER_H
