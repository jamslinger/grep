#include "regex.h"

#include <cstdio>
#include <cstring>

int main(int argc, char *argv[])
{
  int i, nmatch;
  FILE *f;

  if (argc < 2) {
    fprintf(stderr, "usage: grep regexp [file ...]");
  }
  
  nmatch = 0;

  if (argc == 2) {
    if (grep(argv[1], stdin, NULL)) {
      nmatch++;
    }
  } else {
    for (i = 2; i < argc; i++) {
      f = fopen(argv[i], "r");
      if (f == NULL) {
        printf("WARNING: Can't open %s:", argv[i]);
        continue;
      }
      if (grep(argv[1], f, argc > 3 ? argv[i] : NULL) > 0) {
        nmatch++;
      }
      fclose(f);
    }
  }
  return nmatch == 0;
}

int grep(char *regexp, FILE *f, char *name) {
  int n, nmatch;
  char buf[BUFSIZ];

  nmatch = 0;
  while (fgets(buf, sizeof buf, f) != NULL) {
    n = strlen(buf);
    if (n > 0 && buf[n-1] == '\n') {
      buf[n-1] = '\0';
    }
    if (match(regexp, buf)) {
      nmatch++;
      if (name != NULL) {
        printf("%s: ", name);
      }
      printf("%s\n", buf);
    }
  }
  return nmatch;
}
