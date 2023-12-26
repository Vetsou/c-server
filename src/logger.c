#include "include/logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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
    logger->filepath = NULL;
  }

  logger = NULL;
}

extern void log_message(ServerLogger *logger, LOG_LEVEL level, const char *format, ...) {
  char log_msg[1024] = {0};
  const char *label = log_level_label[level];
  
  // Parse args
  va_list args;
  va_start(args, format);
  vsnprintf(log_msg, sizeof(log_msg) / sizeof(char), format, args);
  va_end(args);

  if (logger->mode & LOG_MODE_CONSOLE) {
    printf("[%s] %s\n", label, log_msg);
  }

  if (logger->mode & LOG_MODE_FILE) {
    FILE *file = NULL;
    fopen_s(&file, logger->filepath, "a");
    fprintf(file, "[%s] %s\n", label, log_msg);
    fclose(file);
  }
}