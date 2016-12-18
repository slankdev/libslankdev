
#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>


int main(int argc, char** argv)
{
    while (char* line = readline("slankdev> ")) {
        printf("readline: \"%s\"  \n", line);
        add_history(line);

        if (strcmp(line, "exit")==0) {
            free(line);
            break;
        }
        free(line);
    }
}
