
#pragma once
#include <slankdev/string.h>
#include <slankdev/filefd.h>

namespace slankdev {

inline std::string get_git_head_hash(std::string gitroot)
{
  char path[100];
  std::string tmp;
  slankdev::filefd file;
  file.fopen(gitroot + "/.git/HEAD", "r");
  tmp = file.readline();
  sscanf(tmp.c_str(), "ref: %s\n", path);
  file.fclose();

  file.fopen(gitroot + "/.git/" + path, "r");
  tmp = file.readline();
  tmp[tmp.size()-1] = '\0';
  return tmp;

}

} /* namespace slankdev */

