
#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <string>
#include <vector>

#include <slankdev/exception.h>
#include <slankdev/string.h>



namespace slankdev {



class node {
public:
    virtual bool match(const std::string& str) = 0;
    virtual std::string& get() { throw slankdev::exception("FAAA"); }
    virtual std::string to_string() = 0;
};


class node_fixedstring : public node {
    std::string str;
public:
    node_fixedstring(const std::string& s) : str(s) {}
    bool match(const std::string& s) { return str == s; }
    std::string to_string() { return str; }
};


class node_string : public node {
    std::string str;
public:
    node_string() {}
    bool match(const std::string& s)
    {
        if (s.length() > 0) {
            str = s;
            return true;
        }
        return false;
    }
    std::string& get() { return str; }
    std::string to_string() { return "<string>"; }
};


class shell;
class command {
public:
    std::vector<node*> nodestack;
    virtual void func(shell* sh) = 0;
    virtual bool match(const std::string& str)
    {
        std::vector<std::string> list = slankdev::split(str, ' ');
        if (list.size() != nodestack.size()) {
            return false;
        }

        for (size_t i=0; i<list.size(); i++) {
            if (nodestack[i]->match(list[i]) == false) {
                return false;
            }
        }
        return true;
    }
};

} /* namespace slankdev */




