

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <string>
#include <vector>


namespace slankdev {

class argparse {
  struct Opt {
    bool          flag;
    std::string   optarg;
    Opt() : flag(false), optarg("") {}
  };

  std::vector<struct option> options;
  Opt array[10000];

 public:
  void add_opt(const char* name, int has_arg, int* flag, int val)
  { options.push_back({name, has_arg, flag, val}); }
  void parse(int argc, char** argv);
  bool is_set(char c) const { return array[c].flag; }
  std::string get_optarg(char c) { return array[c].optarg.c_str(); }
};


void argparse::parse(int argc, char** argv)
{
  std::string optstring;
  for (size_t i=0; ; i++) {
    if (options[i].name == 0) break;

    optstring += char(options[i].val);
    if (options[i].has_arg) optstring += ":";
  }

  while (true) {
    int c = getopt_long(argc, argv, optstring.c_str(), options.data(), NULL);
    if (c == -1) break;

    for (size_t i=0; ; i++) {
      if (options[i].name == 0) break;

      if (c == options[i].val) {
        array[c].flag = true;

        if (options[i].has_arg) {
          array[c].optarg = optarg;
        }
      }
    }
  }
}


} /* namespace slankdev */
