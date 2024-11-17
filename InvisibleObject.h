#ifndef INVISIBLEOBJECT_H
#define INVISIBLEOBJECT_H

#include "BaseObject.h"

class InvisibleObject: public BaseObject
{
public:
    InvisibleObject() : _type(std::string("no_type")) {}
    virtual ~InvisibleObject() override = default;
    std::string get_type() {return _type;}
    void set_type(std::string type) {_type = type;}
protected:
    std::string _type;
};

#endif // INVISIBLEOBJECT_H
