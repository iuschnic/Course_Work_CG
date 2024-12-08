#ifndef BASEDRAWER_H
#define BASEDRAWER_H

#include "Point.h"

/*class BaseDrawer
{
public:
    BaseDrawer() = default;
    virtual ~BaseDrawer() = default;

    virtual void draw_line(const Point &p1, const Point &p2) = 0;
    virtual void clear_scene() = 0;
};*/

class BaseDrawer
{
public:
    BaseDrawer() = default;
    virtual ~BaseDrawer() = default;

    virtual void clear() = 0;
    virtual void draw() = 0;
    virtual void add_line(const Point &point1, const Point &point2, const QColor &color) = 0;
    virtual void add_point(const Pixel &point, QColor &color) = 0;
    virtual int get_height() const = 0;
    virtual int get_width() const = 0;
};

#endif // BASEDRAWER_H
