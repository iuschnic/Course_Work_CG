#ifndef BASEDRAWER_H
#define BASEDRAWER_H

#include "Point.h"

class BaseDrawer
{
public:
    BaseDrawer() = default;
    virtual ~BaseDrawer() = default;

    virtual void clear() = 0;
    virtual void draw() = 0;
    virtual void add_point(const Pixel &point, QColor &color) = 0;
    virtual int get_height() const = 0;
    virtual int get_width() const = 0;
protected:
    int _height;
    int _width;
};

#endif // BASEDRAWER_H
