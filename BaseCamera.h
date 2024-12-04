#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "InvisibleObject.h"
#include "Point.h"

class BaseCamera: public InvisibleObject
{
public:
    BaseCamera()
    {
        _type = "camera";
    }
    virtual ~BaseCamera() = default;
    virtual Point& get_direction() = 0;
    virtual Point& get_center() = 0;
    virtual Point get_projection(const Point &point) = 0;
};

#endif // BASECAMERA_H
