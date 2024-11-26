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
    virtual void addLine(const Point &point1, const Point &point2, const QColor &color) = 0;
    virtual void addPoint(const Point &point, const QColor &color) = 0;
    virtual size_t getHeight() const = 0;
    virtual size_t getWidth() const = 0;
};

#endif // BASEDRAWER_H
