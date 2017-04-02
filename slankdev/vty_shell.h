
/*
 * MIT License
 *
 * Copyright (c) 2017 Susanoo G
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file   slankdev/vty_shell.h
 * @brief  vty shell
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <algorithm>

#include <slankdev/exception.h>
#include <slankdev/string.h>
#include <slankdev/util.h>
#include <slankdev/asciicode.h>

#include "vty_keyfunc.h"
#include "vty_command.h"




namespace slankdev {



class inputbuffer {
  std::string ibuf;
  size_t cur_idx;
 public:
  inputbuffer() : cur_idx(0) {}
  void input_char(char c) { ibuf.insert(ibuf.begin()+cur_idx, c); cur_idx++; }
  void input_str(const std::string& str) { for (char c : str) input_char(c); }
  void clear()  { ibuf.clear(); cur_idx = 0; }
  size_t length()   const { return ibuf.length(); }
  const char* c_str() const { return ibuf.c_str();  }
  size_t index() const { return cur_idx; }
  bool empty() const { return ibuf.empty(); }
  std::string to_string() const { return ibuf; }

  void cursor_top() { cur_idx = 0; }
  void cursor_end() { cur_idx = ibuf.size(); }
  void cursor_right() { if (cur_idx < ibuf.length()) cur_idx ++ ; }
  void cursor_left() { if (cur_idx > 0) cur_idx -- ; }
  void cursor_backspace()
  {
    if (cur_idx > 0) {
      cur_idx --;
      ibuf.erase(ibuf.begin() + cur_idx);
    }
  }
};



class commandhistory {
  size_t hist_index;
  std::vector<std::string> history;
 public:
  commandhistory() : hist_index(0) {}
  void add(const std::string& str) { history.push_back(str); }
  void clean() { hist_index=0; }
  std::string deep_get()
  {
    if (history.empty()) return "";
    if (hist_index+2 > history.size()) return *(history.end() - hist_index - 1);
    return *(history.end() - ++hist_index);
  }
  std::string shallow_get()
  {
    if (history.empty()) return "";
    if (ssize_t(hist_index)-1 < 0) return *(history.end() - hist_index - 1);
    return *(history.end() - --hist_index - 1);
  }
};



class shell {
  friend class vty;
  std::string prompt;
  int   fd;
  bool  closed;

  void press_keys(const void* d, size_t l)
  {
    const uint8_t* p = reinterpret_cast<const uint8_t*>(d);
    if (l == 0) throw slankdev::exception("empty data received");

    for (key_func* kf : *keyfuncs) {
      if (kf->match(p, l)) {
        kf->function(this);
        return ;
      }
    }

    if (l > 1) {
      return ;
    }
    ibuf.input_char(p[0]);
  }
 public:
  commandhistory history;
  inputbuffer    ibuf;
  const std::vector<command*> * commands;
  const std::vector<key_func*>* keyfuncs;
  void* user_ptr;

  void close() { closed = true; }

  template <class... ARGS> void Printf(const char* fmt, ARGS... args)
  { slankdev::fdprintf(fd, fmt, args...); }

  shell(
      int d,
      const char* bootmsg,
      const char* prmpt,
      const std::vector<command*>* cmds,
      const std::vector<key_func*>* kfs,
      void* ptr
      ) :
    prompt(prmpt),
    fd(d),
    closed(false),
    commands(cmds),
    keyfuncs(kfs),
    user_ptr(ptr)
  {
    Printf(bootmsg);
    refresh_prompt();
  }
  void process()
  {
    char str[100];
    ssize_t res = ::read(fd, str, sizeof(str));
    if (res <= 0) throw slankdev::exception("OKASHII");

    press_keys(str, res);
    refresh_prompt();
  }
  void refresh_prompt();
  void exec_command();
};
inline void shell::refresh_prompt()
{
  char lineclear[] = {slankdev::AC_ESC, '[', 2, slankdev::AC_K, '\0'};
  Printf("\r%s", lineclear);
  Printf("\r%s%s", prompt.c_str(), ibuf.c_str());

  size_t backlen = ibuf.length() - ibuf.index();
  char left [] = {slankdev::AC_ESC, '[', slankdev::AC_D, '\0'};
  for (size_t i=0; i<backlen; i++) {
    Printf("%s", left);
  }
}
inline void shell::exec_command()
{
  Printf("\r\n");
  if (!ibuf.empty()) {
    history.add(ibuf.to_string());
    for (size_t i=0; i<commands->size(); i++) {
      if (commands->at(i)->match(ibuf.c_str())) {
        commands->at(i)->func(this);
        ibuf.clear();
        history.clean();
        Printf("\r%s%s", prompt.c_str(), ibuf.c_str());
        refresh_prompt();
        return ;
      }
    }
    Printf("command not found: \"%s\"\r\n", ibuf.c_str());
  }
  history.clean();
  ibuf.clear();
  Printf("\r%s%s", prompt.c_str(), ibuf.c_str());
  refresh_prompt();
}


/*
 * Default key_functions Implementation
 */
struct KF_return : public key_func {
  KF_return(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh) { sh->exec_command(); }
};
struct KF_backspace : public key_func {
  KF_backspace(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh) { sh->ibuf.cursor_backspace(); }
};
struct KF_right : public key_func {
  KF_right(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh) { sh->ibuf.cursor_right(); }
};
struct KF_left : public key_func {
  KF_left(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh) { sh->ibuf.cursor_left(); }
};
struct KF_hist_search_deep : public key_func {
  KF_hist_search_deep(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh)
  {
    sh->ibuf.clear();
    sh->ibuf.input_str(sh->history.deep_get());
    sh->refresh_prompt();
  }
};
struct KF_hist_search_shallow : public key_func {
  KF_hist_search_shallow(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh)
  {
    sh->ibuf.clear();
    sh->ibuf.input_str(sh->history.shallow_get());
    sh->refresh_prompt();
  }
};
struct KF_cursor_top : public key_func {
  KF_cursor_top(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh) { sh->ibuf.cursor_top(); }
};
struct KF_cursor_end : public key_func {
  KF_cursor_end(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh) { sh->ibuf.cursor_end(); }
};
struct KF_completion : public key_func {
  KF_completion(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh);
};


#if 0
struct help_node {
  help_node(const char* s) : str(s) {}
  std::string str;
  std::vector<help_node> childs;
};

template <class... ARGS>
  void dpprintf(size_t depth, const char* fmt, ARGS... args)
  {
    for (size_t i=0; i<depth; i++) printf("   ");
    printf(fmt, args...);
  }

void pprint(help_node& top, size_t depth=0)
{
  dpprintf(depth, "%s\n", top.str.c_str());
  for (help_node& n : top.childs) {
    pprint(n, depth+1);
  }
}
#endif


// help_node N("top");
// N.childs.push_back(help_node("ss"));
// help_node C("aa");
// C.childs.push_back("dsfdf");
// C.childs.push_back("dfdf");
// C.childs.push_back("awfgg");
// N.childs.push_back(C);
// N.childs.push_back(help_node("slankdev"));
// pprint(N);


struct KF_help : public key_func {
  KF_help(const void* c, size_t l) : key_func(c, l) {}
  void function(shell* sh);
};
static inline bool endisspace(std::string str)
{
  const char* istr = str.c_str();
  return (istr[strlen(istr)-1] == ' ');
}
inline bool is_prefix(std::string str, std::string pref)
{
  auto ret = str.find(pref);
  if (ret != std::string::npos) {
    if (ret == 0) return true;
  }
  return false;
}

inline void KF_help::function(shell* sh)
{
  const std::vector<command*>& commands = *sh->commands;
  std::vector<std::string> list = slankdev::split(sh->ibuf.c_str(), ' ');
  if (endisspace(sh->ibuf.c_str()) || list.empty()) list.push_back("");

  sh->Printf("\r\n");
  std::vector<std::string> match;
  for (command* cmd : commands) {

    for (size_t i=0; i<list.size(); i++) {

      if (i == cmd->nodes.size()) {

        if (list[i] == "") {
          sh->Printf("  <CR>\r\n");
        } else {
          sh->Printf("  %% There is no matched command.\r\n");
        }
        return ;

      } else {

        if (!cmd->nodes[i]->match_prefix(list[i])) {
          break;
        } else {
          if (i+1==list.size()) {
            std::string s = slankdev::fs("%-10s:   %-20s",
                cmd->nodes[i]->to_string().c_str(),
                cmd->nodes[i]->msg().c_str()
                );
            match.push_back(s);
          } else {
            ;
          }
        }

      }

    }

  }

  std::sort(match.begin(), match.end());
  match.erase(std::unique(match.begin(), match.end()), match.end());
  for (std::string& s : match) {
    sh->Printf("  %s \r\n", s.c_str());
  }

}

inline void KF_completion::function(shell* sh)
{
  const std::vector<command*>& commands = *sh->commands;
  std::vector<std::string> list = slankdev::split(sh->ibuf.c_str(), ' ');
  if (endisspace(sh->ibuf.c_str()) || list.empty()) list.push_back("");

  sh->Printf("\r\n");
  std::vector<std::string> match;
  for (command* cmd : commands) {

    for (size_t i=0; i<list.size(); i++) {

      if (i == cmd->nodes.size()) {

        if (list[i] == "") {
          sh->Printf("  <CR>\r\n");
        } else {
          sh->Printf("  %% There is no matched command.\r\n");
        }
        return ;

      } else {

        if (!cmd->nodes[i]->match_prefix(list[i])) {
          break;
        } else {
          if (i+1==list.size()) {
            std::string s = cmd->nodes[i]->to_string();
            match.push_back(s);
          } else {
            ;
          }
        }

      }

    }

  }

  std::sort(match.begin(), match.end());
  match.erase(std::unique(match.begin(), match.end()), match.end());

  if (match.size() == 1) {
    std::string s;
    for (size_t i=0; i<list.size(); i++) {
      if (i == list.size()-1) {
        s += match[0];
      } else {
        s += list[i];
      }
      s += " ";
    }
    sh->ibuf.clear();
    sh->ibuf.input_str(s);
  } else {
    for (std::string& s : match) {
      sh->Printf("  %s ", s.c_str());
    }
    sh->Printf("\r\n");
  }
}


} /* namespace slankdev */





