#pragma once
#include <iostream>
#include <string>

enum class Error
{
    NoError,
    CorruptedArchive,
    ErrorType
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    
private:
    std::ostream &out_;
    
    Error process(uint64_t val)
    {
        out_ << val << Separator;
        return Error::NoError;
    }
    
    Error process(bool val)
    {
        if (val)
            out_ << "true" << Separator;
        else
            out_ << "false" << Separator;
        return Error::NoError;
    }
    
    template <class T>
    Error process(T) {
        return Error::ErrorType;
    }

    template <class T, class... Args>
    Error process(T val, Args... args)
    {
        process(val);
        return process(args...);
    }
};


class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }
    
private:
    std::istream &in_;
    
    Error process(uint64_t& val)
    {
        std::string text;
        std::getline(in_, text, Separator);
        try {
            val = std::stoull(text);
        }
        catch (std::invalid_argument &e) {
            return Error::CorruptedArchive;
        }
        catch (std::out_of_range &e) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
    
    Error process(bool& val)
    {
        std::string text;
        std::getline(in_, text, Separator);
        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }
    
    template <class T>
    Error process(T&) {
        return Error::ErrorType;
    }

    template <class T, class... Args>
    Error process(T& val, Args&... args)
    {
        process(val);
        return process(args...);
    }
};

