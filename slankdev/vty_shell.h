
#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <string>
#include <vector>

#include <slankdev/exception.h>
#include <slankdev/string.h>

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

    void cursor_right() { cur_idx ++ ; }
    void cursor_left() { cur_idx -- ; }
    void cursor_backspace()
    {
        if (cur_idx > 0) {
            cur_idx --;
            ibuf.erase(ibuf.begin() + cur_idx);
        }
    }
};



class commandhistory {
    ssize_t hist_index;
    std::vector<std::string> history;
public:
    commandhistory() : hist_index(0) {}
    void add(const std::string& str) { history.push_back(str); }
    void clean() { hist_index=0; }
    std::string deep_get()
    {
        if (hist_index+2 > history.size()) return *(history.end() - hist_index - 1);
        return *(history.end() - ++hist_index);
    }
    std::string shallow_get()
    {
        if (hist_index-1 < 0) return *(history.end() - hist_index - 1);
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
    void* user_ptr;
    commandhistory history;
    inputbuffer    ibuf;
    const std::vector<command*> * commands;
    const std::vector<key_func*>* keyfuncs;

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
    char left [] = {slankdev::AC_ESC, '[', slankdev::AC_D};
    for (size_t i=0; i<backlen; i++) {
        Printf("%s", left);
    }
}
inline void shell::exec_command()
{
    Printf("\r\n");
    if (!ibuf.empty()) {
        history.add(ibuf.to_string());
        bool cmd_not_found = true;
        for (size_t i=0; i<commands->size(); i++) {
            if (commands->at(i)->match(ibuf.c_str())) {
                commands->at(i)->func(this);
                ibuf.clear();
                Printf("\r%s%s", prompt.c_str(), ibuf.c_str());
                refresh_prompt();
                return ;
            }
        }
        if (cmd_not_found)
            Printf("command not found: \"%s\"\r\n", ibuf.c_str());
    }
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
struct KF_completion : public key_func {
    KF_completion(const void* c, size_t l) : key_func(c, l) {}
    void function(shell* sh)
    {
        sh->Printf("\r\nCOMPLETION\r\n");
    }
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


struct KF_help : public key_func {
    KF_help(const void* c, size_t l) : key_func(c, l) {}
    void function(shell* sh);
};
inline void KF_help::function(shell* sh)
{
    sh->Printf("\r\nHELP\r\n");
#if 0
    const std::vector<command*>& commands = *sh->commands;
    std::vector<std::string> list = slankdev::split(sh->ibuf.c_str(), ' ');
    const char* istr = sh->ibuf.c_str();
    if (istr[strlen(istr)-1] == ' ' || list.empty()) {
        list.push_back("");
    }

    help_node N("top");
    N.childs.push_back(help_node("ss"));
    help_node C("aa");
    C.childs.push_back("dsfdf");
    C.childs.push_back("dfdf");
    C.childs.push_back("awfgg");
    N.childs.push_back(C);
    N.childs.push_back(help_node("slankdev"));
    pprint(N);

    sh->Printf("\r\n");
    for (size_t i=0; i<list.size(); i++) {
        for (command* cmd : commands) {
            if (cmd->nodestack.size() <= i) continue;

            if (strncmp(list[i].c_str(),
                        cmd->nodestack[i]->to_string().c_str(),
                        list[i].length())==0) {
                sh->Printf("  \"%s\", \"%s\"\r\n",
                        list[i].c_str(),
                        cmd->nodestack[i]->to_string().c_str());
            }
        }
    }
#endif
}



} /* namespace slankdev */





