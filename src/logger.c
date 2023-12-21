#include "include/logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *log_level_label[] = {
  "INFO",
  "WARN",
  "ERROR",
  "DEBUG"
};

extern void create_logger(ServerLogger *logger, const char *log_filepath, LOG_MODE mode) {
  logger->mode = mode;
  size_t path_size = strlen(log_filepath) + 1;
  logger->filepath = (char *)malloc(path_size);
  memcpy(logger->filepath, log_filepath, path_size);
}

extern void destroy_logger(ServerLogger *logger) {
  if (logger->filepath) {
    free(logger->filepath);
  }
}

extern void log_message(ServerLogger *logger, LOG_LEVEL level, const char *msg) {
  const char *label = log_level_label[level];

  char log_msg[512] = {0};
  snprintf(log_msg, sizeof(log_msg), "%s %s\n", label, msg);

  if (logger->mode & LOG_MODE_CONSOLE) {
    printf("%s\n", log_msg);
  }

  if (logger->mode & LOG_MODE_FILE) {
    FILE *file = NULL;
    fopen_s(&file, logger->filepath, "a");
    fprintf(file, "%s", log_msg);
    fclose(file);
  }
}