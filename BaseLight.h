#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "InvisibleObject.h"
#include "Point.h"

class BaseLight: public InvisibleObject
{
public:
    BaseLight()
    {
        _type = "light";
    }
    virtual ~BaseLight() = default;
    //virtual Point& get_direction() = 0;
    //virtual Point get_projection(const Point &point) = 0;
};

#endif // BASELIGHT_H
