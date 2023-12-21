#include "include/logger.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *log_level_label[] = {
  "INFO",
  "WARN",
  "ERROR"
};

extern void create_logger(ServerLogger *logger, LOG_MODE mode) {
  logger = (ServerLogger *)malloc(sizeof *logger);
  logger->mode = mode;
}

extern void destroy_logger(ServerLogger *logger) {
  free(logger);
}

extern void log_message(ServerLogger *logger, LOG_LEVEL level, const char *msg) {
  const char *label = log_level_label[level];

  char log_msg[512] = {0};
  snprintf(log_msg, sizeof(log_msg), "%s %s\n", label, msg);

  if (logger->mode & LOG_MODE_CONSOLE) {
    printf("%s\n", log_msg);
  }
}