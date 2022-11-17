#include "regex.h"

#include <cstdio>
#include <cstring>
#include <cctype>


int match(char *regexp, char *text, Options opts)
{
  if (regexp[0] == '^')
  {
    return matchhere(regexp + 1, text, opts);
  }
  do
  {
    if (matchhere(regexp, text, opts))
    {
      return 1;
    }
  } while (*text++ != '\0');
  return 0;
}

int matchhere(char *regexp, char *text, Options opts)
{
  if (regexp[0] == '\0')
  {
    return 1;
  }
  if (regexp[1] == '*')
  {
    return matchstar(regexp[0], regexp + 2, text, opts);
  }
  if (regexp[1] == '+')
  {
    if (regexp[0] == '.' || regexp[0] == *text) {
        return matchstar(regexp[0], regexp + 2, text, opts);
    }
    return 0;
  }
  if (regexp[0] == '$' && regexp[1] == '\0')
  {
    return *text == '\0';
  }
  if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text || (opts.caseinsensitive && regexp[0] == tolower(*text))))
  {
    return matchhere(regexp + 1, text + 1, opts);
  }
  return 0;
}

int matchstar(int c, char *regexp, char *text, Options opts)
{
  do
  {
    if (matchhere(regexp, text, opts))
    {
      return 1;
    }
  } while (*text != '\0' && (c == '.' || opts.caseinsensitive ? tolower(*text++) == c : *text++ == c ));
  return 0;
}
