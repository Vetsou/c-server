#include "include/cse_logger.h"
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

static const char* CSE_GetLogLabelCstr(CSE_LOG_LEVEL log_level) {
  switch (log_level) {
    case LOG_INFO: return "INFO";
    case LOG_WARN: return "WARN";
    case LOG_ERROR: return "ERROR";
    case LOG_DEBUG: return "DEBUG";
    default: assert(0 && "CSE_GetLogLabelCstr: Unreachable code");
  }
  return "ERROR";
}



CSE_Logger* CSE_InitLogger(const char *log_filepath, CSE_LOG_MODE mode) {
  size_t path_size = strlen(log_filepath) + 1;
  char *log_file_path = (char *)malloc(path_size);
  memcpy(log_file_path, log_filepath, path_size);

  CSE_Logger *logger = (CSE_Logger *)malloc(sizeof(CSE_Logger));
  *logger = (CSE_Logger) {
    .filepath = log_file_path,
    .mode = mode
  };

  return logger;
}

void CSE_LogMsg(CSE_Logger *logger, CSE_LOG_LEVEL level, const char *msg_format, ...) {
  char log_msg[CSE_LOG_LEN_LIMIT] = { 0 };

  va_list args;
  va_start(args, msg_format);
  vsnprintf(log_msg, CSE_LOG_LEN_LIMIT, msg_format, args);
  va_end(args);

  if (logger->mode & LOG_TARGET_CONSOLE) {
    printf("[%s] %s\n", CSE_GetLogLabelCstr(level), log_msg);
  }

  if (logger->mode & LOG_TARGET_FILE) {
    FILE *file = NULL;
    fopen_s(&file, logger->filepath, "a");
    fprintf(file, "[%s] %s\n", CSE_GetLogLabelCstr(level), log_msg);
    fclose(file);
  }
}

void CSE_FreeLogger(CSE_Logger *logger) {
  if (logger->filepath) {
    free(logger->filepath);
    logger->filepath = NULL;
  }

  free(logger);
  logger = NULL;
}