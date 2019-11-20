#pragma once
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> args_strings() {
    return std::vector<std::string>();
}

template <class T>
std::vector<std::string> args_strings(T&& val) {
    std::stringstream stream;
    stream << val;
    return {stream.str()};
}

template <class T, class... Args>
std::vector<std::string> args_strings(T&& val, Args&&... args) {
    std::stringstream stream;
    try {
        stream << val;
    }
    catch (const std::exception &e) {
        throw std::runtime_error(e.what());
    }
    catch (...) {
        throw std::runtime_error("strange error while <<");
    }
    std::vector<std::string> result;
    result.push_back(stream.str());
    std::vector<std::string> temp(args_strings(std::forward<Args>(args)...));
    for (auto &st : temp)
        result.push_back(st);
    return result;
}

template <class... Args>
std::string format(const char* st, Args&&... args)
{
    if (st == nullptr)
        throw std::runtime_error("no format string"); 
            
    std::vector<std::string> strings(
    args_strings(std::forward<Args>(args)...));
    std::string tmp;
    std::stringstream stream;
    bool add = false;
    for (int i = 0; st[i] != '\0'; ++i) {
        if (st[i] == '{')
            add = true;
        else if (add && st[i] == '}') {
            int ind;
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
            if (ind >= 0 && ind < static_cast<int>(strings.size()))
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