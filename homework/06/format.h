#pragma once
#include <string>
#include <vector>
#include <sstream>

template <class T>
void args_strings(std::vector<std::string>& res, T&& val) {
    std::stringstream stream;
    stream << val;
    res.push_back(stream.str());
}

template <class T, class... Args>
void args_strings(std::vector<std::string>&res, T&& val, Args&&... args) {
    std::stringstream stream;
    stream << val;
    res.push_back(stream.str());
    args_strings(res, std::forward<Args>(args)...);
}

std::string format(const char *st) {
    if (st == nullptr)
        throw std::runtime_error("no format string");
    for (int i = 0; st[i] != '\0'; ++i)
        if (st[i] == '{' || st[i] == '}')
            throw std::runtime_error("{} with empty args");
    return std::string(st);
}

template <class... Args>
std::string format(const char* st, Args&&... args)
{
    if (st == nullptr)
        throw std::runtime_error("no format string"); 
            
    std::vector<std::string> strings;
    args_strings(strings, std::forward<Args>(args)...);
    std::string tmp;
    std::stringstream stream;
    bool add = false;
    for (int i = 0; st[i] != '\0'; ++i) {
        if (st[i] == '{')
            add = true;
        else if (add && st[i] == '}') {
            size_t ind;
            try {
                ind = std::stoi(tmp);
            }
            catch (const std::invalid_argument &e) {
                throw std::runtime_error("bad index");
            }
            catch (const std::out_of_range &e) {
                throw std::runtime_error("too big index");
            }
            tmp.clear();
            if (ind >= 0 && ind < strings.size())
                stream << strings[ind];
            else
                throw std::runtime_error("no such index");
            add = false;
        }
        else if (st[i] == '}')
            throw std::runtime_error("{ expected");
        else if (add) {
            tmp += st[i];
        }
        else {
            stream << st[i];
        }
    }
    if (tmp.size() != 0)
        throw std::runtime_error("} expected");
    return stream.str();
}
