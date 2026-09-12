#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>

#define MAXB 1028

int printA(int mode, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  if (mode == INFO) {
    printf("[ \033[36mINFO\033[0m ] ");
  }

  if (mode == OK) {
    printf("[  \033[32mOK\033[0m  ] ");
  }

  if (mode == WARN) {
    printf("[ \033[33mWARN\033[0m ] ");
  }

  if (mode == FAIL) {
    printf("[ \033[31mFAIL\033[0m ] ");
  }

  vprintf(fmt, args);
  va_end(args);

  return 0;
}

int getGlobal(char **out) {

  char *buffer = malloc(MAXB);

  if (buffer == NULL)
    return -1;

  snprintf(buffer, MAXB, "%s%s", HOME_DIR, GLOBAL_CONFIG_DIR);

  *out = buffer;

  return 0;
}

int getGlobalFile(char **out) {

  char *buffer = malloc(MAXB);

  if (buffer == NULL)
    return -1;

  snprintf(buffer, MAXB, "%s%s/config", HOME_DIR, GLOBAL_CONFIG_DIR);

  *out = buffer;

  return 0;
}

int getLocal(char **out) {

  char *buffer = malloc(MAXB);

  if (buffer == NULL)
    return -1;

  snprintf(buffer, MAXB, "%s", LOCAL_CONFIG_DIR);

  *out = buffer;

  return 0;
}

int getLocalFile(char **out) {

  char *buffer = malloc(MAXB);

  if (buffer == NULL)
    return -1;

  snprintf(buffer, MAXB, "%s/config", LOCAL_CONFIG_DIR);

  *out = buffer;

  return 0;
}

int getDir(char *s1, char *s2, char **out) {

  char *buffer = malloc(MAXB);

  if (buffer == NULL)
    return -1;

  snprintf(buffer, MAXB, "%s/%s", s1, s2);

  *out = buffer;

  return 0;
}

int readFile_(char *path, char **out) {
  FILE *file = fopen(path, "rb");

  if (file == NULL)
    return -1;

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *data = malloc(size);
  fread(data, 1, size, file);

  fclose(file);

  *out = data;

  return 0;
}

int writeFile_(char *path, const char *content, ...) {
  FILE *file = fopen(path, "w");

  if (file == NULL)
    return -1;

  va_list args;
  va_start(args, content);

  if (vfprintf(file, content, args) < 0) {
    va_end(args);
    return -1;
  }
  va_end(args);
  return 0;
}
