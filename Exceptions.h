#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>

class BaseException : public std::exception
{
public:
    explicit BaseException(std::string &msg) : _msg(msg) {}

    const char *what() const noexcept override { return "Error!";}
    const std::string &get_info() const noexcept { return _msg;}

protected:
    std::string _msg;
};

class CameraException : public BaseException
{
public:
    explicit CameraException(std::string &msg) : BaseException(msg) {}
    const char *what() const noexcept override
    {
        return "You need to add camera first";
    }
};

class ModelException : public BaseException
{
public:
    explicit ModelException(std::string &msg) : BaseException(msg) {}
    const char *what() const noexcept override
    {
        return "You need to add model first";
    }
};

class LoadException : public BaseException
{
public:
    explicit LoadException(std::string &msg) : BaseException(msg) {}
    const char *what() const noexcept override
    {
        return "Fail while loading model";
    }
};

#endif // EXCEPTIONS_H
