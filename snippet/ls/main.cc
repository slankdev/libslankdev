
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

static void list_dir(const char *base_path) {
  DIR *dir;
  struct dirent *dent;
  dir = opendir(base_path);
  if (dir == NULL) {
    perror(base_path);
    return;
  }
  while ((dent = readdir(dir)) != NULL) {
    printf("%s\n", dent->d_name);
  }
  closedir(dir);
}

int main(int argc, char**argv) {
  if (argc > 1) list_dir(argv[1]);
  else list_dir("./");
}


