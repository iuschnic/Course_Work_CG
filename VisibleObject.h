#ifndef VISIBLEOBJECT_H
#define VISIBLEOBJECT_H

#include "BaseObject.h"

class VisibleObject: public BaseObject
{
public:
    VisibleObject() : _type(std::string("no_type")) {}
    virtual ~VisibleObject() override = default;
    std::string get_type() {return _type;}
    void set_type(std::string type) {_type = type;}
protected:
    std::string _type;
};

#endif // VISIBLEOBJECT_H
