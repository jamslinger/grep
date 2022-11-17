#include "regex.h"

#include <cstdio>
#include <cstring>

int main(int argc, char *argv[])
{
  int i, nmatch;
  FILE *f;
  Options opts;

  if (argc < 2) {
    fprintf(stderr, "usage: grep regexp [option ...] [file ...]");
  }
  
  nmatch = 0; 

  for (i = 1; i < argc && argv[i][0] == '-'; i++) {
    if (strlen(argv[i]) < 2) {
      fprintf(stderr, "usage: grep [option ...] regexp [file ...]");
    } else {
      for (size_t j = 0; j < strlen(argv[i]); j++) {
        switch (argv[i][j])
        {
          case 'i':
            opts.caseinsensitive = true;
            break;
          case 'v':
            opts.negate = true;
            break;
          case 'n':
            opts.printlinenumbers = true;
          default:
            break;
        }
      }
    }
  }

  if (i == argc) {
    fprintf(stderr, "usage: grep [option ...] regexp [file ...]");
  }

  char *pattern = argv[i++];

  if (i == argc) {
    if (grep(pattern, stdin, NULL, opts)) {
      nmatch++;
    }
  } else {
    int nfiles = argc - i;
    for (; i < argc; i++)
    {
      f = fopen(argv[i], "r");
      if (f == NULL) {
        printf("WARNING: Can't open %s:", argv[i]);
        continue;
      }
      if (grep(pattern, f, nfiles > 1 ? argv[i] : NULL, opts) > 0) {
        nmatch++;
      }
        fclose(f);
    }
    }
  return opts.negate ? nmatch != 0 : nmatch == 0;
}

int grep(char *regexp, FILE *f, char *name, Options opts) {
  int n, nmatch;
  char buf[BUFSIZ];

  nmatch = 0;
  int lnumber = 0;
  while (fgets(buf, sizeof buf, f) != NULL)
  {
    n = strlen(buf);
    if (n > 0 && buf[n-1] == '\n') {
      buf[n-1] = '\0';
    }
    if (buf[n - 1] == '\0' || feof(f))
    {
      lnumber++;
    }
    if (match(regexp, buf, opts)) {
      nmatch++;
      if (opts.negate) {
        continue;
      }
      if (name != NULL) {
        printf("%s:", name);
      }
      if (opts.printlinenumbers) {
        printf("%d:", lnumber);
      }
      printf("%s\n", buf);
    }
  }
  return nmatch;
}
