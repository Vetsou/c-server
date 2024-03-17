#ifndef _CSE_LOGGER
#define _CSE_LOGGER

#define CSE_LOG_LEN_LIMIT 1024

typedef enum {
  LOG_INFO = 0,
  LOG_WARN,
  LOG_ERROR,
  LOG_DEBUG
} CSE_LOG_LEVEL;

typedef enum {
  LOG_TARGET_CONSOLE = 0b01,
  LOG_TARGET_FILE = 0b10
} CSE_LOG_MODE;

typedef struct {
  CSE_LOG_MODE mode;
  char *filepath;
} CSE_Logger;

extern CSE_Logger* CSE_InitLogger(const char *log_filepath, CSE_LOG_MODE mode);
extern void CSE_LogMsg(CSE_Logger *logger, CSE_LOG_LEVEL level, const char *msg_format, ...);
extern void CSE_FreeLogger(CSE_Logger *logger);

#endif // _CSE_LOGGER