#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <cstddef>

class BaseObject
{
public:
    BaseObject() {_id = _next_id++;}
    virtual ~BaseObject() = default;

    virtual std::size_t get_id() const {return _id;}
    virtual void set_id(std::size_t id) {_id = id;}

    virtual void move(const double dx, const double dy, const double dz) = 0;
    virtual void scale(const double kx, const double ky, const double kz) = 0;
    virtual void rotate(const double ang_x, const double ang_y, const double ang_z) = 0;

protected:
    std::size_t _id;
    static std::size_t _next_id;
    //Point _center;
};

#endif // BASEOBJECT_H
