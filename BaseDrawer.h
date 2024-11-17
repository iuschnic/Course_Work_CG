#ifndef BASEDRAWER_H
#define BASEDRAWER_H

#include "Point.h"

class BaseDrawer
{
public:
    BaseDrawer() = default;
    virtual ~BaseDrawer() = default;

    virtual void draw_line(const Point &p1, const Point &p2) = 0;
    virtual void clear_scene() = 0;
};

#endif // BASEDRAWER_H
