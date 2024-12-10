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
};

#endif // BASELIGHT_H
