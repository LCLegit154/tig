#ifndef HELPER_H
#define HELPER_H

#include <stdarg.h>
#include <stdlib.h>

typedef enum { INFO, OK, WARN, FAIL } Trig;

#define HOME_DIR getenv("HOME")
#define LOCAL_CONFIG_DIR ".tig"
#define GLOBAL_CONFIG_DIR "/.config/tig"

int printA(int mode, const char *fmt, ...);
int getGlobal(char **out);
int getGlobalFile(char **out);
int getLocal(char **out);
int getLocalFile(char **out);
int getDir(char *s1, char *s2, char **out);
int readFile_(char *path, char **out);
int writeFile_(char *path, const char *content, ...);

#endif
