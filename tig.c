#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "helper.h"

#define TIG_CONFIG_DIR "/.config/tig"
#define TIG_LOCAL_CONFIG_DIR "/.tig/"

int createDirectory(char *filename) {
  int returnCode = mkdir(filename, 0700);
  return returnCode;
}

int createFile(char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    return -1;
  }
  fclose(file);
  return 0;
}

int setup() {
  struct stat stats;

  int warnC = 0;
  int okC = 0;

  char *out;

  printA(INFO, "TIG version=DEVELOPMENT\n");
  printA(WARN, "No compatibility inputted.\n\t Defaulting to tig-only\n");
  warnC += 1;
  printA(INFO,
         "State 1: Initializing repository...\n\t tig-only repository\n\n");

  if (getGlobal(&out) != 0) {
    printA(FAIL, "Config directory fetch failed. Aborting...\n");
    return -1;
  }

  printA(INFO, "Global config directory creation... \n");
  if (stat(out, &stats) != 0) {
    printA(INFO, "Directory %s not created. Creating... \n", out);
    if (createDirectory(out) != 0) {
      printA(FAIL, "Failed to create directory %s\n", out);
      return -1;
    }
    printA(OK, "Created directory %s successfully\n", out);
    okC += 1;
  } else {
    printA(OK, "Directory already created.\n");
    okC += 1;
  }

  if (getGlobalFile(&out) != 0) {
    printA(FAIL, "Directory of global config fething failed. Aborting...\n");
    return -1;
  }

  printA(INFO, "Config file creation... \n");
  if (stat(out, &stats) != 0 || !S_ISREG(stats.st_mode)) {
    printA(INFO, "File not already created. Creating... \n");
    if (createFile(out) != 0) {
      printA(FAIL, "Failed to create config. Aborting... {%s}{%s}{%s}\n", out,
             HOME_DIR, GLOBAL_CONFIG_DIR);
      return -1;
    }
    printA(OK, "Created file config/\n");
    okC += 1;
  } else {
    printA(OK, "File already created.\n");
    okC += 1;
  }

  if (getLocal(&out) != 0) {
    printA(FAIL, "Failed to fetch directory. Aborting...\n");
    return -1;
  }

  printA(INFO, "Creating %s directory...\n", out);
  if (stat(out, &stats) != 0) {
    printA(INFO, "Directory %s not created. Creating... \n", out);
    if (createDirectory(".tig") != 0) {
      printA(FAIL, "Failed to create directory %s\n", out);
      return -1;
    }
    printA(OK, "Created directory %s successfully\n", out);
    okC += 1;
  } else {
    printA(OK, "Directory already created.\n");
    okC += 1;
  }

  if (getLocalFile(&out) != 0) {
    printA(FAIL, "Failed to fetch local config file. Aborting...\n");
    return -1;
  }

  printA(INFO, "Local config file creation...\n");
  if (stat(out, &stats) != 0 || !S_ISREG(stats.st_mode)) {
    int a = 0;
    printA(INFO, "File %s not already created. Creating... \n", out);
    if (createFile(out) != 0) {
      printA(WARN, "Failed to create local config %s. Continuing...\n", out);
      a += 1;
      warnC += 1;
    }
    if (a != 1) {
      printA(OK, "Created local file config %s successfully\n", out);
      okC += 1;
    }
  } else {
    printA(OK, "File already created.\n");
    okC += 1;
  }

  printf("\n");
  printA(INFO,
         "STAGE 1: Success, With %d warnings.\n\t \033[32mOK\033[0m: %d\t "
         "\033[33mWARN\033[0m: %d\n",
         warnC, okC, warnC);

  printf("\n");
  printA(INFO, "Stage 2: Config propagation...\n\n");

  printA(INFO, "Reseting count...\n");
  warnC = 0;
  okC = 0;
  printA(OK, "Count reset success!\n");
  okC += 1;

  if (getGlobalFile(&out) != 0) {
    printA(FAIL, "Fatal Error: {%s/%s} does not exist!\n", HOME_DIR,
           TIG_CONFIG_DIR);
    return -1;
  }

  if (writeFile_(out, "[GLOBAL]\n\tdefault: tig-only\n") != 0) {
    printA(FAIL, "Fatal Error: failed to propagate global config file!\n");
    return -1;
  }
  printA(OK, "Success in propagating %s\n", out);
  okC += 1;

  if (getLocalFile(&out) != 0) {
    printA(FAIL, "Warning: {%s} does not exist!\n", TIG_LOCAL_CONFIG_DIR);
    return -1;
  }

  if (writeFile_(
          out,
          "[LOCAL]\n\tdefault: <placeholder> (global takes precedence)\n") !=
      0) {
    printA(WARN, "Warning: failed to propagate local config file!\n");
    warnC += 1;
  } else {
    printA(OK, "Success in propagating %s\n", out);
    okC += 1;
  }

  printf("\n");
  printA(INFO,
         "STAGE 2: Success, With %d warnings.\n\t \033[32mOK\033[0m: "
         "%d\t\033[33mWARN\033[0m: %d\n",
         warnC, okC, warnC);

  free(out);
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2 || argc > 2) {
    fprintf(stderr, "Usage: %s <flags>\n", argv[0]);
    return EXIT_FAILURE;
  }
  if (strcmp(argv[1], "setup") == 0) {
    setup();
  }
  return EXIT_SUCCESS;
}
