#ifndef HELPER_H
#define HELPER_H

#include <stdarg.h>
#include <stdlib.h>

typedef enum { INFO, OK, WARN, FAIL } Trig;

#define HOME_DIR getenv("HOME")
#define LOCAL_CONFIG_DIR ".tig"
#define GLOBAL_CONFIG_DIR "/.config/tig"

#define INFO_COLOR "\033[36m"
#define OK_COLOR "\033[32m"
#define WARN_COLOR "\033[33m"
#define FAIL_COLOR "\033[31m"
#define RESET "\033[0m"

int printA(int mode, const char *fmt, ...);
int getGlobal(char **out);
int getGlobalFile(char **out);
int getLocal(char **out);
int getLocalFile(char **out);
int getDir(char *s1, char *s2, char **out);
int readFile_(char *path, char **out);
int writeFile_(char *path, const char *content, ...);

#endif
