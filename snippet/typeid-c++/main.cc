
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

class A {
};
class B : A {
};

template <class T>
std::string get_classname_from_instance(T& instance)
{
  std::string ret = "";
  const std::type_info& id = typeid(instance);
  int stat;
  char *name = abi::__cxa_demangle(id.name(),0,0,&stat);
  if(name!=NULL) {
    if(stat==0) {
      ret = name;
    }
    free(name);
  }
  return ret;
}

template <class T>
std::string get_classname_from_classtype()
{
  T t;
  return get_classname_from_instance(t);
}

int main()
{
  using std::cout;
  using std::endl;

  A a;
  B b;
  B* bp;
  cout << get_classname_from_classtype<A>() << endl;
  cout << get_classname_from_instance(a) << endl;
  cout << get_classname_from_classtype<B>() << endl;
  cout << get_classname_from_instance(b) << endl;
  cout << get_classname_from_instance(*bp) << endl;
}

