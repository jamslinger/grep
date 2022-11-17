#include <cstdio>

#ifndef REGEX_H
#define REGEX_H

struct Options {
  bool negate = false;
  bool caseinsensitive = false;
  bool printlinenumbers = false;
};

int grep(char *regexp, FILE *f, char *name, Options opts);

int match(char *regexp, char *text, Options opts);

int matchhere(char *regexp, char *text, Options opts);

int matchstar(int c, char *regexp, char *text, Options opts);

int matchplus(int c, char *regexp, char *text, Options opts);

#endif
