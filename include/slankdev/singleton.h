
#pragma once

#include <stdio.h>
#include <string.h>
#include <stddef.h>


namespace slankdev {
    


#if 1
template <typename T>
class singleton {
protected:
    singleton() {}
    ~singleton() {}
    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;

public:
    static T& instance()
    {
        static T instnc;
        return instnc;
    }
};
#endif


#if 0
template <typename T>
class singleton {
protected:
    static bool _inited;
    singleton() {}
    ~singleton() {}
    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;

public:
    static T& _instance()
    {
        static T t;
        return t;
    }
    template <typename... Arg>
    static T& instance(Arg... args)
    {
        T& t = _instance();
        if (!_inited) {
            t.init(args...);
            _inited = true;
        }
        return t;
    }
    bool inited()
    {
        return _inited;
    }

    // void init();
    // void init(Arg.. args);
};
template <typename T>
bool singleton<T>::_inited = false;
#endif



#if 0 // I don't like this implementation
template <typename T, class... Arg>
class unsafe_singleton2 {
protected:
    static bool _inited;
    unsafe_singleton2() {}
    ~unsafe_singleton2() {}
    unsafe_singleton2(const unsafe_singleton2&) = delete;
    unsafe_singleton2& operator=(const unsafe_singleton2&) = delete;

public:
    static T& instance(bool dontinit=false)
    {
        static T _instance;
        if (dontinit) {
            _inited = true;
            return _instance;
        }
        if (!_inited)
            _instance.init();
        _inited = true;
        return _instance;
    }
    static T& instance(Arg... args)
    {
        if (_inited) {
            fprintf(stderr, "You try to reinit, Maybe it's not allow.\n");
            exit(-1);
        }
        static T& t = instance(true);
        t.init(args...);
        return t;
    }
};
template <typename T, class... Arg>
bool unsafe_singleton2<T, Arg...>::_inited = false;

class test : public unsafe_singleton<test> {
    friend unsafe_singleton<test>;
    private:
    test() {puts("cnst");}
    ~test() {puts("dst");}
    void init()         {puts("init(void)");}
    void init(int)      {puts("init(int)");};
    void init(double, int) {puts("init(double, int)");};
};


int main()
{
    test& ada = test::instance(1.1, 2);
    test& t2a = test::instance();
}
#endif


} /* namespace slankdev */
