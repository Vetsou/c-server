#ifndef _LOGGER_H
#define _LOGGER_H

typedef enum {
  LOG_LEVEL_INFO = 0,
  LOG_LEVEL_WARN,
  LOG_LEVEL_ERROR,
  LOG_LEVEL_DEBUG
} LOG_LEVEL;

typedef enum {
  LOG_MODE_CONSOLE = 0b01,
  LOG_MODE_FILE = 0b10
} LOG_MODE;

typedef struct {
  volatile LOG_MODE mode;
  char *filepath;
} ServerLogger;

// Create/Destroy logger
extern void create_logger(ServerLogger *logger, const char *log_filepath, LOG_MODE mode);
extern void destroy_logger(ServerLogger *logger);

// Log message
extern void log_message(ServerLogger *logger, LOG_LEVEL level, const char *msg);

#endif //_LOGGER_H