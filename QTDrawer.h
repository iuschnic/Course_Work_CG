#ifndef QT_DRAWER_H
#define QT_DRAWER_H

#include <QGraphicsScene>
#include "BaseDrawer.h"

/*class QtDrawer: public BaseDrawer
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
};*/

class QtDrawer : public BaseDrawer
{
public:
    QtDrawer() = delete;
    explicit QtDrawer(const std::shared_ptr<QGraphicsScene> &scene): _scene(scene)
    {
        _image = std::shared_ptr<QImage>(new QImage(_scene->width(), _scene->height(), QImage::Format_RGB32));
        _scene->setBackgroundBrush(QBrush(QColor(0, 0, 0)));
        _width = _scene->width();
        _height = _scene->height();
    }

    virtual void clear() override
    {
        _lines.clear();
        _scene->clear();
        _image->fill(QColor(0, 0, 0));
    }
    virtual void draw() override
    {
        QPixmap item(QPixmap::fromImage(*_image));
        _scene->addPixmap(item);
        /*for (const auto &line : m_lines)
            m_scene->addLine(line.first, QPen(line.second));*/
    }
    virtual void add_line(const Point &point1, const Point &point2, const QColor &color) override
    {
        _lines.emplace_back(QLine(point1.get_x(), point1.get_y(), point2.get_x(), point2.get_y()), color);
    }
    virtual void add_point(const Pixel &point, const QColor &color) override
    {
        //auto c = color;
        if (point.get_x() < 0 || point.get_x() >= _width)
            return;
        if (point.get_y() < 0 || point.get_y() >= _height)
            return;
        //c.setAlphaF(point.get_intensity());
        _image->setPixelColor(QPoint(point.get_x(), point.get_y()), color);
    }
    virtual size_t get_height() const override
    {
        return _scene->height();
    }
    virtual size_t get_width() const override
    {
        return _scene->width();
    }

private:
    size_t _width;
    size_t _height;
    std::shared_ptr<QGraphicsScene> _scene;
    std::shared_ptr<QImage> _image;
    std::vector<std::pair<QLineF, QColor>> _lines;
};

#endif // QT_DRAWER_H
