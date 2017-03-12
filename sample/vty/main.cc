
#include <slankdev/vty.h>

using slankdev::vty;


struct Cmd_show : public slankdev::vty::cmd_node {
    struct author : public cmd_node {
        author() : cmd_node("author") {}
        void function(vty::shell* sh)
        {
            sh->Printf("Hiroki SHIROKURA.\r\n");
            sh->Printf(" Twitter : @slankdev\r\n");
            sh->Printf(" Github  : slankdev\r\n");
            sh->Printf(" Facebook: hiroki.shirokura\r\n");
            sh->Printf(" E-mail  : slank.dev@gmail.com\r\n");
        }
    };
    struct version : public cmd_node {
        version() : cmd_node("version") {}
        void function(vty::shell* sh)
        {
            sh->Printf("Susanow 0.0.0\r\n");
            sh->Printf("Copyright 2017-2020 Hiroki SHIROKURA.\r\n");
        }
    };
    struct thread_info : public cmd_node {
        thread_info() : cmd_node("thread-info") {}
        void function(vty::shell* sh)
        {
            sh->Printf("show thread-info\r\n");
        }
    };
    Cmd_show() : cmd_node("show")
    {
        commands.push_back(new author);
        commands.push_back(new version);
        commands.push_back(new thread_info);
    }
    void function(vty::shell* sh) { sh->Printf("show\r\n"); }
};
struct Cmd_quit : public slankdev::vty::cmd_node {
    Cmd_quit() : cmd_node("quit") {}
    void function(vty::shell* sh) { sh->close(); }
};





int main()
{
    char str[] = "\r\n"
        "Hello, this is Susanow (version 0.00.00.0).\r\n"
        "Copyright 2017-2020 Hiroki SHIROKURA.\r\n"
        "\r\n";
    slankdev::vty vty0(9999, str);

    vty0.add_command(new Cmd_show);
    vty0.add_command(new Cmd_quit);
    vty0.dispatch();
}
