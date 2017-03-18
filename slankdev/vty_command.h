
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
    virtual bool match_prefix(const std::string& str) = 0;
    virtual std::string& get() { throw slankdev::exception("FAAA"); }
    virtual std::string to_string() = 0;
    virtual const std::string& msg() = 0;
};


class node_fixedstring : public node {
    std::string str;
    const std::string msg_;
public:
    node_fixedstring(const std::string& s, const std::string m) : str(s), msg_(m) {}
    bool match(const std::string& s) { return str == s; }
    bool match_prefix(const std::string& s)
    {
        auto ret = str.find(s);
        if (ret != std::string::npos) {
            if (ret == 0) return true;
        }
        return false;
    }
    std::string to_string() { return str; }
    const std::string& msg() { return msg_; }
};


class node_string : public node {
    std::string str;
    const std::string msg_;
public:
    node_string() : msg_("string") {}
    bool match(const std::string& s)
    {
        if (s.length() > 0) {
            str = s;
            return true;
        }
        return false;
    }
    bool match_prefix(const std::string&)
    {
        return true;
    }
    std::string& get() { return str; }
    std::string to_string() { return "<string>"; }
    const std::string& msg() { return msg_; }
};


class shell;
class command {
public:
    std::vector<node*> nodes;
    virtual ~command() {}
    virtual void func(shell* sh) = 0;
    virtual bool match(const std::string& str)
    {
        std::vector<std::string> list = slankdev::split(str, ' ');
        if (list.size() != nodes.size()) {
            return false;
        }

        for (size_t i=0; i<list.size(); i++) {
            if (nodes[i]->match(list[i]) == false) {
                return false;
            }
        }
        return true;
    }
};

} /* namespace slankdev */




