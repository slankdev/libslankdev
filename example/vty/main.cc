


#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <slankdev/string.h>
#include <slankdev/asciicode.h>
#include <slankdev/exception.h>
#include <slankdev/util.h>

#include <slankdev/vty.h>


// using namespace slankdev;

struct echo : public slankdev::command {
    echo()
    {
        nodes.push_back(new slankdev::node_fixedstring("echo", ""));
        nodes.push_back(new slankdev::node_string              );
    }
    void func(slankdev::shell* sh)
    {
        std::string s = nodes[1]->get();
        sh->Printf("%s\n", s.c_str());
    }
};

struct show_author : public slankdev::command {
    show_author()
    {
        nodes.push_back(new slankdev::node_fixedstring("show", ""));
        nodes.push_back(new slankdev::node_fixedstring("author", ""));
    }
    void func(slankdev::shell* sh)
    {
        sh->Printf("Hiroki SHIROKURA.\r\n");
        sh->Printf(" Twitter : @\r\n");
        sh->Printf(" Github  : \r\n");
        sh->Printf(" Facebook: hiroki.shirokura\r\n");
        sh->Printf(" E-mail  : slank.dev@gmail.com\r\n");
    }
};

struct show_version : public slankdev::command {
    show_version()
    {
        nodes.push_back(new slankdev::node_fixedstring("show", ""));
        nodes.push_back(new slankdev::node_fixedstring("version", ""));
    }
    void func(slankdev::shell* sh)
    {
        sh->Printf("Susanow 0.0.0\r\n");
        sh->Printf("Copyright 2017-2020 Hiroki SHIROKURA.\r\n");
    }
};

struct show : public slankdev::command {
    show()
    {
        nodes.push_back(new slankdev::node_fixedstring("show", ""));
    }
    void func(slankdev::shell* sh) { sh->Printf("show\r\n"); }
};

struct quit : public slankdev::command {
    quit() { nodes.push_back(new slankdev::node_fixedstring("quit", "")); }
    void func(slankdev::shell* sh) { sh->close(); }
};

struct clear : public slankdev::command {
    clear() { nodes.push_back(new slankdev::node_fixedstring("clear", "")); }
    void func(slankdev::shell* sh) { sh->Printf("\033[2J\r\n"); }
};

struct list : public slankdev::command {
    list() { nodes.push_back(new slankdev::node_fixedstring("list", "")); }
    void func(slankdev::shell* sh)
    {
        const std::vector<slankdev::command*>& commands = *sh->commands;
        for (slankdev::command* cmd : commands) {
            std::string s = "";
            for (slankdev::node* nd : cmd->nodes) {
                s += nd->to_string();
                s += " ";
            }
            sh->Printf("  %s\r\n", s.c_str());
        }
    }
};


char str[] = "\r\n"
    "Hello, this is Susanow (version 0.00.00.0).\r\n"
    "Copyright 2017-2020 Hiroki SHIROKURA.\r\n"
    "\r\n"
    " .d8888b.                                                             \r\n"
    "d88P  Y88b                                                            \r\n"
    "Y88b.                                                                 \r\n"
    " \"Y888b.   888  888 .d8888b   8888b.  88888b.   .d88b.  888  888  888 \r\n"
    "    \"Y88b. 888  888 88K          \"88b 888 \"88b d88\"\"88b 888  888  888 \r\n"
    "      \"888 888  888 \"Y8888b. .d888888 888  888 888  888 888  888  888 \r\n"
    "Y88b  d88P Y88b 888      X88 888  888 888  888 Y88..88P Y88b 888 d88P \r\n"
    " \"Y8888P\"   \"Y88888  88888P\' \"Y888888 888  888  \"Y88P\"   \"Y8888888P\"  \r\n"
    "\r\n";

int main(int argc, char** argv)
{
    slankdev::vty vty0(9999, str, "Susanow> ");
    vty0.install_command(new quit        );
    vty0.install_command(new clear       );
    vty0.install_command(new echo        );
    vty0.install_command(new list        );
    vty0.install_command(new show        );
    vty0.install_command(new show_author );
    vty0.install_command(new show_version);
    vty0.dispatch();
}




