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

class LoadException : public BaseException
{
public:
    explicit LoadException(std::string &msg) : BaseException(msg) {}
    const char *what() const noexcept override
    {
        return "Fail while loading scene";
    }
};

class GenException : public BaseException
{
public:
    explicit GenException(std::string &msg) : BaseException(msg) {}
    const char *what() const noexcept override
    {
        return "Fail while generating scene";
    }
};

class EmptySceneException : public BaseException
{
public:
    explicit EmptySceneException(std::string &msg) : BaseException(msg) {}
    const char *what() const noexcept override
    {
        return "Empty scene";
    }
};

#endif // EXCEPTIONS_H
